//
// Created by frostzt on 8/7/2025.
//

#include "lz4.h"
#include "lz_4_compressor.hpp"

#include <cstring>
#include <stdexcept>

namespace compression {
    void LZ4Compressor::compress(const uint8_t *data, const size_t len, std::vector<uint8_t> &out) {
        if (len > UINT32_MAX) {
            throw std::runtime_error("LZ4Compressor only supports inputs <= 4GB");
        }

        // Resize the out vector with "worst-case" size possible
        const int maxSizePossible = LZ4_compressBound(static_cast<int>(len));
        out.resize(maxSizePossible + 4);

        // Write the size header
        const auto lenLE = static_cast<uint32_t>(len);
        std::memcpy(out.data(), &lenLE, 4);

        // Compress the data
        const int size = LZ4_compress_default(reinterpret_cast<const char *>(data),
                                              reinterpret_cast<char *>(out.data() + 4), static_cast<int>(len),
                                              maxSizePossible);
        if (size <= 0) {
            throw std::runtime_error("LZ4_compress() failed");
        }

        // Resize based on the compressed size
        out.resize(static_cast<size_t>(size) + 4);
    }

    void LZ4Compressor::decompress(const uint8_t *data, const size_t len, std::vector<uint8_t> &out) {
        if (len < 4) throw std::runtime_error("compressed buffer too small");

        // Get original size
        uint32_t originalSize = 0;
        std::memcpy(&originalSize, data, 4);

        // Resize the buffer
        out.resize(originalSize);
        const int size = LZ4_decompress_safe(reinterpret_cast<const char *>(data + 4),
                                             reinterpret_cast<char *>(out.data()),
                                             static_cast<int>(len - 4),
                                             static_cast<int>(originalSize));
        if (size != static_cast<int>(originalSize)) {
            throw std::runtime_error("LZ4_decompress_safe() failed");
        }
    }
} // namespace compression
