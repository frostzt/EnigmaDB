//
// Created by aiden on 7/30/2025.
//

#ifndef TEST_WRITER_BEHAVIOR_HPP
#define TEST_WRITER_BEHAVIOR_HPP

#include <filesystem>

#include "lib/wal/wal_manager.hpp"
#include "lib/utils/constants.hpp"
#include "tests/test_abs.hpp"

class TestAppendForSingleWriter final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[WAL]";
    };

    [[nodiscard]] std::string name() const override {
        return "should be able to append in to the file with single writer";
    }

    [[nodiscard]] bool execute() const override {
        std::string path = "wal";
        std::filesystem::create_directory(path);

        const WAL::WALManager manager(1, 4_KB, path);

        // Create a new wal file by adding an entry
        const Entry sourav("customer", "cid", {{"name", "sourav"}}, false);
        if (const auto appended = manager.append(sourav); !appended) return false;

        // Check if the new file was made
        for (const auto &file: std::filesystem::directory_iterator(path)) {
            if (file.is_regular_file()) {
                if (const auto fileName = file.path().filename().string(); fileName != "w_1_00000001.wal") {
                    std::cerr << "File name mismatched: " << fileName << std::endl;
                    return false;
                };
            }
        }

        // Read the first entries and validate them
        const auto initialEntries = manager.loadAll();
        if (initialEntries.size() != 1) return false;
        if (!Entry::compareEntries(initialEntries[0], sourav)) return false;

        // Add another entry
        const Entry gourav("customer", "cid", {{"name", "gourav"}}, false);
        if (const auto appended = manager.append(gourav); !appended) return false;

        // Check if the append, appended the entry
        const auto finalEntries = manager.loadAll();
        if (finalEntries.size() != 2) return false;
        if (!Entry::compareEntries(finalEntries[0], sourav)) return false;
        if (!Entry::compareEntries(finalEntries[1], gourav)) return false;

        manager.close();
        std::filesystem::remove_all(path);
        return true;
    }
};

class TestWriterFlushToDisk final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[WAL]";
    };

    [[nodiscard]] std::string name() const override {
        return "writer should flush data to disk";
    }

    [[nodiscard]] bool execute() const override {
        const std::string path = "wal";
        std::filesystem::create_directory(path);

        // Initialize the writer
        WAL::WALWriter writer(1, path, 4_KB);
        if (!writer.isStreamGood()) return false;

        // Write an entry to the buffer
        const Entry sourav("customer", "cid", {{"name", "sourav"}}, false);
        writer.append(sourav, WAL::FlushMode::NO_FLUSH); // EXPLICITLY DENYING fsync

        // Validate if the file exists
        for (const auto &file: std::filesystem::directory_iterator(path)) {
            if (file.is_regular_file()) {
                if (const auto fileName = file.path().filename().string(); fileName != "w_1_00000001.wal") {
                    std::cerr << "File name mismatched: " << fileName << std::endl;
                    return false;
                };
            }
        }

        std::ifstream fileIn(path + "/w_1_00000001.wal", std::ios::in | std::ios::binary);
        if (!fileIn.good()) return false;

        // Try and read the entry should return null
        if (const auto possibleEntry = WAL::readRecord(fileIn); possibleEntry.has_value()) return false;
        fileIn.close();

        // Flush the buffer into the disk
        writer.flush();

        // Try and read the entry should return null
        fileIn.open(path + "/w_1_00000001.wal", std::ios::in | std::ios::binary);
        if (const auto possibleEntry = WAL::readRecord(fileIn); !possibleEntry.has_value()) return false;

        writer.close();
        fileIn.close();
        std::filesystem::remove_all(path);
        return true;
    }
};

#endif //TEST_WRITER_BEHAVIOR_HPP
