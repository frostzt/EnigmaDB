//
// Created by frostzt on 8/3/2025.
//

#include "catch2/catch_test_macros.hpp"
#include "lib/datatypes/types/uuid.hpp"

using UUID = core::datatypes::UUID;

TEST_CASE("should create a valid uuid from string", "[DATATYPES]") {
    const std::string uuid = "8f1860aa-5d34-43b1-b70e-917fe5227642";
    const UUID uuid4{uuid};
    REQUIRE(uuid4.toString() == uuid);
};

TEST_CASE("should create a valid uuid from byte array", "[DATATYPES]") {
    const std::array<uint8_t, 16> arr = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
    };

    const UUID uuid(arr);
    REQUIRE(uuid.toString() == "12345678-9abc-def0-0123-456789abcdef");

    const UUID uuid2("12345678-9abc-def0-0123-456789abcdef");
    REQUIRE(uuid == uuid2);
};

TEST_CASE("should compare two uuids", "[DATATYPES]") {
    const std::string uuid1 = "8f1860aa-5d34-43b1-b70e-917fe5227642";
    const std::string uuid2 = "8f1860aa-5d34-43b1-b70e-917fe5227643";

    const UUID uuid1Obj{uuid1};
    const UUID uuid2Obj{uuid2};

    // Equality test
    REQUIRE(uuid1Obj != uuid2Obj);

    // Comparison test
    REQUIRE(uuid1Obj < uuid2Obj);
};
