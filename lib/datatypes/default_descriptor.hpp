//
// Created by frostzt on 8/2/2025.
//

#pragma once
#ifndef DEFAULT_DESCRIPTOR_HPP
#define DEFAULT_DESCRIPTOR_HPP

#include "comparator.hpp"
#include "serializer.hpp"
#include "types/uuid.hpp"

#include <cassert>

namespace core::datatypes {
    template<typename T>
    class DefaultDescriptor : public ITypeDescriptor, public ComparatorOnly<T> {
    public:
        [[nodiscard]] std::string name() const override = 0;

        [[nodiscard]] bool equals(const FieldValue &x, const FieldValue &y) const override {
            return ComparatorOnly<T>::equals(x, y);
        };

        [[nodiscard]] bool lessThan(const FieldValue &x, const FieldValue &y) const override {
            return ComparatorOnly<T>::lessThan(x, y);
        };

        [[nodiscard]] std::vector<std::byte> serialize(const FieldValue &v) const override {
            return Serializer<T>::serialize(std::get<T>(v));
        };

        [[nodiscard]] FieldValue deserialize(const std::byte *data, size_t len) const override {
            return Serializer<T>::deserialize(data, len);
        }

        [[nodiscard]] std::string toString(const FieldValue &v) const override {
            assert(std::holds_alternative<T>(v));
            return Serializer<T>::toString(std::get<T>(v));
        }
    };

    class Int64Descriptor final : public DefaultDescriptor<int64_t> {
        [[nodiscard]] std::string name() const override { return "Int64"; }
    };

    class Int32Descriptor final : public DefaultDescriptor<int32_t> {
        [[nodiscard]] std::string name() const override { return "Int32"; }
    };

    class DoubleDescriptor final : public DefaultDescriptor<double> {
        [[nodiscard]] std::string name() const override { return "Double"; }
    };

    class StringDescriptor final : public DefaultDescriptor<std::string> {
        [[nodiscard]] std::string name() const override { return "String"; }
    };

    class BoolDescriptor final : public DefaultDescriptor<bool> {
        [[nodiscard]] std::string name() const override { return "Bool"; }
    };

    class TimestampDescriptor final : public DefaultDescriptor<std::chrono::system_clock::time_point> {
        [[nodiscard]] std::string name() const override { return "Timestamp"; }
    };

    class BinaryDescriptor final : public DefaultDescriptor<std::vector<uint8_t> > {
        [[nodiscard]] std::string name() const override { return "Binary"; }
    };

    class NullDescriptor final : public DefaultDescriptor<std::monostate> {
        [[nodiscard]] std::string name() const override { return "Null"; }
    };

    class UUIDDescriptor final : public DefaultDescriptor<UUID> {
        [[nodiscard]] std::string name() const override { return "UUID"; }
    };
} // namespace core::datatypes

#endif //DEFAULT_DESCRIPTOR_HPP
