//
// Created by aiden on 7/17/2025.
//

#ifndef BYTE_UTILS_HPP
#define BYTE_UTILS_HPP

#include "tests/test_abs.hpp"

namespace TESTS::UTILS {
    class TestConvertingToByes final : public TestCase {
    public:
        [[nodiscard]] std::string suiteName() const override {
            return "[BYTE_PARSER]";
        };

        [[nodiscard]] std::string name() const override {
            return "should convert to bytes";
        }

        [[nodiscard]] bool execute() const override;
    };
} // namespace TESTS::UTILS

#endif //BYTE_UTILS_HPP
