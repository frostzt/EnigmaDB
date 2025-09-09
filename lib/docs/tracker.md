# EnigmaDB — Engineering Tracker

*Last updated: 16 Aug 2025 (IST)*

This is the living, end‑to‑end tracker for EnigmaDB: status, scope, design decisions, TODOs, experiments, and reading list. It is meant to be **authoritative** for day‑to‑day execution.

---

## Legend

* **✅ Done** – implemented, tested, and merged
* **🟡 In Progress** – partially implemented or under active work
* **🧪 Planned/Experiments** – prototypes and perf studies to run
* **🔜 TODO** – backlog, not started
* **⚠️ Risk/Open Q** – needs decision or carries risk

---

## Project Snapshot

* **WAL**: ✅ Writers, rotation, periodic flush thread, `loadAll()`; codec OK. CRC util exists.
* **MemTable**: ✅ AVL‑backed store; tombstones; freeze/unfreeze; size tracking; manager with rotation.
* **Varints & Byte I/O**: ✅ Robust varint32/64; byte parser utilities.
* **Types**: ✅ `Field` variant + `TypeRegistry` + (de)serialization; UUID, Binary, etc.
* **Compression**: ✅ LZ4 wrapper + tests. No‑op compressor available.
* **SSTable Bring‑up**: 🟡 KeyEncoder/BlockEncoder/FilterPolicy interfaces created; implementations TBD.
* **Flusher**: 🔜 Wire MemTable → SSTable writer thread; gate with WAL sync.
* **Compaction/Manifest/Query/Coordinator/Cluster/Networking**: 🔜 Not implemented yet; scoped below.
* **Performance Roadmap**: 🧪 io\_uring, mmap, SIMD listed as targeted experiments once storage path is correct.

> **Note on io\_uring:**  io\_uring for WAL appends and later SSTable flush/compaction I/O to reduce syscall overhead, leverage SQ/CQ batching, file registration, and fixed buffers. We’ll test it after correctness is locked in on the POSIX path.

---

## Architecture & Components — Deep Tracker

### 0) Engineering Foundations

**Purpose**: Tooling, quality bars, and common infra.

**State**: ✅ Catch2 harness; ✅ spdlog; ✅ CMake (Win/Linux); ✅ `.clang-format`; 🧪 Sanitizers/Static‑analysis to add; 🧪 Bench harness.

**TODO / Checklist**

* 🔜 Add CI pipeline (build + tests + linters + ASan/UBSan/TSan matrix)
* 🔜 Cross‑platform file I/O abstraction (POSIX + Win32) with clean edges
* 🔜 Fuzz tests (libFuzzer/AFL++) for WAL codec and varints
* 🔜 Property‑based tests (RapidCheck) for key/order laws
* 🔜 Benchmark harness (Google Benchmark) for read/write paths
* 🔜 Observability: metrics (Prometheus), tracing (OpenTelemetry), structured logs

**Experiments**

* 🧪 Compare build flags (LTO, PGO) on hot loops (varints, encoders)
* 🧪 Impact of allocator choices (tcmalloc/jemalloc vs system)

**Research**

* Build/CI patterns in RocksDB/ClickHouse; Fuzzing case studies in storage engines

---

### 1) Write‑Ahead Log (WAL)

**Purpose**: Durability for unflushed MemTables; crash recovery.

**State**: ✅ `WALWriter`, `WALManager`, rotation, periodic flush, `loadAll()`, codec; ✅ round‑trip tests; 🟡 Integrate CRC into record format; 🔜 richer corruption handling.

**Design Notes**

* Append‑only files with size‑based rotation
* Status+fast‑pointer style decoding in codec
* Background flush thread across writers
* Recovery scans rotated files sequentially

**TODO / Checklist**

* 🔜 Integrate CRC32 into each record (header) and verify on read
* 🔜 Corruption handling policy: skip‑record vs stop‑file vs quarantine
* 🔜 Sync semantics: `fsync`/`fdatasync` policy and batching
* 🔜 Rotation manifest: minimal index of segments for faster recovery
* 🔜 Backpressure hooks when disks are slow (expose metrics)

**Experiments**

* 🧪 `write`+`fdatasync` vs `pwrite`+`FUA` (where available)
* 🧪 `mmap` vs buffered I/O for append (page cache behavior)
* 🧪 **io\_uring**: SQPOLL, IOPOLL, fixed buffers, file registration; batch submits; compare QD
* 🧪 Preallocate with `fallocate` / sparse files; measure fragmentation

**Research**

* WAL durability patterns (Postgres, RocksDB)
* Kernel I/O paths; io\_uring internals and caveats; fsync realities on ext4/xfs/ntfs

