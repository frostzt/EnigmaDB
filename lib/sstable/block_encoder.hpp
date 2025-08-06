//
// Created by frostzt on 8/7/2025.
//

#ifndef BLOCK_ENCODER_HPP
#define BLOCK_ENCODER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace sstable {
    class BlockEncoder {
    public:
        virtual ~BlockEncoder() = default;

        virtual void add(const std::string &key, const std::string &value) = 0;

        virtual std::vector<uint8_t> finish() = 0;

        [[nodiscard]] virtual bool shouldFlush() const = 0;

        [[nodiscard]] virtual size_t estimatedSize() const = 0;
    };
} // namespace sstable

#endif //BLOCK_ENCODER_HPP
