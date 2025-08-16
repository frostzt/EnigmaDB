//
// Created by frostzt on 7/17/2025.
//

#include "catch2/catch_test_macros.hpp"
#include "lib/entry/entry.hpp"
#include "tests/test_utils.hpp"

TEST_CASE("should convert to bytes", "[BYTE_PARSER]") {
    const core::Entry entry("customer", core::Key{{TESTS::makeField("cid")}}, {}, false);
    const auto serialized = entry.serialize();
    REQUIRE(!serialized.empty());
    auto de = entry.toHex();
};
