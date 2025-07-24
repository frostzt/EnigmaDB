//
// Created by aiden on 7/24/2025.
//

#ifndef WAL_MANAGER_HPP
#define WAL_MANAGER_HPP

#include <fstream>
#include <string>

#include "lib/entry/entry.hpp"

class WALManager {
private:
    std::string walDir_;
    size_t maxFileSizeBytes_;
    std::ofstream currentOut_;
    size_t currentBytesWritten_;
    uint32_t currentFileId_;

    std::string filePath(uint32_t fileId);

    size_t getFileSize(const std::string &path);

    uint32_t getLatestWALId();

    bool init() {
        currentFileId_ = getLatestWALId();
        const std::string path = filePath(currentFileId_);
        currentOut_.open(path, std::ios::out | std::ios::app | std::ios::binary);
        if (!currentOut_.good()) return false;

        currentBytesWritten_ = getFileSize(path);
        return true;
    }

public:
    static constexpr size_t operator"" _MB(const unsigned long long int mb) { return mb * 1024 * 1024; }

    explicit WALManager(std::string walDir, size_t maxFileSizeBytes = 1_MB): walDir_(std::move(walDir)),
                                                                             maxFileSizeBytes_(maxFileSizeBytes),
                                                                             currentBytesWritten_(0),
                                                                             currentFileId_(0) {
        if (!init()) {
            throw std::runtime_error("Failed to initialize WAL Manager!");
        }
    };

    bool append(const Entry &entry);

    std::vector<Entry> loadAll();

    void close();

    void rotate();
};


#endif //WAL_MANAGER_HPP
