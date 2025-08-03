//
// Created by frostzt on 8/3/2025.
//

#pragma once
#ifndef TEST_FIELD_SERIALIZATION_HPP
#define TEST_FIELD_SERIALIZATION_HPP

#include "tests/test_abs.hpp"
#include "tests/test_utils.hpp"

using Field = core::datatypes::Field;
using FieldType = core::datatypes::FieldType;
using UUID = core::datatypes::UUID;

namespace TESTS::DATATYPES {
    class TestUINTSerialization final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[DATATYPES]";
        }

        [[nodiscard]] std::string name() const override {
            return "should serialize and deserialize a uint64";
        }

        [[nodiscard]] bool execute() const override {
            constexpr int64_t value64 = 1;
            const Field field64 = makeField(value64);

            const auto serialized64 = field64.serialize();
            const auto deserialized64 = Field::deserialize(serialized64.data(), serialized64.size());

            if (deserialized64.type_ != field64.type_) {
                return failTest("[64bit] deserialized field type mismatch");
            }

            if (deserialized64.value_ != field64.value_) {
                return failTest("[64bit] deserialized field value mismatch");
            }

            constexpr int32_t value32 = 1;
            const Field field32 = makeField(value32);

            const auto serialized32 = field32.serialize();
            const auto deserialized32 = Field::deserialize(serialized32.data(), serialized32.size());

            if (deserialized32.type_ != field32.type_) {
                return failTest("[32bit] deserialized field type mismatch");
            }

            if (deserialized32.value_ != field32.value_) {
                return failTest("[32bit] deserialized field value mismatch");
            }

            return true;
        };
    };

    class TestStringSerialization final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[DATATYPES]";
        }

        [[nodiscard]] std::string name() const override {
            return "should serialize and deserialize a std::string";
        }

        [[nodiscard]] bool execute() const override {
            const std::string value = "hello world";
            const Field field = makeField(value);

            if (field.type_ != FieldType::String) {
                return failTest("constructed field type mismatch");
            }

            const auto serialized = field.serialize();
            const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

            if (deserialized.type_ != field.type_) {
                return failTest("deserialized field type mismatch");
            }

            if (deserialized.value_ != field.value_) {
                return failTest("deserialized field value mismatch");
            }

            return true;
        };
    };

    class TestUUIDSerialization final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[DATATYPES]";
        }

        [[nodiscard]] std::string name() const override {
            return "should serialize and deserialize a UUID";
        }

        [[nodiscard]] bool execute() const override {
            const UUID value{"5a5f1b38-d767-4b30-81ae-b6f4c0d1360d"};
            const Field field = makeField(value);

            if (field.type_ != FieldType::UUID) {
                return failTest("constructed field type mismatch");
            }

            const auto serialized = field.serialize();
            const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

            if (deserialized.type_ != field.type_) {
                return failTest("deserialized field type mismatch");
            }

            if (deserialized.value_ != field.value_) {
                return failTest("deserialized field value mismatch");
            }

            return true;
        };
    };

    class TestBinaryArraySerialization final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[DATATYPES]";
        }

        [[nodiscard]] std::string name() const override {
            return "should serialize and deserialize a binary array";
        }

        [[nodiscard]] bool execute() const override {
            const std::vector<uint8_t> value{0xde, 0xad, 0xbe, 0xef, 0x10, 0x20};
            const Field field = makeField(value);

            if (field.type_ != FieldType::Binary) {
                return failTest("constructed field type mismatch");
            }

            const auto serialized = field.serialize();
            const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

            if (deserialized.type_ != field.type_) {
                return failTest("deserialized field type mismatch");
            }

            // Assuming Field::value_ can be safely casted or compared to the vector
            if (deserialized.value_ != field.value_) {
                return failTest("deserialized field value mismatch");
            }

            return true;
        };
    };

    class TestTimestampSerialization final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[DATATYPES]";
        }

        [[nodiscard]] std::string name() const override {
            return "should serialize and deserialize a binary array";
        }

        [[nodiscard]] bool execute() const override {
            const auto value = std::chrono::system_clock::now();
            const Field field = makeField(value);

            if (field.type_ != FieldType::Timestamp) {
                return failTest("constructed field type mismatch");
            }

            const auto serialized = field.serialize();
            const auto deserialized = Field::deserialize(serialized.data(), serialized.size());

            if (deserialized.type_ != field.type_) {
                return failTest("deserialized field type mismatch");
            }

            if (deserialized.value_ != field.value_) {
                return failTest("deserialized field value mismatch");
            }

            return true;
        };
    };
} // namespace TESTS::DATATYPES

#endif //TEST_FIELD_SERIALIZATION_HPP
