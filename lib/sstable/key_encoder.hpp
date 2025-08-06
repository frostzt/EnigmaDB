//
// Created by frostzt on 8/7/2025.
//

#ifndef KEY_ENCODER_HPP
#define KEY_ENCODER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace sstable {
    class KeyEncoder {
    public:
        virtual ~KeyEncoder() = default;

        virtual void encode(const std::string &lastKey, const std::string &currentKey, std::vector<uint8_t> &out) = 0;

        virtual std::string decode(const std::string &lastKey, const uint8_t *data, size_t &bytesRead) = 0;

        [[nodiscard]] virtual std::string_view name() const = 0;
    };
} // namespace sstable

#endif //KEY_ENCODER_HPP
