//
// Created by frostzt on 8/2/2025.
//

#pragma once
#ifndef FIELD_VALUE_HPP
#define FIELD_VALUE_HPP

#include "types/uuid.hpp"

#include <variant>
#include <string>
#include <chrono>
#include <vector>

namespace core::datatypes {
    using FieldValue = std::variant<
        std::monostate, // NULL
        int32_t,
        int64_t,
        double,
        std::string,
        bool,
        std::vector<uint8_t>, // binary data
        std::chrono::system_clock::time_point,
        UUID
    >;
} // namespace core::datatypes

#endif //FIELD_VALUE_HPP
