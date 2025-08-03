//
// Created by frostzt on 8/2/2025.
//

#pragma once
#ifndef FIELD_TYPE_HPP
#define FIELD_TYPE_HPP

#include <cstdint>

namespace core::datatypes {
    /**
     * @enum FieldType
     * @brief Represents the type of a field in data structures.
     *
     * This enumeration defines the various data types that a field can represent.
     * These types include primitive data types, binary data, and custom data types.
     */
    enum class FieldType : uint8_t {
        Int32,
        Int64,
        Double,
        String,
        Bool,
        UUID,
        Binary,
        Timestamp,
        Null,
        Custom
    };
} // namespace core::datatypes

#endif //FIELD_TYPE_HPP
