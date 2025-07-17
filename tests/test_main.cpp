//
// Created by aiden on 7/16/2025.
//

#include <iostream>
#include <vector>
#include <memory>

#include "test_abs.hpp"
#include "timestamp_generator_test.hpp"
#include "utils/byte_utils.hpp"

int main() {
    std::vector<std::unique_ptr<TestCase>> tests;
    tests.emplace_back(std::make_unique<TimestampMonotonicTest>());
    tests.emplace_back(std::make_unique<TimestampConcurrencyTest>());
    tests.emplace_back(std::make_unique<TestConvertingToByes>());

    for (const auto& test : tests) {
        const bool result = test->execute();
        std::cout << "[ " << (result ? "PASS" : "FAIL") << " ] " << test->name() << '\n';
    }
}
