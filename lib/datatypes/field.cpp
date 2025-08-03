//
// Created by frostzt on 8/1/2025.
//

#include "field.hpp"

namespace core::datatypes {
    bool valueMatchesType(const FieldValue &val, const FieldType type) {
        // Note: My IDE is going all out on this because `actual` is only used to decay and get
        //       a type; it is asking to overload all types but it is not required as we can get
        //       the type by decaying bruh :|
        return std::visit([&](auto &&actual) -> bool {
            using T = std::decay_t<decltype(actual)>;

            switch (type) {
                case FieldType::Int32: return std::is_same_v<T, int32_t>;
                case FieldType::Int64: return std::is_same_v<T, int64_t>;
                case FieldType::Double: return std::is_same_v<T, double>;
                case FieldType::String: return std::is_same_v<T, std::string>;
                case FieldType::Bool: return std::is_same_v<T, bool>;
                case FieldType::Timestamp: return std::is_same_v<T, std::chrono::system_clock::time_point>;
                case FieldType::Binary: return std::is_same_v<T, std::vector<uint8_t> >;
                case FieldType::Null: return std::is_same_v<T, std::monostate>;
                case FieldType::Custom: return true; // validated by descriptor
                case FieldType::UUID: return std::is_same_v<T, UUID>;
                default: return false;
            }
        }, val);
    }

    bool Field::isNull() const {
        return std::holds_alternative<std::monostate>(this->value_);
    }

    std::string Field::toString() const {
        return this->getDescriptor()->toString(this->value_);
    }

    std::vector<std::byte> Field::serialize() const {
        std::vector<std::byte> out;
        out.push_back(static_cast<std::byte>(this->type_));

        const auto *desc = this->getDescriptor();
        std::vector<std::byte> data = desc->serialize(this->value_);
        out.insert(out.end(), data.begin(), data.end());

        return out;
    }

    Field Field::deserialize(const std::byte *data, const size_t len) {
        if (len < 1) throw std::runtime_error("Invalid field length");

        const auto type = static_cast<FieldType>(data[0]);
        const auto *desc = TypeRegistry::get(type);
        const FieldValue val = desc->deserialize(data + 1, len - 1);

        return Field{val, type, nullptr};
    }
} // namespace core::datatypes
