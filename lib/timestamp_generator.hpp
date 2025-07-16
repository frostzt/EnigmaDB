//
// Created by aiden on 7/15/2025.
//

#ifndef TIMESTAMP_GENERATOR_HPP
#define TIMESTAMP_GENERATOR_HPP

#include <atomic>
#include <cstdint>

class TimestampGenerator {
private:
    std::atomic<uint64_t> logicalCounter_{};
    std::atomic<uint64_t> lastReturned_{};
public:
    uint64_t next();
};

#endif //TIMESTAMP_GENERATOR_HPP
