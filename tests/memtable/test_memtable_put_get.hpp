//
// Created by aiden on 7/31/2025.
//

#ifndef TEST_MEMTABLE_PUT_GET_HPP
#define TEST_MEMTABLE_PUT_GET_HPP

#include <unordered_map>

#include "lib/memtable/memtable.hpp"
#include "tests/test_abs.hpp"

class TestMemTablePutGet final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[MEMTABLE]";
    };

    [[nodiscard]] std::string name() const override {
        return "should be able to put and get from memtable";
    }

    [[nodiscard]] bool execute() const override {
        const std::string tableName = "customers";
        const memtable::MemTable table{tableName};

        const auto primaryKey = core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227642")}};

        const core::Entry preEntry{tableName, primaryKey, {}, false};
        table.put(preEntry);

        const auto gotEntry = table.get(primaryKey);
        if (!gotEntry.has_value()) return failTest("failed to get entry from memtable");
        if (!core::Entry::compareEntries(preEntry, gotEntry.value())) {
            return failTest("invalid entry returned from memtable");
        }

        return true;
    }
};

class TestMemTablePutGetWithMultipleEntries final : public TestCase {
    [[nodiscard]] std::string suiteName() const override {
        return "[MEMTABLE]";
    };

    [[nodiscard]] std::string name() const override {
        return "should be able to put and get multiple entries from memtable";
    }

    [[nodiscard]] bool execute() const override {
        const std::string tableName = "customers";
        const memtable::MemTable table{tableName};

        const std::vector data{
            std::make_pair(core::Row{{"name", "Sourav"}, {"age", 25}, {"orders", 567}, {"balance", 567.974}},
                           core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227641")}}),
            std::make_pair(core::Row{{"name", "Gourav"}, {"age", 23}, {"orders", 967}, {"balance", 2.532}},
                           core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227642")}}),
            std::make_pair(core::Row{{"name", "Sudheer"}, {"age", 25}, {"orders", 2539}, {"balance", 592.329}},
                           core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227643")}}),
            std::make_pair(core::Row{{"name", "Sachin"}, {"age", 28}, {"orders", 82}, {"balance", 57098.56}},
                           core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227644")}}),
            std::make_pair(core::Row{{"name", "Amit"}, {"age", 29}, {"orders", 298}, {"balance", 529.52}},
                           core::Key{{std::string("8f1860aa-5d34-43b1-b70e-917fe5227645")}})
        };

        // Insert the vector data
        for (const auto &[rowData, primaryKey]: data) {
            table.put(core::Entry{tableName, primaryKey, rowData, false});
        }

        // Read and match the data
        for (const auto &[rowData, primaryKey]: data) {
            const auto getEntry = table.get(primaryKey);
            if (!getEntry.has_value()) {
                return failTest("failed to get entry from memtable");
            }

            const auto toCompare = core::Entry{tableName, primaryKey, rowData, false};
            const auto entryFound = getEntry.value();
            if (!core::Entry::compareEntries(entryFound, toCompare, false)) {
                return failTest("invalid entry returned from memtable");
            }
        }

        return true;
    }
};

#endif //TEST_MEMTABLE_PUT_GET_HPP
