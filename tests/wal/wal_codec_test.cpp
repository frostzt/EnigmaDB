//
// Created by aiden on 7/24/2025.
//

#include "wal_codec_test.hpp"
#include "lib/wal/wal_codec.hpp"

#include "lib/entry/entry.hpp"

bool TestFullWALCodecTrip::execute() const {
    const Entry entry("customers", "cid", {
                          {"name", std::string("Sourav")},
                          {"age", 25},
                          {"balance", 999.999},
                      }, false);

    std::string fileName = "00001_test.wal";
    std::ofstream out(fileName, std::ios_base::out | std::ios_base::binary);
    if (!out.good()) {
        std::cerr << "Failed to open file for writing, or the file was not created!" << std::endl;
        return false;
    }

    // Write the entry to the WAL file
    WAL::writeRecord(out, entry);

    std::ifstream in(fileName, std::ios_base::in | std::ios_base::binary);
    if (!in.good()) {
        std::cerr << "Failed to open file for reading!" << std::endl;
        return false;
    }

    const auto readEntry = WAL::readRecord(in);
    if (!readEntry.has_value()) {
        std::cerr << "Failed to read the entry from the WAL file!" << std::endl;
        return false;
    }

    const auto &dEntry = readEntry.value();

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
