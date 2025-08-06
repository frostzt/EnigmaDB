## Plan

### 🔧 Phase 1: Core Components

- [ ] LZ4Compressor and NoopCompressor
- [ ] PrefixKeyEncoder and RawKeyEncoder
- [ ] BasicBlockEncoder (uses KeyEncoder + Compressor)
- [ ] BloomFilterPolicy (with test Bloom impl)

### 📦 Phase 2: SSTable Writing

- [ ] SSTableWriter
    - Receives sorted KV pairs
    - Encodes blocks
    - Writes Index, Filter, Footer
    - Enforces block size
- [ ] Footer structure writer
- [ ] CRC32 wrapper (reusable from WAL, need to convert this to SIMD impl)

### 🔍 Phase 3: SSTable Reading

- [ ] SSTableReader
    - Loads footer
    - Binary searches index
    - Validates CRC + decompresses
    - Applies Bloom, Cuckoo or whatever bruh

### 🧪 Phase 4: Testing

- [ ] Compressor tests
- [ ] KeyEncoder roundtrip
- [ ] BlockEncoder tests (entry packing + flushing)
- [ ] Full SSTableWriter + Reader test roundtrip

## Structure for an SSTable

<pre>
[SSTable File Layout]
+-----------------------------+
| Data Block 1 |
+-----------------------------+
| Data Block 2 |
+-----------------------------+
| ... |
+-----------------------------+
| Optional Filter Block(s)    | <- e.g. Bloom or Cuckoo
+-----------------------------+
| Index Block |
+-----------------------------+
| Meta Block (optional)      | <- min/max keys, column info
+-----------------------------+
| Footer (fixed size)        |
+-----------------------------+
[Block Structure]
+-----------------------------+
| BlockHeader |
| - compressed_size |
| - uncompressed_size |
| - compression_type |
| - checksum (of compressed data)
+-----------------------------+
| Block Data (maybe compressed) |
+-----------------------------+

[Footer Structure (Fixed Size)]
+-----------------------------+
| index_block_offset (u64)   |
| filter_block_offset (u64)  |
| meta_block_offset (u64)    |
| compressor_id (u8)         |
| format_version (u8)        |
| footer_checksum (u32)      |
| magic[8] = "ENIGSSTB"      |
+-----------------------------+
</pre>
