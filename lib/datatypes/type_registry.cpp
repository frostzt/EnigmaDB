//
// Created by frostzt on 8/2/2025.
//

#include "type_registry.hpp"
#include "default_descriptor.hpp"

namespace core::datatypes {
    const ITypeDescriptor *TypeRegistry::get(const FieldType type) {
        static const Int64Descriptor int64Desc;
        static const Int32Descriptor int32Desc;
        static const DoubleDescriptor doubleDesc;
        static const StringDescriptor stringDesc;
        static const BoolDescriptor boolDesc;
        static const TimestampDescriptor timestampDesc;
        static const BinaryDescriptor binaryDesc;
        static const NullDescriptor nullDesc;
        static const UUIDDescriptor uuidDesc;

        switch (type) {
            case FieldType::Int32: return &int32Desc;
            case FieldType::Int64: return &int64Desc;
            case FieldType::Double: return &doubleDesc;
            case FieldType::String: return &stringDesc;
            case FieldType::Bool: return &boolDesc;
            case FieldType::UUID: return &uuidDesc;
            case FieldType::Binary: return &binaryDesc;
            case FieldType::Timestamp: return &timestampDesc;
            case FieldType::Null: return &nullDesc;
            case FieldType::Custom: throw std::runtime_error("No descriptor for custom type");
            default: throw std::runtime_error("Invalid type");
        };
    }
} // namespace core::datatypes
