//
// Created by frostzt on 8/1/2025.
//

#ifndef BUILTIN_DESCRIPTORS_HPP
#define BUILTIN_DESCRIPTORS_HPP

#include <string>

#include "lib/datatypes/type_descriptor.hpp"

namespace core::datatypes {
    template<typename T>
    class DefaultComparator<T> : public ITypeDescriptor {
        [[nodiscard]] bool equals(const FieldValue &x, const FieldValue &y) const override {
            assert(x.index() == y.index());
            return std::get<T>(x) == std::get<T>(y);
        }

        [[nodiscard]] bool lessThan(const FieldValue &x, const FieldValue &y) const override {
            assert(x.index() == y.index());
            return std::get<T>(x) < std::get<T>(y);
        }
    };

    class Int64Descriptor final : public DefaultComparator<int64_t> {
        [[nodiscard]] std::string name() const override {
            return "Int64";
        }
    };

    class Int32Descriptor final : public DefaultComparator<int32_t> {
        [[nodiscard]] std::string name() const override {
            return "Int32";
        }
    };

    class DoubleDescriptor final : public DefaultComparator<double> {
        [[nodiscard]] std::string name() const override {
            return "Double";
        }
    };

    class StringDescriptor final : public DefaultComparator<std::string> {
        [[nodiscard]] std::string name() const override {
            return "String";
        }
    };

    class BoolDescriptor final : public DefaultComparator<bool> {
        [[nodiscard]] std::string name() const override {
            return "Bool";
        }
    };

    class TimestampDescriptor final : public DefaultComparator<std::chrono::system_clock::time_point> {
        [[nodiscard]] std::string name() const override {
            return "Timestamp";
        }
    };

    class BinaryDescriptor final : public DefaultComparator<std::vector<uint8_t> > {
        [[nodiscard]] std::string name() const override {
            return "Binary";
        }
    };

    class NullDescriptor final : public DefaultComparator<std::monostate> {
        [[nodiscard]] std::string name() const override {
            return "Null";
        }
    };

    class UUIDDescriptor final : public DefaultComparator<std::string> {
        [[nodiscard]] std::string name() const override {
            return "UUID";
        }
    };
} // namespace core::datatypes

#endif //BUILTIN_DESCRIPTORS_HPP
