//
// Created by aiden on 7/16/2025.
//

#include <iostream>
#include <vector>
#include <memory>

#include "test_abs.hpp"
#include "timestamp_generator_test.hpp"
#include "spdlog/spdlog.h"
#include "utils/byte_parser.hpp"
#include "utils/byte_utils.hpp"
#include "wal/wal_codec_test.hpp"

int main() {
    std::vector<std::unique_ptr<TestCase> > tests;

    // Timestamp generator
    tests.emplace_back(std::make_unique<TimestampMonotonicTest>());
    tests.emplace_back(std::make_unique<TimestampConcurrencyTest>());

    // Byte parser
    tests.emplace_back(std::make_unique<TestConvertingToByes>());
    tests.emplace_back(std::make_unique<TestFullRoundTripWAL>());
    tests.emplace_back(std::make_unique<TestChecksumCorruption>());

    // WAL
    tests.emplace_back(std::make_unique<TestFullWALCodecTrip>());

    std::unordered_map<std::string, std::vector<TestCase *> > grouped;
    std::vector<std::unique_ptr<TestCase> > testInstances;

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
