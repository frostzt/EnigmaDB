//
// Created by frostzt on 7/24/2025.
//

#include <filesystem>

#include "catch2/catch_test_macros.hpp"
#include "lib/wal/wal_codec.hpp"
#include "lib/entry/entry.hpp"
#include "tests/test_utils.hpp"

TEST_CASE("should encode and decode a full WAL file", "[WAL]") {
    const core::Entry entry("customers", core::Key{{TESTS::makeField("cid")}}, {
                                {"name", TESTS::makeField("Sourav")},
                                {"age", TESTS::makeField(25)},
                                {"balance", TESTS::makeField(999.999)},
                            }, false);

    std::string fileName = "00001_test.wal";
    std::ofstream out(fileName, std::ios_base::out | std::ios_base::binary);
    REQUIRE(out.good());

    // Write the entry to the WAL file
    ::WAL::writeRecord(out, entry, ::WAL::FlushMode::FORCE_FLUSH);
    out.close();

    std::ifstream in(fileName, std::ios_base::in | std::ios_base::binary);
    REQUIRE(in.good());

    const auto readEntry = ::WAL::readRecord(in);
    REQUIRE(readEntry.has_value());

    const auto &dEntry = readEntry.value();

    REQUIRE(core::Entry::compareEntries(entry, dEntry));
    in.close();
    std::filesystem::remove(fileName);
};

TEST_CASE("should encode and decode a full WAL file with multiple entries", "[WAL]") {
    const core::Entry sourav("customers", core::Key{{TESTS::makeField("cid")}}, {
                                 {"name", TESTS::makeField("Sourav")},
                                 {"age", TESTS::makeField(25)},
                                 {"balance", TESTS::makeField(1000)},
                             }, false);

    const core::Entry sudheer("customers", core::Key{{TESTS::makeField("cid")}}, {
                                  {"name", TESTS::makeField("Sudheer")},
                                  {"age", TESTS::makeField(25)},
                                  {"balance", TESTS::makeField(2000)},
                              }, false);

    const core::Entry sachin("customers", core::Key{{TESTS::makeField("cid")}}, {
                                 {"name", TESTS::makeField("Sachin")},
                                 {"age", TESTS::makeField(27)},
                                 {"balance", TESTS::makeField(5000.500)},
                             }, false);

    std::string fileName = "00001_test.wal";
    std::ofstream out(fileName, std::ios_base::out | std::ios_base::binary);
    REQUIRE(out.good());

    // Record all the entries in WAL
    ::WAL::writeRecord(out, sourav);
    ::WAL::writeRecord(out, sudheer);
    ::WAL::writeRecord(out, sachin, ::WAL::FlushMode::FORCE_FLUSH);
    out.close();

    // Read the entries
    std::ifstream in(fileName, std::ios_base::in | std::ios_base::binary);
    REQUIRE(in.good());

    std::vector<core::Entry> entries;
    while (true) {
        const auto readEntry = ::WAL::readRecord(in);
        if (!readEntry.has_value()) break;
        entries.push_back(readEntry.value());
    }

    REQUIRE(core::Entry::compareEntries(sourav, entries[0]));
    REQUIRE(core::Entry::compareEntries(sudheer, entries[1]));
    REQUIRE(core::Entry::compareEntries(sachin, entries[2]));

    in.close();
    std::filesystem::remove(fileName);
};
