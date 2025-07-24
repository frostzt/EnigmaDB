//
// Created by aiden on 7/18/2025.
//

#ifndef BYTE_PARSER_HPP
#define BYTE_PARSER_HPP

#include "tests/test_abs.hpp"

class TestFullRoundTripWAL final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[BYTE_PARSER]";
    };

    [[nodiscard]] std::string name() const override {
        return "should return correct fields after encoding and decoding";
    }

    [[nodiscard]] bool execute() const override;
};

class TestChecksumCorruption final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[BYTE_PARSER]";
    };

    [[nodiscard]] std::string name() const override {
        return "should return nullopt if the checksum is invalid";
    }

    [[nodiscard]] bool execute() const override;
};


#endif //BYTE_PARSER_HPP
