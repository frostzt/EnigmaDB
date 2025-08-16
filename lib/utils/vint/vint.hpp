//
// Created by frostzt on 8/14/2025.
//

#ifndef LSMTREE_VINT_H
#define LSMTREE_VINT_H

#include <cstdint>
#include <vector>

namespace utility {
    enum class StatusCode : uint8_t { OK=0, TRUNCATED, MALFORMED };

    /**
     * Encodes a 32-bit unsigned integer into a variable-length integer format
     * and appends it to the given output vector.
     *
     * The variable-length format encodes integers in a compact manner by using
     * the least significant 7 bits of each byte to store the integer's value,
     * while the MSB is used as a continuation flag.
     *
     * @param out The output vector to which the encoded variable-length integer
     *            will be appended.
     * @param value The 32-bit unsigned integer value to be encoded.
     */
    inline void putVarint32(std::vector<uint8_t> &out, uint32_t value) {
        const size_t start = out.size();
        size_t i = 0;
        for (; i < 5; i++) {
            out.push_back(static_cast<uint8_t>((value & 0x7Fu) | 0x80u));
            value >>= 7;
            if (value == 0) break;
        }

        out[start + i] &= 0x7Fu;
        assert(i < 5);
    }

    /**
     * Decodes a variable-length 32-bit unsigned integer from the provided data
     * pointer and updates the output parameter with the decoded value.
     *
     * The variable-length integer is expected to be encoded using the least
     * significant 7 bits per byte, with the MSB serving as a continuation flag.
     * The decoding process continues until the continuation flag (MSB) is cleared
     * or an error is encountered.
     *
     * @param status A reference to a StatusCode variable where the status of
     *               the operation will be set. It will be set to `StatusCode::OK`
     *               if decoding is successful, `StatusCode::TRUNCATED` if the data
     *               runs out before decoding completes, or `StatusCode::MALFORMED`
     *               if the input exceeds the maximum expected size of 5 bytes.
     * @param data A pointer to the beginning of the input data to be decoded.
     * @param limit A pointer to the end of the input data range. Used to ensure
     *              that decoding does not exceed the available input bounds.
     * @param out A reference to a 32-bit unsigned integer where the decoded value
     *            will be stored if decoding is successful.
     *
     * @return A pointer to the next byte in the input data after decoding is
     *         complete, or `nullptr` if an error occurred during decoding.
     */
    inline const uint8_t *getVarint32(StatusCode& status, const uint8_t *data,  const uint8_t* limit, uint32_t &out) {
        status = StatusCode::OK;
        uint32_t result = 0;
        int shift = 0;

        for (int i = 0; i < 5; ++i) {
            if (data == limit) {status = StatusCode::TRUNCATED; return nullptr;}
            const uint8_t byte = *data++;
            result |= static_cast<uint32_t>(byte & 0x7Fu) << shift;
            if ((byte & 0x80u) == 0) {
                out = result;
                return data;
            }

            shift += 7;
        }

        status = StatusCode::MALFORMED;
        return nullptr;
    }
} // namespace utility

#endif //LSMTREE_VINT_H