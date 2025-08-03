//
// Created by frostzt on 8/2/2025.
//

#pragma once
#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include "lib/datatypes/field.hpp"

namespace TESTS {
    inline core::datatypes::Field makeField(std::string v) {
        return core::datatypes::Field{std::move(v), core::datatypes::FieldType::String, nullptr};
    }

    inline core::datatypes::Field makeField(int32_t v) {
        return core::datatypes::Field{v, core::datatypes::FieldType::Int32, nullptr};
    }

    inline core::datatypes::Field makeField(int64_t v) {
        return core::datatypes::Field{v, core::datatypes::FieldType::Int64, nullptr};
    }

    inline core::datatypes::Field makeField(double v) {
        return core::datatypes::Field{v, core::datatypes::FieldType::Double, nullptr};
    }
} // namespace TESTS

#endif //TEST_UTILS_HPP
