//
// Created by frostzt on 8/2/2025.
//

#pragma once
#ifndef TYPE_REGISTRY_HPP
#define TYPE_REGISTRY_HPP

#include "type_descriptor.hpp"
#include "field_type.hpp"

namespace core::datatypes {
    class TypeRegistry {
    public:
        static const ITypeDescriptor* get(FieldType type);
    };
} // namespace core::datatypes

#endif //TYPE_REGISTRY_HPP
