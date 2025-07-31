//
// Created by aiden on 7/31/2025.
//

#ifndef KEY_HPP
#define KEY_HPP

#include <vector>
#include <sstream>

#include "core_constants.hpp"

namespace core {
    struct Key {
        std::vector<Field> parts_;

        Key() = default;

        explicit Key(std::vector<Field> components_): parts_(std::move(components_)) {
        }

        bool operator<(const Key &other) const {
            const size_t min = std::min(this->parts_.size(), other.parts_.size());
            for (size_t i = 0; i < min; ++i) {
                const auto &a = this->parts_[i];
                const auto &b = other.parts_[i];

                // Cross-type comparison fallback (int < double < string)
                if (a.index() != b.index()) {
                    return a.index() < b.index();
                }

                // Same-type comparison only — safely unwrap and compare
                const bool less = std::visit(
                    [](const auto &x, const auto &y) -> bool {
                        using T = std::decay_t<decltype(x)>;
                        if constexpr (std::is_same_v<T, decltype(y)>) {
                            return x < y;
                        } else {
                            return false;  // Should never happen after index() check
                        }
                    },
                    a, b
                );

                if (less) return true;
                if (a != b) return false;  // compare whole field variant if not less
            }

            return this->parts_.size() < other.parts_.size();
        }

        bool operator==(const Key &other) const {
            return parts_ == other.parts_;
        }

        [[nodiscard]] std::string toString() const {
            std::ostringstream oss;
            oss << "[";
            for (size_t i = 0; i < this->parts_.size(); ++i) {
                std::visit([&](auto &&x) { oss << x; }, this->parts_[i]);
                if (i != this->parts_.size() - 1) oss << ",";
            }

            oss << "]";
            return oss.str();
        };
    };
} // namespace core


#endif //KEY_HPP
