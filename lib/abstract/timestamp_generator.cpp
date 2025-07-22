//
// Created by aiden on 7/15/2025.
//


#include "timestamp_generator.hpp"

#include <chrono>

uint64_t TimestampGenerator::next() {
    // Generate milliseconds from the current epoch with the system clock
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // Take Wall Clock's 48 Bits and Lamport Counters 16 bits
    const uint64_t wallTime48 = (static_cast<uint64_t>(millis) & 0xFFFFFFFFFFFF) << 16;
    uint64_t lamport16 = logicalCounter_++ & 0xFFFF; // NOTE: incremented it here

    uint64_t candidate = wallTime48 | lamport16;
    const uint64_t prev = lastReturned_.load();

    // Account for clock rebalance
    while (candidate <= prev) {
        lamport16 = logicalCounter_++ & 0xFFFF;
        candidate = wallTime48 | lamport16;
    }

    lastReturned_.store(candidate);

    return candidate;
}

