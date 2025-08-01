# EnigmaDB

## 1. Vision

**EnigmaDB** is a distributed, highly available, fault-tolerant, LSM Tree-based database system inspired by Cassandra. It is optimized for fast writes, horizontal scalability, tunable consistency, and eventual convergence in distributed settings. EnigmaDB will serve as a learning-grade but production-inspired implementation of a modern distributed column-family store.

---

## 2. Goals & Non-Goals

### ✅ Goals

* Durable writes via WAL
* LSM Tree-based storage engine
* Horizontal sharding (token ring / consistent hashing)
* Tunable consistency (e.g., QUORUM, ONE, ALL)
* Compaction and SSTable flushing
* Crash-safe recovery and replay
* Simple query language or API for CRUD
* Multi-replica coordination and eventual consistency
* Network protocol for intra-cluster communication

### 🚫 Non-Goals (for now)

* Distributed transactions
* Secondary indexes
* Complex joins or SQL
* Multi-tenancy or multi-database support

---

## 3. Architecture Overview

### Core Layers:

* **WAL Layer**: Durability guarantee for unflushed MemTables.
* **MemTable**: In-memory, sorted map of recent writes.
* **LSM Tree**: Flushes MemTable to disk as immutable SSTables.
* **SSTable Layer**: On-disk columnar store with sparse indexes.
* **Compaction Engine**: Merges older SSTables to reclaim space and reduce read amplification.
* **Query Engine**: Accepts reads and writes, handles routing.
* **Cluster Layer** (Future): Token ring, replication, gossip.
* **Replication Layer**: Coordinates data redundancy across nodes.
* **Coordinator Node**: Centralized (or distributed) request manager.
* **Network Layer**: Handles all inter-node messaging.

---

## 4. Component Breakdown

### 1. `WAL`

* Append-only write log with rotation
* Timestamped entries
* Replay on crash

### 2. `MemTable`

* In-memory balanced tree (e.g., AVL or skip list)
* HLC timestamps per row
* Thread-safe writes

### 3. `SSTable`

* Immutable sorted files
* Footer with sparse index block
* Optional bloom filter
* Metadata file (min/max key, timestamp range)

### 4. `Flusher`

* Flushes MemTable to SSTable
* Creates index blocks and bloom filters
* Updates manifest

### 5. `Compactor`

* Identifies overlapping SSTables
* Merges rows by timestamp
* Deletes tombstoned rows

### 6. `Manifest`

* Tracks all SSTables
* Versioned with checksums
* Keeps recovery consistent

### 7. `Query Layer`

* Point lookup (key)
* Range scans (token range)
* Supports `get`, `put`, `delete`

### 8. `Coordinator`

* Receives client requests
* Determines replicas using token range
* Routes and collects responses
* Ensures quorum consistency levels

### 9. `Replication`

* Manages data propagation to other replicas
* Tracks pending writes and hints
* Ensures eventual consistency
* Applies hinted handoff and read repair

### 10. `Network`

* Handles RPC/peer-to-peer communication
* Health checks and gossip heartbeat
* Encodes/decodes request/response messages

### 11. `Token Ring`

* Consistent hashing partitioner
* Assigns key ranges to virtual nodes
* Scalable and balanced sharding logic

### 12. `Clock`

* Hybrid Logical Clock for ordering
* Thread-safe, overflow protected

---

## 5. Development Milestones

### ✅ Phase 1: WAL Engine

* [x] Thread-safe WAL writers
* [x] File rotation
* [x] Concurrent surge writes
* [ ] `loadAll()` recovery

### 🔜 Phase 2: MemTable + Flushing

* [ ] AVL Tree w/ timestamp versioning
* [ ] Background flush trigger
* [ ] WAL sync on flush

### 🔜 Phase 3: SSTable Format

* [ ] Row encoding
* [ ] Footer, sparse index block
* [ ] Read path (point + range)

### 🔜 Phase 4: Compaction

* [ ] File picking heuristics
* [ ] Merge/replace by timestamp
* [ ] Delete tombstones

### 🔜 Phase 5: Query Layer

* [ ] `get`, `put`, `delete` interface
* [ ] Batch writes
* [ ] Error handling

### 🔜 Phase 6: Cluster Mode (Cassandra-style)

* [ ] Token ring & partitioning
* [ ] Replication config
* [ ] Simple gossip heartbeat
* [ ] Quorum reads/writes

### 🔜 Phase 7: Replication Layer

* [ ] Define replica placement strategy
* [ ] Implement hinted handoff queue
* [ ] Enable background replica sync
* [ ] Track consistency with vector clock or HLC

### 🔜 Phase 8: Coordinator

* [ ] Receive external write/read
* [ ] Route to correct replicas
* [ ] Enforce consistency level policies
* [ ] Retry on failure

### 🔜 Phase 9: Networking Engine

* [ ] Encode/decode wire format
* [ ] Send/receive messages (mock via thread queues first)
* [ ] Add basic RPC with gRPC or TCP framing
* [ ] Enable heartbeat/gossip discovery

---

## 6. Notes for Contributor Engineers

* Every entry in WAL and MemTable must be timestamped (HLC)
* SSTable files are immutable once flushed
* Writes are always `append-only`
* Compaction must preserve last-write-wins via timestamp
* Read path must merge in-memory + disk sources
* Replication must account for message loss and recovery

---

## 7. Future Work

* RocksDB-style tiered compaction
* Secondary indexes
* LSM tree visualizer (SSTable browser)
* Cloud storage backend
* Node failover and rebalancing
* Dynamic membership (node add/remove)

---

## 8. License

MIT or Apache 2.0 — TBD

---

## 9. Author

Sourav Singh Rawat
[https://github.com/frostzt](https://github.com/frostzt)
