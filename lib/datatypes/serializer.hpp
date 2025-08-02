//
// Created by aiden on 8/2/2025.
//

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <cstddef>
#include <cstring>
#include <vector>

namespace core::datatypes {
    template<typename T>
    struct ScalerSerializer {
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
    };

    template<typename T>
    struct Serializer {
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
            return ScalerSerializer<T>::serialize(v);
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
            return ScalerSerializer<T>::deserialize(data, len);
        }
    };
} // namespace core::datatypes

#endif //SERIALIZER_HPP
