//
// Created by frostzt on 7/16/2025.
//

#include "timestamp_generator_test.hpp"
#include "lib/timestamp_generator.hpp"

#include <thread>
#include <vector>
#include <unordered_set>

std::string TimestampMonotonicTest::name() const {
    return "should return monotonic values";
}

std::string TimestampConcurrencyTest::name() const {
    return "should stay unique under concurrency";
}

bool TimestampMonotonicTest::execute() const {
    TimestampGenerator tsGen;

    uint64_t last = 0;
    for (size_t i = 0; i < 1000; i++) {
        const auto ts = tsGen.next();
        if (last > ts) return false;

        last = ts;
    }

    return true;
}

bool TimestampConcurrencyTest::execute() const {
    TimestampGenerator tsGen;

    constexpr int threadCount = 10;
    constexpr int perThreadTsCount = 1000;

    std::vector<std::thread> threads;
    std::vector<std::vector<uint64_t> > threadResults(threadCount);

    threads.reserve(threadCount);

    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([&tsGen, &threadResults, i]() {
            for (size_t j = 0; j < perThreadTsCount; ++j) {
                uint64_t ts = tsGen.next();
                threadResults[i].push_back(ts); // Each thread writes to its own vec
            }
        });
    }

    for (auto &t: threads) t.join();

    std::unordered_set<uint64_t> allTimestamps;
    for (const auto &vec: threadResults) {
        for (uint64_t ts: vec) {
            if (!allTimestamps.insert(ts).second) return false; // duplicate found
        }
    }

    return allTimestamps.size() == threadCount * perThreadTsCount;
}

