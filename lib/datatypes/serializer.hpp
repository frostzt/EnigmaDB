//
// Created by aiden on 8/2/2025.
//

#pragma once
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <cstddef>
#include <cstring>
#include <vector>

#include "lib/utils/byte_parser.hpp"

namespace core::datatypes {
    template<typename T>
    class ScalarSerializer {
    public:
        static std::vector<std::byte> serialize(const T &v) {
            static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

            std::vector<std::byte> out(sizeof(T));
            std::memcpy(out.data(), &v, sizeof(T));
            return out;
        }

        static T deserialize(const std::byte *data, const size_t len) {
            static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

            if (len != sizeof(T)) throw std::runtime_error("Invalid length");
            T v;
            std::memcpy(&v, data, sizeof(T));
            return v;
        }

        static std::string toString(const T &v) {
            return std::to_string(v);
        }
    };


    template<typename T>
    class Serializer {
    public:
        /**
         * @brief Serializes the given object into a vector of bytes.
         *
         * This method converts an object of type `T` into a binary representation,
         * encapsulated in a `std::vector<std::byte>`, which can be used for storage
         * or transmission purposes. The serialization logic depends on the specific
         * type of the object and is implemented separately.
         *
         * @tparam T The type of the object to be serialized.
         * @param v The object to be serialized into a byte vector.
         * @return A vector of bytes representing the serialized form of the given object.
         */
        static std::vector<std::byte> serialize(const T &v) {
            return ScalarSerializer<T>::serialize(v);
        }

        /**
         * @brief Deserializes a vector of bytes into an object of type `T`.
         *
         * This method reconstructs an object of type `T` from its binary representation
         * provided as a pointer to a byte array and its length. The deserialization logic
         * depends on the specific type of the object and must be implemented separately.
         *
         * @tparam T The type of the object to be deserialized.
         * @param data A pointer to the byte array containing the serialized representation of the object.
         * @param len The length of the byte array.
         * @return The deserialized object of type `T`.
         */
        static T deserialize(const std::byte *data, const size_t len) {
            return ScalarSerializer<T>::deserialize(data, len);
        }

        /**
         * @brief Converts the given object of type `T` into its string representation.
         *
         * This method generates a string representation for the object provided as input.
         * The conversion logic relies on the availability of an appropriate `std::to_string`
         * implementation for the type `T`.
         *
         * @tparam T The type of the object to be converted to a string.
         * @param v The object to be converted into a string representation.
         * @return A string representation of the given object.
         */
        static std::string toString(const T &v) {
            return ScalarSerializer<T>::toString(v);
        }
    };

    template<>
    class Serializer<std::string> {
    public:
        static std::vector<std::byte> serialize(const std::string &v) {
            assert(v.size() <= UINT16_MAX);
            std::vector<std::byte> out;
            Utility::ByteParser::writeString(out, v);
            return out;
        }

        static std::string deserialize(const std::byte *data, const size_t len) {
            const uint16_t size = Utility::ByteParser::readUint16(data, 0);
            if (len < 2) throw std::runtime_error("Invalid string length");
            std::string out(reinterpret_cast<const char *>(data + 2), size);
            return out;
        }

        static std::string toString(const std::string &v) {
            return v;
        }
    };

    template<>
    class Serializer<UUID> {
    public:
        static std::vector<std::byte> serialize(const UUID &v) {
            std::vector<std::byte> out;
            out.insert(
                out.end(),
                reinterpret_cast<const std::byte *>(v._value.data()),
                reinterpret_cast<const std::byte *>(v._value.data() + 16)
            );

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

        static std::string toString(const UUID &v) {
            return v.toString();
        }
    };

    template<>
    class Serializer<std::vector<uint8_t> > {
    public:
        static std::vector<std::byte> serialize(const std::vector<uint8_t> &v) {
            std::vector<std::byte> out;
            out.insert(
                out.end(),
                reinterpret_cast<const std::byte *>(v.data()),
                reinterpret_cast<const std::byte *>(v.data() + v.size())
            );

            return out;
        }

        static std::vector<uint8_t> deserialize(const std::byte *data, const size_t len) {
            return std::vector<uint8_t>{
                reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data) + len
            };
        }

        static std::string toString(const std::vector<uint8_t> &v) {
            std::ostringstream oss;
            oss << "<bin:" << v.size() << "b>";
            return oss.str();
        }
    };

    template<>
    class ScalarSerializer<std::byte> {
    public:
        static std::vector<std::byte> serialize(std::byte v) {
            return {v};
        }

        static std::byte deserialize(const std::byte *data, size_t len) {
            if (len != 1) throw std::runtime_error("Invalid byte length");
            return data[0];
        }

        static std::string toString(std::byte v) {
            return std::to_string(std::to_integer<uint8_t>(v));
        }
    };


    template<>
    class ScalarSerializer<std::chrono::system_clock::time_point> {
    public:
        static std::vector<std::byte> serialize(const std::chrono::system_clock::time_point &tp) {
            const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
            return ScalarSerializer<int64_t>::serialize(nanos);
        }

        static std::chrono::system_clock::time_point deserialize(const std::byte *data, size_t len) {
            int64_t nanos = ScalarSerializer<int64_t>::deserialize(data, len);
            return std::chrono::system_clock::time_point(std::chrono::nanoseconds(nanos));
        }

        static std::string toString(const std::chrono::system_clock::time_point &tp) {
            const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
            return std::to_string(nanos) + "ns";
        }
    };


    template<>
    class Serializer<std::monostate> {
    public:
        static std::vector<std::byte> serialize(const std::monostate &) {
            return {}; // Nothing to write
        }

        static std::monostate deserialize(const std::byte *, size_t) {
            return {};
        }

        static std::string toString(const std::monostate &) {
            return "null";
        }
    };
} // namespace core::datatypes

#endif //SERIALIZER_HPP
