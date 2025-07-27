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
#include "lib/utils/constants.hpp"

namespace WAL {
    /**
     * Represents a manager for Write-Ahead Logging (WAL). This class
     * handles operations such as appending entries, rotating WAL files,
     * and loading entries from WAL files. It ensures thread safety for
     * read and write operations.
     */
    class WALManager {
    private:
        /** Represents the directory path where Write-Ahead Log (WAL) files are stored. */
        std::string walDir_;

        /** Represents the maximum bytes that can be stored in one WAL file. Once achieved, the file will be rotated. */
        size_t maxFileSizeBytes_;

        /** Represents the current WAL file (ofstream) */
        std::ofstream currentOut_;

        /** Stores the bytes currently written and flushed into the current ofstream */
        std::atomic<size_t> currentBytesWritten_;

        /** The current WAL File ID, file is are used to construct the next WAL File */
        uint32_t currentFileId_;

        /** Current entries stored in the buffer, after certain entries this can be used to flush */
        size_t currentEntriesInBuffer_;

        /** Maximum number of WAL entries to buffer in memory */
        size_t maxEntriesInBuffer_{};

        // Mutexes
        std::mutex writeMutex_;
        std::shared_mutex readMutex_;

        /**
         * Constructs the full file path for a Write-Ahead Log (WAL) file
         * given its file identifier. The file path is generated based on
         * the WAL directory path and includes the fileId as part of the name.
         *
         * @param fileId The unique identifier for the WAL file.
         * @return The complete file path for the specified WAL file as a string.
         */
        std::string filePath(uint32_t fileId) const;

        /**
         * Retrieves the size of a file specified by its path.
         * This method uses system calls to determine the size of the file
         * in bytes. If the file does not exist or an error occurs during
         * the retrieval process, it returns 0.
         *
         * @param path The file path for which the size is to be determined.
         * @return The size of the specified file in bytes. Returns 0 if the file
         *         does not exist or an error occurs.
         */
        static size_t getFileSize(const std::string &path);

        uint32_t getLatestWALId();

        bool init() {
            this->currentFileId_ = 0;
            const std::string path = filePath(this->currentFileId_);
            this->currentEntriesInBuffer_ = 0;
            this->currentOut_.open(path, std::ios::out | std::ios::app | std::ios::binary);
            if (!this->currentOut_.good()) return false;

            this->currentBytesWritten_ = getFileSize(path);
            return true;
        }

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

    public:
        /**
         * Constructs and initializes the WALManager with the specified WAL directory
         * and maximum file size. This constructor sets up the initial state of the
         * WALManager, including the WAL directory path, the maximum size for each WAL
         * file in bytes, and internal counters. If initialization fails, it throws
         * an exception.
         *
         * @param walDir The directory where WAL files will be stored.
         * @param maxEntriesToBuffer The maximum entries (inclusive) to buffer
         *                          in-memory before flushing to disk.
         *                          Larger values directly impact durability
         * @param maxFileSizeBytes The maximum size for each WAL file in bytes.
         *                         Defaults to 1 MB if not specified.
         * @throws std::runtime_error if the initialization process fails.
         */
        explicit WALManager(std::string walDir, size_t maxEntriesToBuffer = 1,
                            size_t maxFileSizeBytes = 1_MB): walDir_(std::move(walDir)),
                                                             maxFileSizeBytes_(maxFileSizeBytes),
                                                             currentBytesWritten_(0),
                                                             currentFileId_(0),
                                                             currentEntriesInBuffer_(0),
                                                             maxEntriesInBuffer_(maxEntriesToBuffer) {
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

        /**
         * Gracefully closes the current Write-Ahead Log (WAL) file by ensuring
         * that the file stream is flushed and closed safely. This method guarantees
         * thread safety by utilizing a mutex to lock the write operations during
         * the closure process.
         *
         * The method first flushes any buffered data to the file and then closes
         * the associated file stream if it is open. Closing the WAL ensures that
         * no further writing operations occur and flushes any pending data from
         * memory to disk.
         */
        void close();
    };
} // namespace WAL


#endif //WAL_MANAGER_HPP
