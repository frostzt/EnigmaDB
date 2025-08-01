//
// Created by frostzt on 8/1/2025.
//

#include "field.hpp"

namespace core::datatypes {
    bool valueMatchesType(const FieldValue &val, const FieldType type) {
        // Note: My IDE is going all out on this because `actual` is only used to decay and get
        //       a type; it is asking to overload all types but it is not required as we can get
        //       the type by decaying bruh :|
        return std::visit([&](auto&& actual) -> bool {
            using T = std::decay_t<decltype(actual)>;

            switch (type) {
                case FieldType::Int32: return std::is_same_v<T, int32_t>;
                case FieldType::Int64: return std::is_same_v<T, int64_t>;
                case FieldType::Double: return std::is_same_v<T, double>;
                case FieldType::String: return std::is_same_v<T, std::string>;
                case FieldType::Bool: return std::is_same_v<T, bool>;
                case FieldType::Timestamp: return std::is_same_v<T, std::chrono::system_clock::time_point>;
                case FieldType::Binary: return std::is_same_v<T, std::vector<uint8_t>>;
                case FieldType::Null: return std::is_same_v<T, std::monostate>;
                case FieldType::Custom: return true; // validated by descriptor
                default: return false;
            }
        }, val);
    }


    bool Field::isNull() const {
        return std::holds_alternative<std::monostate>(this->value_);
    }
} // namespace core::datatypes