**Open Questions**

* ⚠️ Exact record header layout (magic/version/len/crc/flags)
* ⚠️ Crash‑atomics: When to consider a record committed (write barrier ordering)

---

### 2) MemTable & Manager

**Purpose**: In‑memory, ordered writes; source of truth until flush.

**State**: ✅ AVL store; ✅ tombstones; ✅ freeze/unfreeze; ✅ size tracking; ✅ manager with auto‑rotate & `flushOldestFrozen()`.

**Design Notes**

* HLC timestamps per entry for last‑write‑wins
* Composite keys via `Key{std::vector<Field>}`
* Type system with `TypeRegistry` to define ordering, (de)serialization

**TODO / Checklist**

* 🔜 Concurrency properties: validate RCU‑like or RWLock semantics under load
* 🔜 Memory accounting & soft/hard limits with backpressure to writers
* 🔜 Pluggable comparator per key schema (stable total order)
* 🔜 TTL & tombstone expiration hooks (metadata on entries)

**Experiments**

* 🧪 AVL vs Skip‑list vs ART for write/read latency trade‑offs
* 🧪 GC cadence for tombstones vs flush/compaction timing

**Research**

* MemTable structures in LevelDB/RocksDB; SSTable row ordering constraints

**Open Questions**

* ⚠️ Comparator stability across versions (upgrades must not reorder)

---

### 3) Flusher (MemTable → SSTable)

**Purpose**: Persist immutable MemTables as SSTables and advance durability frontier.

**State**: 🔜 Not implemented; Manager exposes `flushOldestFrozen()` for handoff; encoders being designed.

**Design Notes (target)**

* Dedicated flusher thread(s) reading from a frozen queue
* Pre‑flush: ensure WAL is synced up to MemTable high‑watermark
* Write SSTable with block encoder + index + filter + footer; update MANIFEST
* Publish new table atomically; remove WAL segments covered by snapshot

**TODO / Checklist**

* 🔜 Implement Flusher thread; backpressure if queue grows
* 🔜 WAL sync barrier integration and durable cutover
* 🔜 Atomic publish (temp file → rename) with fsync on directory
* 🔜 Error handling & retry policy; partial file cleanup
* 🔜 Hook metrics: flush size, duration, stall time, bytes written

**Experiments**

* 🧪 Parallel flush (multiple frozen memtables) vs single‑threaded (seek contention)
* 🧪 Large vs small memtable targets (impact on compaction/read‑amp)

**Research**

* Flush scheduling in RocksDB (write stalls, level triggers)

**Open Questions**

* ⚠️ Flush ordering guarantees (do we need strict FIFO?)

---

### 4) SSTable Format & Readers

**Purpose**: Immutable, sorted on‑disk tables with efficient point/range reads.

**State**: 🟡 Interfaces for KeyEncoder/BlockEncoder/FilterPolicy present; full implementation TBD.

**Design Notes (target)**

* **Data blocks**: prefix‑compressed or varint‑encoded entries
* **Index block**: sparse index (e.g., first key per data block)
* **Filter block**: bloom/Xor filter per block or per table
* **Footer**: fixed‑size, magic/version, offsets/lengths for blocks
* **Table meta**: min/max key, sequence/timestamp range, checksum, compression

**TODO / Checklist**

* 🔜 Finalize on‑disk row layout (key encoding, value encoding, timestamp, tombstone)
* 🔜 Implement BlockEncoder (restart‑intervals / prefix compression)
* 🔜 Implement per‑block checksums (CRC32) and whole‑table checksum
* 🔜 Implement IndexReader / TableReader, iterators (forward & reverse)
* 🔜 Pluggable compression (Noop, LZ4; later Zstd)
* 🔜 Bloom filter writer/reader; FPR configurable
* 🔜 File format versioning & compatibility plan

**Experiments**

* 🧪 Block size (4–64 KB) vs FPR vs latency
* 🧪 Prefix vs dictionary encoding for strings
* 🧪 LZ4 vs Zstd (ratio vs CPU) and compressibility by data type
* 🧪 Checksumming strategies (per‑block vs per‑file)

**Research**

* LevelDB/RocksDB table formats; Cassandra SSTable evolution; Xor filters

**Open Questions**

* ⚠️ Value format: column family vs wide rows vs raw blob first
* ⚠️ Secondary indices (out of scope now) but reserve metadata

---

### 5) Manifest & Versioning

**Purpose**: Authoritative list of live SSTables and their metadata; enables crash‑safe installs and rollbacks.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* MANIFEST journal recording table additions/removals with checksums
* Snapshot mechanism for quick bootstrap; atomic replace pattern
* Version numbers / epochs to coordinate with readers

