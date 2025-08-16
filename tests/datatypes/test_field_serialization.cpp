//
// Created by frostzt on 8/3/2025.
//

#include "catch2/catch_test_macros.hpp"
#include "tests/test_utils.hpp"

using Field = core::datatypes::Field;
using FieldType = core::datatypes::FieldType;
using UUID = core::datatypes::UUID;

TEST_CASE("should serialize and deserialize a uint64", "[DATATYPES]") {
    constexpr int64_t value64 = 1;
    const Field field64 = TESTS::makeField(value64);

    const auto serialized64 = field64.serialize();
    const auto deserialized64 = Field::deserialize(serialized64.data(), serialized64.size());

    REQUIRE(deserialized64.type_ == field64.type_);
    REQUIRE(deserialized64.value_ == field64.value_);

    constexpr int32_t value32 = 1;
    const Field field32 = TESTS::makeField(value32);

    const auto serialized32 = field32.serialize();
    const auto deserialized32 = Field::deserialize(serialized32.data(), serialized32.size());

    REQUIRE(deserialized32.type_ == field32.type_);
    REQUIRE(deserialized32.value_ == field32.value_);
};

TEST_CASE("should serialize and deserialize a std::string", "[DATATYPES]") {
    const std::string value = "hello world";
    const Field field = TESTS::makeField(value);

    REQUIRE(field.type_ == FieldType::String);

    const auto serialized = field.serialize();
    const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

    REQUIRE(deserialized.type_ == field.type_);
    REQUIRE(deserialized.value_ == field.value_);
};

TEST_CASE("should serialize and deserialize a UUID", "[DATATYPES]") {
    const UUID value{"5a5f1b38-d767-4b30-81ae-b6f4c0d1360d"};
    const Field field = TESTS::makeField(value);

    REQUIRE(field.type_ == FieldType::UUID);

    const auto serialized = field.serialize();
    const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

    REQUIRE(deserialized.type_ == field.type_);
    REQUIRE(deserialized.value_ == field.value_);
};

TEST_CASE("should serialize and deserialize a binary array", "[DATATYPES]") {
    const std::vector<uint8_t> value{0xde, 0xad, 0xbe, 0xef, 0x10, 0x20};
    const Field field = TESTS::makeField(value);

    REQUIRE(field.type_ == FieldType::Binary);

    const auto serialized = field.serialize();
    const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

    REQUIRE(deserialized.type_ == field.type_);
    REQUIRE(deserialized.value_ == field.value_);
};

TEST_CASE("should serialize and deserialize a timestamp", "[DATATYPES]") {
    const auto value = std::chrono::system_clock::now();
    const Field field = TESTS::makeField(value);

    REQUIRE(field.type_ == FieldType::Timestamp);

    const auto serialized = field.serialize();
    const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

    REQUIRE(deserialized.type_ == field.type_);
    REQUIRE(deserialized.value_ == field.value_);
};
