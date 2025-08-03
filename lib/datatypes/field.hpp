//
// Created by aiden on 8/1/2025.
//

#pragma once
#ifndef FIELD_HPP
#define FIELD_HPP

#include "type_registry.hpp"
#include "field_value.hpp"
#include "field_type.hpp"

#include <cassert>
#include <chrono>
#include <vector>
#include <string>

namespace core::datatypes {
    class ITypeDescriptor;

    bool valueMatchesType(const FieldValue &val, FieldType type);

    /**
     * @struct Field
     * @brief Represents a field in a data structure, containing a value, type, and optional descriptor.
     *
     * This structure encapsulates the concept of a field, which is a core component of various data
     * structures. A field contains a value, its type, and an optional type descriptor to provide
     * additional metadata or processing capabilities for custom field types.
     */
    struct Field {
        /** Represents the value held by this field; this value should match the field type */
        FieldValue value_;

        /** Represents the type of value */
        FieldType type_;

        /**
         * @brief Represents an optional type descriptor for custom field types.
         *
         * This shared pointer is used to hold a type descriptor that provides additional
         * metadata or processing capabilities for fields with custom types. It is an optional
         * attribute and may remain null for fields that do not require a custom descriptor.
         */
        std::shared_ptr<ITypeDescriptor> descriptor_;

        Field(FieldValue v, const FieldType t,
              std::shared_ptr<ITypeDescriptor> desc): value_(std::move(v)), type_(t), descriptor_(std::move(desc)) {
            assert(valueMatchesType(value_, type_));
        };

        [[nodiscard]] bool isNull() const;

        [[nodiscard]] const ITypeDescriptor *getDescriptor() const {
            return this->descriptor_ ? this->descriptor_.get() : TypeRegistry::get(this->type_);
        }

        [[nodiscard]] std::vector<std::byte> serialize() const;

        static Field deserialize(const std::byte *data, size_t len);

        bool operator==(const Field &other) const {
            return getDescriptor()->equals(this->value_, other.value_);
        }

        bool operator<(const Field &other) const {
            return getDescriptor()->lessThan(this->value_, other.value_);
        }

        [[nodiscard]] std::string toString() const;
    };
} // namespace core::datatypes

#endif //FIELD_HPP
