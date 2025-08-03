//
// Created by aiden on 7/31/2025.
//

#include "key.hpp"

namespace core {
    inline Key buildKeyFromRow(const std::vector<std::string>& pkCols, const Row& row) {
        std::vector<datatypes::Field> keyParts;
        for (const auto& col : pkCols) {
            auto it = row.find(col);
            if (it == row.end()) throw std::runtime_error("missing pk field" + col);
            keyParts.push_back(it->second);
        }

        return Key{std::move(keyParts)};
    }
} // namespace core
