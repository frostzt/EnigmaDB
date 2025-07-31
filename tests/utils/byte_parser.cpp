//
// Created by aiden on 7/18/2025.
//


#include <iomanip>

#include "byte_parser.hpp"
#include "lib/entry/entry.hpp"
#include "lib/wal/wal_codec.hpp"

bool TestFullRoundTripWAL::execute() const {
    const core::Entry entry("customers", core::Key{{std::string("cid")}}, {
                          {"name", std::string("Sourav")},
                          {"age", 25},
                          {"balance", 999.999},
                      }, false);

    const auto serializedEntry = entry.serialize();
    const auto deserialized = core::Entry::deserialize(serializedEntry.data(), serializedEntry.size());
    if (!deserialized) {
        std::cerr << "Deserialization failed" << std::endl;
        return false;
    }

    const auto &dEntry = deserialized.value();
    if (dEntry.tableName != entry.tableName) {
        std::cerr << "Table name mismatch found!" << std::endl;
        return false;
    }

    if (dEntry.primaryKey_ != entry.primaryKey_) {
        std::cerr << "Primary key mismatch found!" << std::endl;
        return false;
    }

    if (dEntry.rowData_.size() != entry.rowData_.size()) {
        std::cerr << "Row data size mismatch!" << std::endl;
        return false;
    }

    if (dEntry.rowData_ != entry.rowData_) {
        std::cerr << "Row data mismatch!" << std::endl;
        return false;
    }

    if (dEntry.isTombstone_ != entry.isTombstone_) {
        std::cerr << "Tombstone mismatch!" << std::endl;
        return false;
    }

    if (dEntry.timestamp_ != entry.timestamp_) {
        std::cerr << "Timestamp mismatch!" << std::endl;
        return false;
    }

    return true;
}

bool TestChecksumCorruption::execute() const {
    const core::Entry entry("customers", core::Key{{std::string("cid")}}, {
                          {"name", std::string("Sourav")},
                          {"age", 25},
                          {"balance", 999.999},
                      }, false);

    auto serializedEntry = entry.serialize();

    // Simulate corruption by flipping lsb
    serializedEntry.back() ^= static_cast<std::byte>(0x01);

    const auto deserialized = core::Entry::deserialize(serializedEntry.data(), serializedEntry.size());
    return !deserialized.has_value(); // nullopt should be returned if checksum is invalid
}
