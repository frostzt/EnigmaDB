//
// Created by frostzt on 7/24/2025.
//

#ifndef WAL_MANAGER_HPP
#define WAL_MANAGER_HPP

#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "lib/entry/entry.hpp"

namespace WAL {
    static constexpr size_t operator"" _MB(const unsigned long long int mb) {
        return mb * 1024 * 1024;
    }

    class WALManager {
    private:
        std::string walDir_;
        size_t maxFileSizeBytes_;
        std::ofstream currentOut_;
        std::atomic<size_t> currentBytesWritten_;
        uint32_t currentFileId_;
        size_t currentEntriesInBuffer_;

        // Mutexes
        std::mutex writeMutex_;
        std::shared_mutex readMutex_;

        std::string filePath(uint32_t fileId) const;

        static size_t getFileSize(const std::string &path);

        uint32_t getLatestWALId();

        bool init() {
            this->currentFileId_ = getLatestWALId();
            const std::string path = filePath(this->currentFileId_);
            this->currentEntriesInBuffer_ = 0;
            this->currentOut_.open(path, std::ios::out | std::ios::app | std::ios::binary);
            if (!this->currentOut_.good()) return false;

            this->currentBytesWritten_ = getFileSize(path);
            return true;
        }

    public:
        explicit WALManager(std::string walDir, size_t maxFileSizeBytes = 1_MB): walDir_(std::move(walDir)),
            maxFileSizeBytes_(maxFileSizeBytes),
            currentBytesWritten_(0),
            currentFileId_(0),
            currentEntriesInBuffer_(0) {
            if (!init()) {
                throw std::runtime_error("Failed to initialize WAL Manager!");
            }
        };

        /**
         * Appends a new entry to the current Write-Ahead Log (WAL) file. The method
         * ensures thread safety by using a mutex to synchronize writes. After appending
         * the entry, the total number of bytes written is updated and logged.
         *
         * @param entry The Entry object to append to the current WAL file.
         * @return true if the entry was successfully written to the WAL.
         */
        bool append(const Entry &entry);

        std::vector<Entry> loadAll();

        void close();

        /**
         * Rotates the Write-Ahead Log (WAL) file by closing the current file,
         * opening a new file for writing, and resetting relevant internal
         * tracking fields. This method ensures thread safety with a mutex.
         *
         * The rotation involves safely flushing and closing the existing
         * file stream, followed by initializing a new WAL file with an
         * incremented file identifier.
         *
         * @throws std::runtime_error if the new WAL file fails to open for writing
         */
        void rotate();
    };
} // namespace WAL


#endif //WAL_MANAGER_HPP
