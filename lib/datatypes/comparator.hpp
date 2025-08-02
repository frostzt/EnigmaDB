//
// Created by frostzt on 8/1/2025.
//

#ifndef BUILTIN_DESCRIPTORS_HPP
#define BUILTIN_DESCRIPTORS_HPP

#include <string>

#include "lib/datatypes/type_descriptor.hpp"

namespace core::datatypes {
    template<typename T>
    struct ComparatorOnly<T> {
        virtual ~ComparatorOnly() = default;

        [[nodiscard]] virtual bool equals(const FieldValue &x, const FieldValue &y) const {
            assert(x.index() == y.index());
            return std::get<T>(x) == std::get<T>(y);
        }

        [[nodiscard]] virtual bool lessThan(const FieldValue &x, const FieldValue &y) const {
            assert(x.index() == y.index());
            return std::get<T>(x) < std::get<T>(y);
        }
    };

} // namespace core::datatypes

#endif //BUILTIN_DESCRIPTORS_HPP
