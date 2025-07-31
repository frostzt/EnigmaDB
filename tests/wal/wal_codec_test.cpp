//
// Created by aiden on 7/24/2025.
//

#include "wal_codec_test.hpp"
#include "lib/wal/wal_codec.hpp"

#include <filesystem>

#include "lib/entry/entry.hpp"

bool TestFullWALCodecTrip::execute() const {
    const core::Entry entry("customers", core::Key{{std::string("cid")}}, {
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
    WAL::writeRecord(out, entry, WAL::FlushMode::FORCE_FLUSH);
    out.close();

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

    in.close();
    std::filesystem::remove(fileName);

    return true;
}

bool TestWALMultipleEntries::execute() const {
    const core::Entry sourav("customers", core::Key{{std::string("cid")}}, {
                          {"name", std::string("Sourav")},
                          {"age", 25},
                          {"balance", 1000},
                      }, false);

    const core::Entry sudheer("customers", core::Key{{std::string("cid")}}, {
                          {"name", std::string("Sudheer")},
                          {"age", 25},
                          {"balance", 2000},
                      }, false);

    const core::Entry sachin("customers", core::Key{{std::string("cid")}}, {
                          {"name", std::string("Sachin")},
                          {"age", 27},
                          {"balance", 5000.500},
                      }, false);

    std::string fileName = "00001_test.wal";
    std::ofstream out(fileName, std::ios_base::out | std::ios_base::binary);
    if (!out.good()) {
        std::cerr << "Failed to open file for writing, or the file was not created!" << std::endl;
        return false;
    }

    // Record all the entries in WAL
    WAL::writeRecord(out, sourav);
    WAL::writeRecord(out, sudheer);
    WAL::writeRecord(out, sachin, WAL::FlushMode::FORCE_FLUSH);
    out.close();

    // Read the entries
    std::ifstream in(fileName, std::ios_base::in | std::ios_base::binary);
    if (!in.good()) {
        std::cerr << "Failed to open file for reading!" << std::endl;
        return false;
    }

    std::vector<core::Entry> entries;
    while (true) {
        const auto readEntry = WAL::readRecord(in);
        if (!readEntry.has_value()) break;
        entries.push_back(readEntry.value());
    }

    int result = false;
    result = core::Entry::compareEntries(sourav, entries[0]);
    if (!result) return false;

    result = core::Entry::compareEntries(sudheer, entries[1]);
    if (!result) return false;

    result = core::Entry::compareEntries(sachin, entries[2]);

    in.close();
    std::filesystem::remove(fileName);
    return result;
}
