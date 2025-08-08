//
// Created by frostzt on 8/7/2025.
//

#ifndef LZ_4_COMPRESSOR_HPP
#define LZ_4_COMPRESSOR_HPP

#include "compressor.hpp"

namespace compression {
    class LZ4Compressor final : public Compressor {
    public:
        [[nodiscard]] CompressorID id() const override {
            return CompressorID::LZ4;
        }

        [[nodiscard]] std::string_view name() const override {
            return std::string_view{"LZ4"};
        }

        void compress(const uint8_t *data, size_t len, std::vector<uint8_t> &out) override;

        void decompress(const uint8_t *data, size_t len, std::vector<uint8_t> &out) override;
    };
} // namespace compression

#endif //LZ_4_COMPRESSOR_HPP
