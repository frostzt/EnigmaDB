//
// Created by frostzt on 7/22/2025.
//

#ifndef WAL_CODEC_HPP
#define WAL_CODEC_HPP

#include <array>
#include <cassert>
#include <fstream>
#include <optional>
#include <cstring>

#include "lib/entry/entry.hpp"
#include "spdlog/spdlog.h"

namespace WAL {
    enum class FlushMode {
        NO_FLUSH,
        FORCE_FLUSH,
    };

    static constexpr size_t MAGIC_SIZE = 5;
    static constexpr std::array<char, 5> MAGIC = {'W', 'A', 'L', '0', '1'};

    /**
     * Writes a record to the provided output file stream. The record consists of magic bytes,
     * serialized payload length, and the serialized payload. Optionally flushes the data to disk
     * based on the flush mode.
     *
     * @param out The output file stream to write the record to. Must be open and valid.
     * @param entry The entry to be serialized and written into the output file.
     * @param forceFlush Specifies whether the stream should be explicitly flushed to disk.
     *                   Defaults to `FlushMode::NO_FLUSH`.
     * @return The total number of bytes written to the output stream.
     */
    inline uint32_t writeRecord(std::ofstream &out, const Entry &entry,
                                const FlushMode forceFlush = FlushMode::NO_FLUSH) {
        assert(out.is_open());

        uint32_t totalBytesWritten = 0;

        // Write the magic bytes
        out.write(MAGIC.data(), MAGIC_SIZE);
        totalBytesWritten += MAGIC_SIZE;

        // Serialize the entry
        const std::vector<std::byte> serialized = entry.serialize();
        const uint32_t payloadLength = serialized.size();

        // Write payload length
        out.write(reinterpret_cast<const char *>(&payloadLength), sizeof(payloadLength));
        totalBytesWritten += sizeof(payloadLength);

        // Write payload
        out.write(reinterpret_cast<const char *>(serialized.data()), payloadLength);
        totalBytesWritten += payloadLength;

        // Flush this into Disk if explicitly specified, otherwise we go the batched durability way
        if (forceFlush == FlushMode::FORCE_FLUSH) { out.flush(); }

        return totalBytesWritten;
    }

    /**
     * Reads a record from the provided input file stream. The record is deserialized
     * and returned as an optional Entry object. This includes reading magic bytes,
     * payload length, and the serialized payload. Logs errors in case of failure.
     *
     * @param in The input file stream to read the record from. Must be open and valid.
     * @return An optional Entry object representing the deserialized record.
     *         Returns std::nullopt if the record cannot be read or deserialized.
     */
    inline std::optional<Entry> readRecord(std::ifstream &in) {
        assert(in.is_open());

        // Read the magic bytes
        char magic[MAGIC_SIZE];
        in.read(magic, MAGIC_SIZE);

        if (in.gcount() < MAGIC_SIZE) return std::nullopt;
        if (std::memcmp(magic, MAGIC.data(), MAGIC_SIZE) != 0) return std::nullopt;

        // Read the payload length
        uint32_t payloadLength;
        in.read(reinterpret_cast<char *>(&payloadLength), sizeof(payloadLength));
        if (in.gcount() < sizeof(payloadLength)) {
            spdlog::error("WAL: failed to read payload length");
            return std::nullopt;
        }

        // Read the payload
        std::vector<std::byte> payload(payloadLength);
        in.read(reinterpret_cast<char *>(payload.data()), payloadLength);
        if (in.gcount() < payloadLength) {
            spdlog::error("WAL: incomplete payload - skipping.");
            return std::nullopt;
        }

        // Deserialize the entry
        const auto entryOpt = Entry::deserialize(payload.data(), payload.size());
        if (!entryOpt.has_value()) {
            spdlog::warn("WAL: failed to deserialize entry");
            return std::nullopt;
        }

        return entryOpt.value();
    }
} // namespace WAL


#endif //WAL_CODEC_HPP
