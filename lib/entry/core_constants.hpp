//
// Created by aiden on 7/31/2025.
//

#ifndef CORE_CONSTANTS_HPP
#define CORE_CONSTANTS_HPP

#include <string>
#include <variant>
#include <unordered_map>

#include "lib/datatypes/field.hpp"

namespace core {
    using Row = std::unordered_map<std::string, datatypes::Field>;
} // namespace core

#endif //CORE_CONSTANTS_HPP
