//
// Created by frostzt on 7/13/25.
//

#include <sstream>
#include <iomanip>

#include "entry.hpp"

#include "utils/byte_parser.hpp"
#include "utils/crypto_utils.hpp"
#include "utils/logger.hpp"

std::vector<std::byte> Entry::serialize() const {
    std::vector<std::byte> byteV{};

    // Write magic bytes
    Utility::ByteParser::writeMagicBytes(byteV);

    // Reserve 8 bytes for total size
    for (int i = 0; i < 8; i++) byteV.push_back(static_cast<std::byte>(0)); // placeholder

    Utility::ByteParser::writeString(byteV, this->tableName);
    Utility::ByteParser::writeString(byteV, this->primaryKey_);
    Utility::ByteParser::writeUint16(byteV, this->rowData_.size());

    for (const auto &[key, value]: this->rowData_) {
        Utility::ByteParser::writeString(byteV, key);
        Utility::ByteParser::writeVariant(byteV, value);
    }

    byteV.push_back(static_cast<std::byte>(this->isTombstone_ ? 1 : 0));
    Utility::ByteParser::writeUint64(byteV, this->timestamp_);

    // Patch the header to update the total size
    const uint64_t totalSize = byteV.size() + 4; // +4 for checksum
    Utility::ByteParser::patchUint64(byteV, 5, totalSize);

    // Compute CRC32
    const uint32_t checksum = Utility::computeCRC32(byteV, 0, byteV.size());
    Utility::ByteParser::writeUint32(byteV, checksum);

    return byteV;
}

std::optional<Entry> Entry::deserialize(const std::byte *data, const size_t length) {
    if (const auto isEntry = std::string(reinterpret_cast<const char *>(data), 5); isEntry != "ENTRY") {
        return std::nullopt;
    }

    Utility::ByteParser parser(data + 5, length - 5);
    const uint64_t totalSize = parser.readUint64();

    Entry entry{};

    const auto tableName = parser.readString();
    entry.tableName = tableName;

    const auto primaryKey = parser.readString();
    entry.primaryKey_ = primaryKey;

    const auto rowSize = parser.readUint16();

    for (size_t i = 0; i < static_cast<size_t>(rowSize); i++) {
        const std::string key = parser.readString();
        const auto variant = parser.readVariant();

        // Handle corrupt entry
        if (!variant.has_value()) {
            return std::nullopt;
        }

        entry.rowData_.emplace(key, *variant);
    }

    const auto isTombstone = static_cast<bool>(parser.readByte());
    entry.isTombstone_ = isTombstone;

    const auto timestamp = parser.readUint64();
    entry.timestamp_ = timestamp;

    const uint32_t expected = parser.readUint32();
    if (const uint32_t actual = Utility::computeCRC32(data, 0, totalSize - 4); expected != actual) {
        Utility::Logger::debug("mismatched checksum while entry deserialization: {} != {}", expected, actual);
        return std::nullopt;
    }

    return entry;
}

std::string Entry::toHex() const {
    const auto serialized = serialize();

    std::ostringstream oss;
    for (std::byte b: serialized) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b) << " ";
    }

    return oss.str();
}

