//
// Created by aiden on 8/1/2025.
//

#ifndef FIELD_HPP
#define FIELD_HPP

#include <cassert>
#include <chrono>
#include <vector>
#include <string>
#include <variant>

namespace core::datatypes {
    class ITypeDescriptor;

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

    using FieldValue = std::variant<
        std::monostate, // NULL
        int32_t,
        int64_t,
        double,
        std::string,
        bool,
        std::vector<uint8_t>, // binary data
        std::chrono::system_clock::time_point
    >;

    bool valueMatchesType(const FieldValue& val, FieldType type);

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

        Field(FieldValue v, FieldType t,
              std::shared_ptr<ITypeDescriptor> desc): value_(std::move(v)), type_(t), descriptor_(std::move(desc)) {
            assert(valueMatchesType(value_, type_));
        };

        bool isNull() const;

        bool operator==(const Field &) const;

        bool operator<(const Field &) const;

        std::string toString() const;
    };
} // namespace core::datatypes

#endif //FIELD_HPP