**TODO / Checklist**

* 🔜 Implement MANIFEST as an append‑only log + periodic snapshot
* 🔜 On startup: replay MANIFEST → build version state
* 🔜 Directory fsync strategy (ensure metadata durability)

**Experiments**

* 🧪 Snapshot cadence vs restart time vs MANIFEST growth

**Research**

* RocksDB VersionSet, MANIFEST patterns; ext4/xfs directory fsync behavior

**Open Questions**

* ⚠️ Multi‑CF support later—do we shard MANIFEST per keyspace?

---

### 6) Read Path / Query Layer (Single Node)

**Purpose**: `get`, `rangeScan`, `put`, `delete` API; merges MemTables + SSTables.

**State**: 🔜 API scaffolding to define; relies on SSTable readers.

**Design Notes (target)**

* Merge views: Active MemTable, Frozen MemTables, SSTables
* Short‑circuit with bloom and sparse index; binary search within block
* Last‑write‑wins by timestamp; tombstones honored

**TODO / Checklist**

* 🔜 Define public API; error model (Status)
* 🔜 Implement MergingIterator over all sources
* 🔜 RangeScan (inclusive/exclusive bounds, pagination)
* 🔜 Batch operations; atomicity at row granularity

**Experiments**

* 🧪 Microbenchmarks for point lookups under mixed workloads
* 🧪 Effect of block cache on p50/p99 latency

**Research**

* Iterator design in LevelDB/RocksDB; Cassandra read repair basics

**Open Questions**

* ⚠️ TTL semantics: read vs compaction enforcement

---

### 7) Caches (Block/Row/Key)

**Purpose**: Reduce I/O by caching decoded blocks/rows/keys.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Block cache (LRU/Clock/ARC); pinning for iterators
* Optional row cache for hot keys; key cache for index blocks

**TODO / Checklist**

* 🔜 Implement block cache with sharded LRU
* 🔜 Integrate with TableReader; expose metrics

**Experiments**

* 🧪 Cache sizing vs hit‑rate; shard count vs lock contention

**Research**

* Caching in RocksDB/Scylla; ARC vs LRU studies

**Open Questions**

* ⚠️ Admission policies (TinyLFU?) and negative caching

---

### 8) Compaction Engine

**Purpose**: Reclaim space, reduce read amp, reconcile versions.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Tiered (leveled later): pick N overlapping tables → merge → new table(s)
* Respect tombstones and TTL; drop obsolete versions
* Throttled to avoid write stalls; IO scheduler aware

**TODO / Checklist**

* 🔜 Compaction picker (size‑tiered initially)
* 🔜 Merge iterator respecting timestamps & deletes
* 🔜 Throttle/slowdown logic; backpressure to writers

**Experiments**

* 🧪 Size‑tiered vs leveled on write amp/read amp/space amp
* 🧪 Compaction parallelism vs foreground latency
* 🧪 IO scheduling with **io\_uring** (batched reads/writes)

**Research**

* LevelDB (leveled), Cassandra (STCS/LCS/TWCS), RocksDB compaction filters

**Open Questions**

* ⚠️ Time‑windowed compaction for time‑series use cases

---

### 9) Coordinator (Single Node → Cluster Entry)

**Purpose**: Entry point for client requests; enforces consistency when clustered.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* For single node: pass‑through to local engine
* For cluster: route to replicas based on token ring; collect acks

**TODO / Checklist**

* 🔜 Define Coordinator API (client‑facing)
* 🔜 Local fast‑path; hooks for future quorum logic

**Experiments**

* 🧪 API overhead under high QPS; thread‑pool sizing

**Research**

* Cassandra coordinator behavior; speculative reads

**Open Questions**

* ⚠️ Back‑pressure from storage to coordinator under overload

---

### 10) Token Ring & Partitioning

**Purpose**: Shard keys across cluster via consistent hashing & vnodes.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Hash partitioner (Murmur3/XXH3) → 64/128b tokens
* Virtual nodes per physical node for balance
* Replication factor → replica set per token range

**TODO / Checklist**

* 🔜 Token assignment & ring metadata
* 🔜 Simple rebalancing (add/remove node) plan

**Experiments**

* 🧪 Skew under different vnode counts; hot‑key mitigation

**Research**

* Cassandra partitioner & vnode design; jump consistent hash

**Open Questions**

* ⚠️ Token movement protocols and streaming

---

### 11) Replication & Repair

**Purpose**: Durability & availability across nodes; eventual convergence.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Tunable consistency: ONE/QUORUM/ALL
* Hinted handoff when replicas are down
* Read repair and anti‑entropy (Merkle trees later)

