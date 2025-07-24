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
    static constexpr size_t MAGIC_SIZE = 5;
    static constexpr std::array<char, 5> MAGIC = {'W', 'A', 'L', '0', '1'};

    inline void writeRecord(std::ofstream &out, const Entry &entry) {
        assert(out.is_open());

        // Write the magic bytes
        out.write(MAGIC.data(), MAGIC_SIZE);

        // Serialize the entry
        const std::vector<std::byte> serialized = entry.serialize();
        const uint32_t payloadLength = serialized.size();

        // Write payload length
        out.write(reinterpret_cast<const char *>(&payloadLength), sizeof(payloadLength));

        // Write payload
        out.write(reinterpret_cast<const char *>(serialized.data()), static_cast<std::streamsize>(payloadLength));

        out.flush();
    }

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
