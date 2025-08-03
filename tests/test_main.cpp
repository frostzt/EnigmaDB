//
// Created by aiden on 7/16/2025.
//

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "test_abs.hpp"
#include "timestamp_generator_test.hpp"
#include "datatypes/types/test_uuid_type.hpp"
#include "memtable/test_memtable_put_get.hpp"
#include "utils/byte_parser.hpp"
#include "utils/byte_utils.hpp"
#include "wal/test_writer_behavior.hpp"
#include "wal/wal_codec_test.hpp"
#include "wal/wal_manager_test.hpp"

int main() {
    std::vector<std::unique_ptr<TESTS::TestCase> > tests;

    // Timestamp generator
    tests.emplace_back(std::make_unique<TESTS::TIMESTAMP::TimestampMonotonicTest>());
    tests.emplace_back(std::make_unique<TESTS::TIMESTAMP::TimestampConcurrencyTest>());

    // Byte parser
    tests.emplace_back(std::make_unique<TESTS::UTILS::TestConvertingToByes>());
    tests.emplace_back(std::make_unique<TESTS::UTILS::TestFullRoundTripWAL>());
    tests.emplace_back(std::make_unique<TESTS::UTILS::TestChecksumCorruption>());

    // WAL
    tests.emplace_back(std::make_unique<TESTS::WAL::TestFullWALCodecTrip>());
    tests.emplace_back(std::make_unique<TESTS::WAL::TestWALMultipleEntries>());
    tests.emplace_back(std::make_unique<TESTS::WAL::TestAppendForSingleWriter>());
    tests.emplace_back(std::make_unique<TESTS::WAL::TestWriterFlushToDisk>());
    tests.emplace_back(std::make_unique<TESTS::WAL::TestWALConcurrentAppendsWithRotation>());

    // MemTable
    tests.emplace_back(std::make_unique<TESTS::MEMTABLE::TestMemTablePutGet>());
    tests.emplace_back(std::make_unique<TESTS::MEMTABLE::TestMemTablePutGetWithMultipleEntries>());

    // DataTypes
    tests.emplace_back(std::make_unique<TESTS::DATATYPES::TestUUIDComparison>());
    tests.emplace_back(std::make_unique<TESTS::DATATYPES::TestUUIDCreationFromString>());
    tests.emplace_back(std::make_unique<TESTS::DATATYPES::TestUUIDCreationFromByteArray>());

    std::unordered_map<std::string, std::vector<TESTS::TestCase *> > grouped;
    std::vector<std::unique_ptr<TESTS::TestCase> > testInstances;

    for (auto &test: tests) {
        grouped[test->suiteName()].push_back(test.get());
        testInstances.push_back(std::move(test));
    }

    int totalTests = 0;
    int passedTests = 0;

    for (const auto &[suite, testList]: grouped) {
        std::cout << ":: " << suite << "\n";
        for (const auto *test: testList) {
            const bool result = test->execute();
            totalTests++;
            if (result) passedTests++;
            std::cout << "    " << (result ? "[ PASS ]" : "[ FAIL ]") << " " << test->name() << '\n';
        }
        std::cout << '\n';
    }

    std::cout << "PASSED " << passedTests << " / " << totalTests << " tests passed." << '\n';

    return passedTests == totalTests ? 0 : 1;
}
