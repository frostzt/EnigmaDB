//
// Created by aiden on 8/2/2025.
//

#ifndef INT64_SERIALIZER_HPP
#define INT64_SERIALIZER_HPP

#include "lib/datatypes/serializer.hpp"
#include "lib/utils/byte_parser.hpp"
#include "lib/datatypes/types/uuid.hpp"

#include <cstdint>
#include <cassert>

namespace core::datatypes {
    template<>
    struct Serializer<std::string> {
        static std::vector<std::byte> serialize(const std::string &v) {
            assert(v.size() <= UINT16_MAX);
            std::vector<std::byte> out;
            Utility::ByteParser::writeString(out, v);
            return out;
        }

        static std::string deserialize(const std::byte *data, const size_t len) {
            const uint16_t size = Utility::ByteParser::readUint16(data, 0);
            if (len < 2 || len < 2 + len) throw std::runtime_error("Invalid string length");
            std::string out(reinterpret_cast<const char *>(data + 2), size);
            return out;
        }
    };

    template<>
    struct Serializer<UUID> {
        static std::vector<std::byte> serialize(const UUID &v) {
            std::vector<std::byte> out;
            out.insert(out.end(), v._value.begin(), v._value.end());
            return out;
        }

        static UUID deserialize(const std::byte *data, const size_t len) {
            if (len != 16) throw std::runtime_error("Invalid UUID length");
            std::array<uint8_t, 16> uuid{};

            for (int i = 0; i < 16; ++i) {
                uuid[i] = static_cast<uint8_t>(data[i]);
            }

            return UUID{uuid};
        }
    };
} // namespace core::datatypes

#endif //INT64_SERIALIZER_HPP
