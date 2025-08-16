//
// Created by frostzt on 7/31/2025.
//

#include <unordered_map>

#include "catch2/catch_test_macros.hpp"
#include "lib/memtable/memtable.hpp"
#include "tests/test_utils.hpp"

TEST_CASE("should be able to put and get from memtable", "[MEMTABLE]") {
    const std::string tableName = "customers";
    const memtable::MemTable table{tableName};

    const auto primaryKey = core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227642")}};

    const core::Entry preEntry{tableName, primaryKey, {}, false};
    table.put(preEntry);

    const auto gotEntry = table.get(primaryKey);
    REQUIRE(gotEntry.has_value());
    REQUIRE(core::Entry::compareEntries(preEntry, gotEntry.value()));
};

TEST_CASE("should be able to put and get multiple entries from memtable", "[MEMTABLE]") {
    const std::string tableName = "customers";
    const memtable::MemTable table{tableName};

    const std::vector data{
        std::make_pair(core::Row{
                           {"name", TESTS::makeField("Sourav")}, {"age", TESTS::makeField(25)},
                           {"orders", TESTS::makeField(567)},
                           {"balance", TESTS::makeField(567.974)}
                       },
                       core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227641")}}),
        std::make_pair(core::Row{
                           {"name", TESTS::makeField("Gourav")}, {"age", TESTS::makeField(23)},
                           {"orders", TESTS::makeField(967)},
                           {"balance", TESTS::makeField(2.532)}
                       },
                       core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227642")}}),
        std::make_pair(core::Row{
                           {"name", TESTS::makeField("Sudheer")}, {"age", TESTS::makeField(25)},
                           {"orders", TESTS::makeField(2539)},
                           {"balance", TESTS::makeField(592.329)}
                       },
                       core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227643")}}),
        std::make_pair(core::Row{
                           {"name", TESTS::makeField("Sachin")}, {"age", TESTS::makeField(28)},
                           {"orders", TESTS::makeField(82)},
                           {"balance", TESTS::makeField(57098.56)}
                       },
                       core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227644")}}),
        std::make_pair(core::Row{
                           {"name", TESTS::makeField("Amit")}, {"age", TESTS::makeField(29)},
                           {"orders", TESTS::makeField(298)},
                           {"balance", TESTS::makeField(529.52)}
                       },
                       core::Key{{TESTS::makeField("8f1860aa-5d34-43b1-b70e-917fe5227645")}})
    };

    // Insert the vector data
    for (const auto &[rowData, primaryKey]: data) {
        table.put(core::Entry{tableName, primaryKey, rowData, false});
    }

    // Read and match the data
    for (const auto &[rowData, primaryKey]: data) {
        const auto getEntry = table.get(primaryKey);
        REQUIRE(getEntry.has_value());

        const auto toCompare = core::Entry{tableName, primaryKey, rowData, false};
        const auto entryFound = getEntry.value();
        REQUIRE(core::Entry::compareEntries(entryFound, toCompare, false));
    }
};
