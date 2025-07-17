//
// Created by aiden on 7/17/2025.
//

#ifndef BYTE_UTILS_HPP
#define BYTE_UTILS_HPP

#include <cassert>
#include <cstring>
#include <vector>
#include <cstdint>
#include <string>
#include <variant>
#include <zlib.h>

namespace Utility {
    /**
     * Converts and dumps the provided 16-Bit value into the out vector.
     * Note that the byte is in Little Endian format
     *
     * @param out A vector of type byte to dump the value to
     * @param value The value to convert and write into the byte vector
     */
    void writeUint16(std::vector<std::byte> &out, uint16_t value);

    /**
     * Converts and dumps the provided 32-Bit value into the out vector.
     * Note that the byte is in Little Endian format
     *
     * @param out A vector of type byte to dump the value to
     * @param value The value to convert and write into the byte vector
     */
    void writeUint32(std::vector<std::byte> &out, uint32_t value);

    /**
     * Converts and dumps the provided 64-Bit value into the out vector.
     * Note that the byte is in Little Endian format
     *
     * @param out A vector of type byte to dump the value to
     * @param value The value to convert and write into the byte vector
     */
    void writeUint64(std::vector<std::byte> &out, uint64_t value);

    /**
     * Patches/Updates an existing 64-Bit memory location with provided data
     * Note that the byte is in Little Endian format
     *
     * @param out A vector of type byte to dump the value to
     * @param offset Offset by which to patch mem
     * @param value The value to convert and write into the byte vector
     */
    void patchUint64(std::vector<std::byte> &out, size_t offset, uint64_t value);

    void writeString(std::vector<std::byte> &out, const std::string &s);

    using Field = std::variant<int, std::string, double>;

    void writeVariant(std::vector<std::byte> &out, const Field &value);

    void writeMagicBytes(std::vector<std::byte> &out);

    uint32_t computeCRC32(const std::vector<std::byte> &data, size_t offset, size_t length);
}

#endif //BYTE_UTILS_HPP
