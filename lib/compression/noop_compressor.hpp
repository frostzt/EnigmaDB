//
// Created by frostzt on 8/7/2025.
//

#ifndef NOOP_COMPRESSOR_HPP
#define NOOP_COMPRESSOR_HPP

#include <cstring>

#include "compressor.hpp"

namespace compression {
    // NOOP Compressor performs identity transform; used for testing or debug mode
    class NOOPCompressor final : public Compressor {
    public:
        [[nodiscard]] CompressorID id() const override {
            return CompressorID::Noop;
        }

        [[nodiscard]] std::string_view name() const override {
            return std::string_view{"noop"};
        };

        void compress(const uint8_t *data, const size_t len, std::vector<uint8_t> &out) override {
            out.resize(len);
            std::memcpy(out.data(), data, len);
        }

        void decompress(const uint8_t *data, const size_t len, std::vector<uint8_t> &out) override {
            out.resize(len);
            std::memcpy(out.data(), data, len);
        }
    };
} // namespace compression


#endif //NOOP_COMPRESSOR_HPP
