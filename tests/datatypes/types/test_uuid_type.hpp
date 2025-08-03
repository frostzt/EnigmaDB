//
// Created by frostzt on 8/3/2025.
//

#pragma once
#ifndef TEST_UUID_TYPE_HPP
#define TEST_UUID_TYPE_HPP

#include "lib/datatypes/types/uuid.hpp"
#include "tests/test_abs.hpp"

namespace TESTS::DATATYPES {
    using UUID = core::datatypes::UUID;

    class TestUUIDCreationFromString final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[UUID]";
        }

        [[nodiscard]] std::string name() const override {
            return "should create a valid uuid from string";
        }

        [[nodiscard]] bool execute() const override {
            const std::string uuid = "8f1860aa-5d34-43b1-b70e-917fe5227642";
            if (const UUID uuid4{uuid}; uuid4.toString() != uuid) {
                return failTest("uuid string mismatch");
            }

            return true;
        };
    };

    class TestUUIDCreationFromByteArray final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[UUID]";
        }

        [[nodiscard]] std::string name() const override {
            return "should create a valid uuid from byte array";
        }

        [[nodiscard]] bool execute() const override {
            const std::array<uint8_t, 16> arr = {
                0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
            };
            const UUID uuid(arr);

            if (uuid.toString() != "12345678-9abc-def0-0123-456789abcdef") {
                return failTest("uuid string mismatch");
            }

            if (const UUID uuid2("12345678-9abc-def0-0123-456789abcdef"); uuid != uuid2) {
                return failTest("uuid mismatch");
            }

            return true;
        };
    };

    class TestUUIDComparison final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[UUID]";
        }

        [[nodiscard]] std::string name() const override {
            return "should compare two uuids";
        }

        [[nodiscard]] bool execute() const override {
            const std::string uuid1 = "8f1860aa-5d34-43b1-b70e-917fe5227642";
            const std::string uuid2 = "8f1860aa-5d34-43b1-b70e-917fe5227643";

            const UUID uuid1Obj{uuid1};
            const UUID uuid2Obj{uuid2};

            // Equality test
            if (uuid1Obj == uuid2Obj) {
                return failTest("uuids should not be equal");
            };

            // Comparison test
            if (uuid2Obj < uuid1Obj) {
                return failTest("uuid2 should be less than uuid1");
            }

            return true;
        };
    };
} // namespace TESTS::DATATYPES

#endif //TEST_UUID_TYPE_HPP
