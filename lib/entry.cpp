//
// Created by frostzt on 7/13/25.
//

#include <sstream>
#include <iomanip>

#include "entry.hpp"
#include "utils/byte_utils.hpp"

std::vector<std::byte> Entry::serialize() const {
    std::vector<std::byte> byteV{};

    // Write magic bytes
    Utility::writeMagicBytes(byteV);

    // Reserve 8 bytes for total size
    for (int i = 0; i < 8; i++) byteV.push_back(static_cast<std::byte>(0)); // placeholder

    Utility::writeString(byteV, this->tableName);
    Utility::writeString(byteV, this->primaryKey_);
    Utility::writeUint16(byteV, this->rowData_.size());

    for (const auto &[key, value]: this->rowData_) {
        Utility::writeString(byteV, key);
        Utility::writeVariant(byteV, value);
    }

    byteV.push_back(static_cast<std::byte>(this->isTombstone_ ? 1 : 0));
    Utility::writeUint64(byteV, this->timestamp_);

    // Patch the header to update total size
    const uint64_t totalSize = byteV.size() + 4; // +4 for checksum
    Utility::patchUint64(byteV, 0, totalSize);

    // Compute CRC32
    const uint32_t checksum = Utility::computeCRC32(byteV, 0, byteV.size());
    Utility::writeUint32(byteV, checksum);

    return byteV;
}

std::string Entry::toHex() const {
    const auto serialized = serialize();

    std::ostringstream oss;
    for (std::byte b: serialized) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b) << " ";
    }

    return oss.str();
}

