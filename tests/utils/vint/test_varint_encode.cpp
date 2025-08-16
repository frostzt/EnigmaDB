//
// Created by frostzt on 8/15/2025.
//

#include <catch2/catch_test_macros.hpp>
#include "lib/utils/vint/vint.hpp"

TEST_CASE("should be able to encode int32", "[VARINT32]") {
    const auto values = std::vector<uint32_t>{
        0, 1, 10, 100, 127, 128, 1000, 10000, 16383, 16384, 100000, 1000000, 10000000, 100000000, 0xFFFFFFFF,
    };

    for (const auto v: values) {
        std::vector<uint8_t> encoded{};
        utility::putVarint32(encoded, v);

        REQUIRE(!encoded.empty());

        utility::StatusCode st;
        uint32_t num;
        const auto result = utility::getVarint32(st, encoded.data(), encoded.data() + encoded.size(), num);

        REQUIRE(result);
        REQUIRE(st == utility::StatusCode::OK);
        REQUIRE(num == v);
    }
};

TEST_CASE("should return correct status in truncated data int32", "[VARINT32]") {
    const auto truncatedData = std::vector<uint8_t>{0x80};

    utility::StatusCode truncSt;
    uint32_t truncOut;
    const auto resultTrunc = utility::getVarint32(truncSt, truncatedData.data(),
                                                  truncatedData.data() + truncatedData.size(),
                                                  truncOut);

    REQUIRE(truncSt == utility::StatusCode::TRUNCATED);
    REQUIRE(resultTrunc == nullptr);
};

TEST_CASE("should return correct status in malformed data int32", "[VARINT32]") {
    const auto malformedData = std::vector<uint8_t>{0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    utility::StatusCode malSt;
    uint32_t malOut;
    const auto resultTrunc = utility::getVarint32(malSt, malformedData.data(),
                                                  malformedData.data() + malformedData.size(),
                                                  malOut);

    REQUIRE(malSt == utility::StatusCode::MALFORMED);
    REQUIRE(resultTrunc == nullptr);
};

TEST_CASE("should be able to encode uint64", "[VARINT64]") {
    const auto values = std::vector<uint64_t>{
        0ull,
        1ull,
        10ull,
        127ull,
        128ull,
        255ull,
        256ull,
        1000ull,
        10000ull,
        (1ull << 14) - 1, // 2-byte upper bound
        (1ull << 14), // 2-byte lower bound
        (1ull << 21) - 1, // 3-byte upper bound
        (1ull << 21), // 3-byte lower bound
        (1ull << 28) - 1, // 4-byte upper bound
        (1ull << 35) - 1, // 5-byte
        (1ull << 42), // upper mid-range
        (1ull << 56) - 1, // near 56 bits
        (1ull << 56), // just over 56 bits
        0xFFFFFFFFull, // 32-bit max
        0xFFFFFFFFFFFFFFFFull, // 64-bit max
        9223372036854775807ull, // INT64_MAX
    };

    for (const auto v: values) {
        std::vector<uint8_t> encoded{};
        utility::putVarint64(encoded, v);
        REQUIRE(!encoded.empty());
        utility::StatusCode st;
        uint64_t num;
        const auto result = utility::getVarint64(st, encoded.data(), encoded.data() + encoded.size(), num);
        REQUIRE(result);
        REQUIRE(st == utility::StatusCode::OK);
        REQUIRE(num == v);
    }
};

TEST_CASE("should return correct status in truncated data int64", "[VARINT64]") {
    const auto truncatedData = std::vector<uint8_t>{
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    };
    utility::StatusCode truncSt;
    uint64_t truncOut;
    const auto resultTrunc = utility::getVarint64(truncSt, truncatedData.data(),
                                                  truncatedData.data() + truncatedData.size(),
                                                  truncOut);
    REQUIRE(truncSt == utility::StatusCode::TRUNCATED);
    REQUIRE(resultTrunc == nullptr);
};

TEST_CASE("should return correct status in malformed data int64", "[VARINT64]") {
    const auto malformedData = std::vector<uint8_t>{
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01
    };
    utility::StatusCode malSt;
    uint64_t malOut;
    const auto resultMal = utility::getVarint64(malSt, malformedData.data(),
                                                malformedData.data() + malformedData.size(),
                                                malOut);
    REQUIRE(malSt == utility::StatusCode::MALFORMED);
    REQUIRE(resultMal == nullptr);
};
