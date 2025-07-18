//
// Created by aiden on 7/18/2025.
//

#ifndef BYTE_PARSER_HPP
#define BYTE_PARSER_HPP

#include "tests/test_abs.hpp"

class TestFullRoundTripWAL final : public TestCase {
public:
    [[nodiscard]] std::string name() const override;

    [[nodiscard]] bool execute() const override;
};

class TestChecksumCorruption final : public TestCase {
public:
    [[nodiscard]] std::string name() const override;

    [[nodiscard]] bool execute() const override;
};


#endif //BYTE_PARSER_HPP
