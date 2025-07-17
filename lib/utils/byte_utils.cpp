//
// Created by aiden on 7/17/2025.
//

#include "byte_utils.hpp"

namespace Utility {
    void writeUint16(std::vector<std::byte> &out, const uint16_t value) {
        out.push_back(static_cast<std::byte>(value & 0xFF)); // lower byte
        out.push_back(static_cast<std::byte>((value >> 8) & 0xFF)); // upper byte
    }

    void writeUint32(std::vector<std::byte> &out, const uint32_t value) {
        for (int i = 0; i < 4; i++) {
            out.push_back(static_cast<std::byte>((value >> 8 * i) & 0xFF));
        }
    }

    void writeUint64(std::vector<std::byte> &out, const uint64_t value) {
        for (int i = 0; i < 8; ++i) {
            out.push_back(static_cast<std::byte>((value >> 8 * i) & 0xFF));
        }
    }

    void patchUint64(std::vector<std::byte> &out, const size_t offset, const uint64_t value) {
        for (int i = 0; i < 8; ++i) {
            out[offset + i] = static_cast<std::byte>((value >> 8 * i) & 0xFF);
        }
    }

    void writeString(std::vector<std::byte> &out, const std::string &s) {
        assert(s.size() <= UINT16_MAX);
        writeUint16(out, static_cast<uint16_t>(s.size()));
        out.insert(out.end(),
                   reinterpret_cast<const std::byte *>(s.data()),
                   reinterpret_cast<const std::byte *>(s.data() + s.size()));
    }

    void writeVariant(std::vector<std::byte> &out, const Utility::Field &value) {
        if (std::holds_alternative<int>(value)) {
            out.push_back(static_cast<std::byte>(0x00)); // int flag
            const int v = std::get<int>(value);
            for (int i = 0; i < 4; i++) {
                out.push_back(static_cast<std::byte>((v >> (8 * i)) & 0xFF));
            }
        } else if (std::holds_alternative<std::string>(value)) {
            out.push_back(static_cast<std::byte>(0x01)); // std::string flag
            writeString(out, std::get<std::string>(value));
        } else if (std::holds_alternative<double>(value)) {
            out.push_back(static_cast<std::byte>(0x02)); // double flag
            const double v = std::get<double>(value);
            uint8_t bytes[sizeof(double)];
            std::memcpy(bytes, &v, sizeof(double));
            out.insert(out.end(),
                       reinterpret_cast<const std::byte *>(&v),
                       reinterpret_cast<const std::byte *>(&v) + sizeof(double));
        }
    }

    uint32_t computeCRC32(const std::vector<std::byte> &data, const size_t offset, const size_t length) {
        return crc32(0L, reinterpret_cast<const Bytef *>(data.data() + offset), static_cast<uInt>(length));
    }

    void writeMagicBytes(std::vector<std::byte> &out) {
        static constexpr std::string_view magic = "ENTRY";
        out.insert(out.end(), reinterpret_cast<const std::byte *>(magic.data()),
                   reinterpret_cast<const std::byte *>(magic.data() + magic.size()));
    }
}
