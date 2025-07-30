//
// Created by aiden on 7/24/2025.
//

#ifndef WAL_CODEC_TEST_HPP
#define WAL_CODEC_TEST_HPP

#include "tests/test_abs.hpp"

class TestFullWALCodecTrip final : public TestCase {
public:
    [[nodiscard]] std::string suiteName() const override {
        return "[WAL]";
    };

    [[nodiscard]] std::string name() const override {
        return "should encode and decode a full wal file";
    }

    [[nodiscard]] bool execute() const override;
};

class TestWALMultipleEntries final : public TestCase {
    [[nodiscard]] std::string suiteName() const override {
        return "[WAL]";
    };

    [[nodiscard]] std::string name() const override {
        return "should encode and decode a full wal file";
    }

    [[nodiscard]] bool execute() const override;
};


#endif //WAL_CODEC_TEST_HPP
