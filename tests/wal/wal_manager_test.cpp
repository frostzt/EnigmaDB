//
// Created by aiden on 7/26/2025.
//

#include "wal_manager_test.hpp"
#include "lib/wal/wal_manager.hpp"

#include <algorithm>
#include <filesystem>
#include <thread>

int totalFilesInDir(const std::string &path) {
    auto dirItr = std::filesystem::directory_iterator(path);

    // This won't be narrowing in tests down below
    return static_cast<int>(std::count_if(
        begin(dirItr), end(dirItr), [](auto &entry) { return entry.is_regular_file(); }
    ));
}

bool TestWALConcurrentAppendsWithRotation::execute() const {
    std::string path = "wal";
    std::filesystem::create_directory(path);

    // Construct a new manager with 10 writers and 4KB maxFileSizeBytes for quick rotation
    WAL::WALManager manager(10, 4_KB, path);

    constexpr int threadCount = 100;
    constexpr int perThreadCount = 10000;

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

    for (auto &t: threads) t.join();

    // Get details
    const auto total = manager.getWritersMetaData();
    if (const auto totalFiles = totalFilesInDir(path); total != totalFiles) return false;

    // Close all the writers
    manager.close();
    std::filesystem::remove_all(path);
    return true;
}
