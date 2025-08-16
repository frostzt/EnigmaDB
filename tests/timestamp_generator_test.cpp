//
// Created by frostzt on 7/16/2025.
//

#include "lib/abstract/timestamp_generator.hpp"
#include "catch2/catch_test_macros.hpp"

#include <thread>
#include <vector>
#include <unordered_set>


TEST_CASE("should generate monotonically increasing timestamps", "[TIMESTAMP_GENERATOR]") {
    TimestampGenerator tsGen;

    uint64_t last = 0;
    for (size_t i = 0; i < 1000; i++) {
        const auto ts = tsGen.next();
        REQUIRE(ts > last);
        last = ts;
    }
};

TEST_CASE("should generate timestamps with no collisions with concurrent calls", "[TIMESTAMP_GENERATOR]") {
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
                threadResults[i].push_back(ts);
            }
        });
    }

    for (auto &t: threads) t.join();

    std::unordered_set<uint64_t> allTimestamps;
    for (const auto &vec: threadResults) {
        for (uint64_t ts: vec) {
            REQUIRE(allTimestamps.insert(ts).second);
        }
    }

    REQUIRE(allTimestamps.size() == threadCount * perThreadTsCount);
};
