//
// Created by frostzt on 8/7/2025.
//

#ifndef FILTER_POLICY_HPP
#define FILTER_POLICY_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace sstable {
    class FilterPolicy {
    public:
        virtual ~FilterPolicy() = default;

        virtual void add(const std::string &key) = 0;

        [[nodiscard]] virtual bool mayContain(const std::string &key) const = 0;

        virtual void serialize(std::vector<uint8_t> *out) const = 0;

        virtual void deserialize(const uint8_t *data, size_t len) = 0;

        [[nodiscard]] virtual std::string_view name() const = 0;
    };
} // namespace sstable

#endif //FILTER_POLICY_HPP
