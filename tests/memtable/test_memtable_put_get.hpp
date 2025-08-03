//
// Created by aiden on 7/31/2025.
//

#ifndef TEST_MEMTABLE_PUT_GET_HPP
#define TEST_MEMTABLE_PUT_GET_HPP

#include <unordered_map>

#include "lib/memtable/memtable.hpp"
#include "tests/test_abs.hpp"
#include "tests/test_utils.hpp"

namespace TESTS::MEMTABLE {
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

            const auto primaryKey = core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227642")}};

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
                std::make_pair(core::Row{
                                   {"name", makeField("Sourav")}, {"age", makeField(25)}, {"orders", makeField(567)},
                                   {"balance", makeField(567.974)}
                               },
                               core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227641")}}),
                std::make_pair(core::Row{
                                   {"name", makeField("Gourav")}, {"age", makeField(23)}, {"orders", makeField(967)},
                                   {"balance", makeField(2.532)}
                               },
                               core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227642")}}),
                std::make_pair(core::Row{
                                   {"name", makeField("Sudheer")}, {"age", makeField(25)}, {"orders", makeField(2539)},
                                   {"balance", makeField(592.329)}
                               },
                               core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227643")}}),
                std::make_pair(core::Row{
                                   {"name", makeField("Sachin")}, {"age", makeField(28)}, {"orders", makeField(82)},
                                   {"balance", makeField(57098.56)}
                               },
                               core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227644")}}),
                std::make_pair(core::Row{
                                   {"name", makeField("Amit")}, {"age", makeField(29)}, {"orders", makeField(298)},
                                   {"balance", makeField(529.52)}
                               },
                               core::Key{{makeField("8f1860aa-5d34-43b1-b70e-917fe5227645")}})
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
} // namespace TESTS::MEMTABLE

#endif //TEST_MEMTABLE_PUT_GET_HPP
