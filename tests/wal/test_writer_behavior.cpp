//
// Created by frostzt on 7/30/2025.
//

#include <filesystem>

#include "catch2/catch_test_macros.hpp"
#include "lib/wal/wal_manager.hpp"
#include "lib/utils/constants.hpp"
#include "tests/test_utils.hpp"

TEST_CASE("should be able to append in to the file with single writer", "[WAL]") {
    std::string path = "wal";
    std::filesystem::create_directory(path);

    const ::WAL::WALManager manager(1, 4_KB, path);

    // Create a new wal file by adding an entry
    const core::Entry sourav("customer", core::Key{{TESTS::makeField("cid")}}, {{"name", TESTS::makeField("sourav")}},
                             false);
    const auto appended = manager.append(sourav);
    REQUIRE(appended);

    // Check if the new file was made
    for (const auto &file: std::filesystem::directory_iterator(path)) {
        if (file.is_regular_file()) {
            if (const auto fileName = file.path().filename().string(); fileName != "w_1_00000001.wal") {
                std::cerr << "File name mismatched: " << fileName << std::endl;
                FAIL("File name mismatched");
            };
        }
    }

    // Read the first entries and validate them
    const auto initialEntries = manager.loadAll();
    REQUIRE(initialEntries.size() == 1);

    REQUIRE(core::Entry::compareEntries(initialEntries[0], sourav));

    // Add another entry
    const core::Entry gourav("customer", core::Key{{TESTS::makeField("cid")}}, {{"name", TESTS::makeField("gourav")}},
                             false);
    const auto appended2 = manager.append(gourav);
    REQUIRE(appended2);

    // Check if the append call appended the entry
    const auto finalEntries = manager.loadAll();
    REQUIRE(finalEntries.size() == 2);
    REQUIRE(core::Entry::compareEntries(finalEntries[0], sourav));
    REQUIRE(core::Entry::compareEntries(finalEntries[1], gourav));

    manager.close();
    std::filesystem::remove_all(path);
};

TEST_CASE("writer should flush data to disk", "[WAL]") {
    const std::string path = "wal";
    std::filesystem::create_directory(path);

    // Initialize the writer
    ::WAL::WALWriter writer(1, path, 4_KB);
    REQUIRE(writer.isStreamGood());

    // Write an entry to the buffer
    const core::Entry sourav("customer", core::Key{{TESTS::makeField("cid")}}, {{"name", TESTS::makeField("sourav")}},
                             false);
    writer.append(sourav, ::WAL::FlushMode::NO_FLUSH); // EXPLICITLY DENYING fsync

    // Validate if the file exists
    for (const auto &file: std::filesystem::directory_iterator(path)) {
        if (file.is_regular_file()) {
            if (const auto fileName = file.path().filename().string(); fileName != "w_1_00000001.wal") {
                std::cerr << "File name mismatched: " << fileName << std::endl;
                FAIL("File name mismatched");
            };
        }
    }

    std::ifstream fileIn(path + "/w_1_00000001.wal", std::ios::in | std::ios::binary);
    REQUIRE(fileIn.good());

    // Try and read the entry should return null
    const auto possibleEntry = ::WAL::readRecord(fileIn);
    REQUIRE(!possibleEntry.has_value());
    fileIn.close();

    // Flush the buffer into the disk
    writer.flush();

    // Try and read the entry should return null
    fileIn.open(path + "/w_1_00000001.wal", std::ios::in | std::ios::binary);
    const auto possibleEntry2 = ::WAL::readRecord(fileIn);
    REQUIRE(possibleEntry2.has_value());

    writer.close();
    fileIn.close();
    std::filesystem::remove_all(path);
};