**TODO / Checklist**

* 🔜 Write path fan‑out and ack tracking
* 🔜 Hinted handoff queue & replay
* 🔜 Basic read repair on mismatches

**Experiments**

* 🧪 Latency vs consistency level trade‑offs
* 🧪 Impact of clock skew on read repair decisions (HLC)

**Research**

* Dynamo‑style replication; Cassandra read repair & repair service

**Open Questions**

* ⚠️ Conflict resolution beyond LWW (future: CRDTs?)

---

### 12) Gossip & Membership (Anti-Entropy)

**Purpose**: Discover peers, exchange liveness/state, drive ring changes.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Periodic heartbeats; phi‑accrual failure detector
* Exchange token ring and schema digests

**TODO / Checklist**

* 🔜 Gossip protocol skeleton (UDP/TCP)
* 🔜 Failure detection thresholds & quarantine

**Experiments**

* 🧪 Failure detector false‑positive rate tuning

**Research**

* Cassandra Gossip, SWIM protocols, phi‑accrual detector

**Open Questions**

* ⚠️ Secure gossip (auth/TLS) from day one?

---

### 13) Networking Layer

**Purpose**: RPC for client & inter‑node; efficient framing and backpressure.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Start with simple length‑prefixed TCP framing; later gRPC
* Timeouts, retries, exponential backoff
* Flow control/backpressure; connection pooling

**TODO / Checklist**

* 🔜 Define wire messages (Get/Put/Delete/Range, HintedHandoff, Gossip)
* 🔜 Implement blocking TCP framing; codecs reuse varints
* 🔜 Add TLS (later)

**Experiments**

* 🧪 **io\_uring** for network send/recv (Linux) vs epoll/kqueue
* 🧪 Nagle/Delayed ACK settings; keep‑alive probing

**Research**

* RPC design in Scylla (Seastar) and gRPC internals

**Open Questions**

* ⚠️ Custom RPC vs gRPC for long‑term maintainability

---

### 14) Clocks & Timestamps

**Purpose**: Order updates and reconcile replicas.

**State**: ✅ HLC implemented & tested.

**Design Notes**

* Monotonicity per node; combine physical time with logical counter
* Use in write path and during compaction

**TODO / Checklist**

* 🔜 Persist last HLC on shutdown to avoid monotonicity regressions
* 🔜 Clock skew monitoring (metrics)

**Experiments**

* 🧪 Simulate skew and bursty traffic; validate monotonicity

**Research**

* Literature on Hybrid Logical Clocks; Cassandra timestamp behavior

**Open Questions**

* ⚠️ Cross‑DC skew policies and client‑assigned timestamps

---

### 15) Schema, Data Model & Types

**Purpose**: Column families/keyspaces; types and serialization policy.

**State**: ✅ Core `Field`/`TypeRegistry` with ordering & (de)serialization.

**Design Notes**

* Composite primary keys; stable comparators
* Per‑column metadata (TTL, default value) later

**TODO / Checklist**

* 🔜 Formalize schema metadata; persist in MANIFEST or schema file
* 🔜 Migration/versioning plan for types

**Experiments**

* 🧪 Sorting stability under mixed types; stress with random generators

**Research**

* Cassandra schema (CQL), type evolution patterns

**Open Questions**

* ⚠️ Handling of collations/locales for strings (initially bytewise)

---

### 16) Observability & Ops

**Purpose**: Make the system diagnosable and operable.

**State**: 🔜 Minimal logging; metrics/tracing not yet integrated.

**Design Notes (target)**

* Metrics: counters, histograms for latencies, sizes, QPS
* Traces around write/read/flush/compact
* Health endpoints; debug dumps

**TODO / Checklist**

* 🔜 Add metrics library integration (Prometheus)
* 🔜 Span tracing via OpenTelemetry C++
* 🔜 Verbose logging gates for hot paths

**Experiments**

* 🧪 SLOs for p50/p95/p99 latencies; alerting thresholds

**Research**

* Observability in DB engines, RED/USE methodologies

**Open Questions**

* ⚠️ Cardinality control for labels (don’t explode metric space)

---

### 17) Backup/Restore & Snapshots (Future)

**Purpose**: Disaster recovery and portability.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* Filesystem snapshots or table‑copy with manifests
* Incremental backup via MANIFEST delta

**TODO / Checklist**

* 🔜 Consistent snapshot cut (pause flush/compaction briefly)

**Experiments**

* 🧪 Backup throughput and restore times

**Research**

* Cassandra snapshots/hardlinks; RocksDB checkpoints

---

### 18) Security (Future)

