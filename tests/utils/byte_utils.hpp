//
// Created by aiden on 7/17/2025.
//

#ifndef BYTE_UTILS_HPP
#define BYTE_UTILS_HPP

#include "tests/test_abs.hpp"

class TestConvertingToByes final : public TestCase {
public:
    [[nodiscard]] std::string name() const override;

    [[nodiscard]] bool execute() const override;
};

#endif //BYTE_UTILS_HPP
