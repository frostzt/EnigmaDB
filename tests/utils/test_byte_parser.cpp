//
// Created by frostzt on 7/18/2025.
//

#include "catch2/catch_test_macros.hpp"
#include "lib/entry/entry.hpp"
#include "tests/test_utils.hpp"

TEST_CASE("should return correct fields after encoding and decoding", "[BYTE_PARSER]") {
    const core::Entry entry("customers", core::Key{{TESTS::makeField("cid")}}, {
                                {"name", TESTS::makeField("Sourav")},
                                {"age", TESTS::makeField(52)},
                                {"balance", TESTS::makeField(999.999)},
                            }, false);

    const auto serializedEntry = entry.serialize();
    const auto deserialized = core::Entry::deserialize(serializedEntry.data(), serializedEntry.size());
    REQUIRE(deserialized);

    const auto &dEntry = deserialized.value();
    REQUIRE(core::Entry::compareEntries(entry, dEntry));
};

TEST_CASE("should return nullopt if the checksum is invalid", "[BYTE_PARSER]") {
    const core::Entry entry("customers", core::Key{{TESTS::makeField("cid")}}, {
                                {"name", TESTS::makeField("Sourav")},
                                {"age", TESTS::makeField(25)},
                                {"balance", TESTS::makeField(999.999)},
                            }, false);

    auto serializedEntry = entry.serialize();

    // Simulate corruption by flipping lsb
    serializedEntry.back() ^= static_cast<std::byte>(0x01);

    const auto deserialized = core::Entry::deserialize(serializedEntry.data(), serializedEntry.size());
    REQUIRE(!deserialized.has_value());
};