**Purpose**: Basic authn/z and transport security.

**State**: 🔜 Not implemented.

**Design Notes (target)**

* TLS for client & inter‑node
* Optional auth; token‑based or mTLS identities

**TODO / Checklist**

* 🔜 Pluggable auth interceptor

**Open Questions**

* ⚠️ Multi‑tenant isolation (explicitly out‑of‑scope for v1)

---

## Cross‑Cutting Invariants

* **Durability**: Acknowledged write implies WAL persisted to durability policy
* **Ordering**: Stable key comparator; HLC total order within shard
* **Immutability**: SSTables are append‑once then read‑only
* **Atomic publish**: New table visible only after full write + fsync + rename
* **Crash safety**: MANIFEST is single source of truth at startup
* **Backpressure**: Writers must slow when flush/compaction lags

---

## Roadmap & Gating Criteria

### Milestone A — Complete Single‑Node Storage Engine (MVP)

* [ ] WAL CRC integrated; corruption policy documented
* [ ] Flusher thread; WAL sync barrier; atomic SSTable publish
* [ ] SSTable v1: data/index/filter/footer + readers + iterators
* [ ] Read path merging MemTable/Frozen/SSTables
* [ ] MANIFEST v1 with replay on start
* **Gate**: End‑to‑end tests pass; crash‑recovery replay verified; p95 latency targets set

### Milestone B — Performance Pass

* [ ] Block cache (LRU) with metrics
* [ ] Bench harness & baseline results
* [ ] **io\_uring** experiments for WAL and flush path (Linux)
* [ ] Compression tuning (LZ4/Zstd) & block size tuning
* **Gate**: Benchmarks show material improvement over baseline

### Milestone C — Cluster Bring‑up (Alpha)

* [ ] Token ring + vnode assignment
* [ ] Simple gossip + failure detector
* [ ] Replication (RF=N) with ONE/QUORUM/ALL
* [ ] Coordinator routing + hinted handoff
* **Gate**: 3‑node cluster stable under Jepsen‑like basic tests (no partitions at first)

### Milestone D — Consistency & Repair (Beta)

* [ ] Read repair & basic anti‑entropy
* [ ] Time‑windowed compaction option
* [ ] Observability (metrics/tracing dashboards)
* **Gate**: Convergence after transient failures verified; chaos tests green

---

## Experiments Matrix (Quick Reference)

| Area               | Hypothesis                                  | Method                     | Metric                        |
| ------------------ | ------------------------------------------- | -------------------------- | ----------------------------- |
| WAL fsync batching | Batch improves throughput                   | Vary group commit sizes    | ops/s, p99                    |
| io\_uring vs write | Fewer syscalls → lower tail                 | Submit batches with SQPOLL | p95/p99 latency               |
| Block size         | Larger blocks reduce reads but increase CPU | Sweep 4–64KB               | Read amp, CPU%                |
| Bloom FPR          | Lower FPR reduces disk hits                 | Sweep 0.1–10%              | False positives, read latency |
| Cache shard count  | More shards reduce lock contention          | Sweep 1–64                 | Throughput, mutex wait        |
| Compaction type    | Tiered vs leveled trade‑offs                | Replay workload traces     | Write/read/space amp          |

---

## Reading & Research List (by Component)

* **WAL & Durability**: Postgres WAL internals; fsync semantics; storage write barriers
* **SSTable/LSM**: LevelDB & RocksDB table format & compaction papers; Cassandra SSTable docs
* **Filters**: Bloom filters; blocked bloom; Xor filters
* **Consistency**: Dynamo; Cassandra consistency levels; HLC literature
* **Gossip/Failure**: SWIM; phi‑accrual failure detector
* **Networking**: gRPC internals; TCP performance tuning guides
* **Perf**: io\_uring guides; CPU cache/NUMA; SIMD intro for parsing
* **Testing**: Jepsen case studies; property‑based testing in C++

---

## Open Risks & Decisions Log

* **Record header**: finalize WAL & SSTable checksum/flags layout (owner: storage)
* **Comparator stability**: define wire‑compatible sort order (owner: types)
* **RPC choice**: custom vs gRPC (owner: networking)
* **Cache policy**: LRU vs ARC/TinyLFU (owner: performance)

---

## Action Items

1. Implement WAL CRC + corruption policy → update tests
2. Finish KeyEncoder/BlockEncoder; define SSTable row format (doc + code)
3. Implement Flusher thread with WAL sync barrier and atomic publish
4. Minimal TableReader + MergingIterator; single‑node `get/put/delete`
5. Write end‑to‑end integration test: write N, crash, recover, verify

---

Maintained by: frostzt
