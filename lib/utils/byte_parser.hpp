//
// Created by aiden on 7/17/2025.
//

#ifndef BYTE_PARSER_HPP
#define BYTE_PARSER_HPP

#include <cassert>
#include <cstring>
#include <vector>
#include <cstdint>
#include <string>
#include <variant>
#include <optional>

#include "lib/entry/core_constants.hpp"
#include "lib/entry/key.hpp"


namespace Utility {
    enum FieldTag : std::underlying_type_t<std::byte> {
        INT = 0x00,
        STRING = 0x01,
        DOUBLE = 0x02,
    };

    static constexpr std::string_view magic = "ENTRY";

    class ByteParser {
    private:
        const std::byte *data_;
        size_t cursor_;
        size_t length_;

    public:
        ByteParser(const std::byte *data, const size_t length): data_(data), cursor_(0), length_(length) {
        }

        /**
        * Converts and dumps the provided 16-Bit value into the out vector.
        * Note that the byte is in Little Endian format
        *
        * @param out A vector of type byte to dump the value to
        * @param value The value to convert and write into the byte vector
        */
        static void writeUint16(std::vector<std::byte> &out, uint16_t value);

        [[nodiscard]] uint16_t readUint16();

        /**
         * Converts and dumps the provided 32-Bit value into the out vector.
         * Note that the byte is in Little Endian format
         *
         * @param out A vector of type byte to dump the value to
         * @param value The value to convert and write into the byte vector
         */
        static void writeUint32(std::vector<std::byte> &out, uint32_t value);

        [[nodiscard]] uint32_t readUint32();

        /**
         * Converts and dumps the provided 64-Bit value into the out vector.
         * Note that the byte is in Little Endian format
         *
         * @param out A vector of type byte to dump the value to
         * @param value The value to convert and write into the byte vector
         */
        static void writeUint64(std::vector<std::byte> &out, uint64_t value);

        [[nodiscard]] uint64_t readUint64();

        /**
         * Patches/Updates an existing 64-Bit memory location with provided data
         * Note that the byte is in Little Endian format
         *
         * @param out A vector of type byte to dump the value to
         * @param offset Offset by which to patch mem
         * @param value The value to convert and write into the byte vector
         */
        static void patchUint64(std::vector<std::byte> &out, size_t offset, uint64_t value);

        static void writeString(std::vector<std::byte> &out, const std::string &s);

        [[nodiscard]] std::string readString();

        static void writeVariant(std::vector<std::byte> &out, const core::Field &value);

        [[nodiscard]] std::optional<core::Field> readVariant();

        static void writeKey(std::vector<std::byte> &out, const core::Key &key);

        core::Key readKey();

        [[nodiscard]] std::byte readByte();

        static void writeMagicBytes(std::vector<std::byte> &out);
    };
}

#endif //BYTE_PARSER_HPP
