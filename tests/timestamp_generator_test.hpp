//
// Created by aiden on 7/16/2025.
//

#ifndef TIMESTAMP_GENERATOR_TEST_HPP
#define TIMESTAMP_GENERATOR_TEST_HPP

#include <string>

#include "test_abs.hpp"

namespace TESTS::TIMESTAMP {
    class TimestampMonotonicTest final : public TestCase {
    public:
        [[nodiscard]] std::string suiteName() const override {
            return "[TIMESTAMP_GENERATOR]";
        }

        [[nodiscard]] std::string name() const override {
            return "should generate monotonically increasing timestamps";
        }

        [[nodiscard]] bool execute() const override;
    };

    class TimestampConcurrencyTest final : public TestCase {
    public:
        [[nodiscard]] std::string suiteName() const override {
            return "[TIMESTAMP_GENERATOR]";
        }

        [[nodiscard]] std::string name() const override {
            return "should generate timestamps with no collisions with concurrent calls";
        }

        [[nodiscard]] bool execute() const override;
    };
} // namespace TESTS::TIMESTAMP

#endif //TIMESTAMP_GENERATOR_TEST_HPP
