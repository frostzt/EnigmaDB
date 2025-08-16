//
// Created by frostzt on 8/15/2025.
//

#include <catch2/catch_test_macros.hpp>
#include "lib/utils/vint/vint.hpp"

TEST_CASE("should be able to encode int32", "[VARINT]") {
    const auto values = std::vector<uint32_t>{
        0, 1, 10, 100, 127, 128, 1000, 10000, 16383, 16384, 100000, 1000000, 10000000, 100000000,
    };

    for (const auto v: values) {
        std::vector<uint8_t> encoded{};
        utility::putVarint32(encoded, v);

        REQUIRE(encoded.size() > 0);

        utility::StatusCode st;
        uint32_t num;
        const auto result = utility::getVarint32(st, encoded.data(), encoded.data() + encoded.size(), num);

        REQUIRE(result);
        REQUIRE(st == utility::StatusCode::OK);
        REQUIRE(num == v);
    }
};
