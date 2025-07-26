//
// Created by aiden on 7/26/2025.
//

#include "wal_manager_test.hpp"
#include "lib/wal/wal_manager.hpp"

#include <filesystem>
#include <thread>

bool TestWALConcurrentAppendsWithRotation::execute() const {
    const std::string path = "test_concurrent_wal";
    std::filesystem::create_directory(path);

    // Construct a new manager with 4KB maxFileSizeBytes for quick rotation
    WAL::WALManager manager(path, 4_KB);

    constexpr int threadCount = 5;
    constexpr int perThreadCount = 5000;

    std::vector<std::thread> threads;
    std::vector<std::vector<bool> > threadResults(threadCount);
    threads.reserve(threadCount);

    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([&manager, &threadResults, i]() {
            for (size_t j = 0; j < perThreadCount; ++j) {
                const bool ok = manager.append(Entry("customer", "cid_" + std::to_string(j), {}, false));
                threadResults[i].push_back(ok);
            }
        });
    }

    for (auto &t : threads) t.join();

    manager.close();
    std::filesystem::remove_all(path);
    return true;
}
