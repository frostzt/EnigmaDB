//
// Created by aiden on 8/1/2025.
//

#ifndef TYPE_DESCRIPTOR_HPP
#define TYPE_DESCRIPTOR_HPP

#include <string>

#include "field.hpp"

namespace core::datatypes {
    class ITypeDescriptor {
    public:
        virtual ~ITypeDescriptor() = default;

        [[nodiscard]] virtual std::string name() const = 0;

        [[nodiscard]] virtual bool equals(const FieldValue &x, const FieldValue &y) const = 0;

        [[nodiscard]] virtual bool lessThan(const FieldValue &x, const FieldValue &y) const = 0;

        [[nodiscard]] virtual std::vector<std::byte> serialize(const FieldValue &v) const = 0;

        [[nodiscard]] virtual FieldValue deserialize(const std::byte *data, size_t len) const = 0;
    };
} // namespace core::datatypes


#endif //TYPE_DESCRIPTOR_HPP
