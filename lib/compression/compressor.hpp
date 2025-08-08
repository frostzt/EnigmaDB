//
// Created by frostzt on 8/7/2025.
//

#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <cstdint>
#include <string_view>
#include <vector>

namespace compression {
    enum class CompressorID : uint8_t {
        Noop = 0,
        LZ4 = 1,
    };

    class Compressor {
    public:
        virtual ~Compressor() = default;

        virtual void compress(const uint8_t *data, size_t len, std::vector<uint8_t> &out) = 0;

        virtual void decompress(const uint8_t *data, size_t len, std::vector<uint8_t> &out) = 0;

        [[nodiscard]] virtual std::string_view name() const = 0;

        [[nodiscard]] virtual CompressorID id() const = 0;
    };
} // namespace compression

#endif //COMPRESSOR_HPP
