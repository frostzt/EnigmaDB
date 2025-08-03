//
// Created by aiden on 7/26/2025.
//

#ifndef WAL_MANAGER_TEST_HPP
#define WAL_MANAGER_TEST_HPP

#include "tests/test_abs.hpp"

namespace TESTS::WAL {
    class TestWALConcurrentAppendsWithRotation final : public TestCase {
    public:
        [[nodiscard]] std::string suiteName() const override {
            return "[WAL]";
        };

        [[nodiscard]] std::string name() const override {
            return "should handle concurrent appends with rotation";
        }

        [[nodiscard]] bool execute() const override;
    };
} // namespace TESTS::WAL

#endif //WAL_MANAGER_TEST_HPP
