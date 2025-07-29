//
// Created by aiden on 7/28/2025.
//

#ifndef WAL_WRITER_HPP
#define WAL_WRITER_HPP

#include <atomic>
#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <mutex>
#include <string>

#include "lib/entry/entry.hpp"

namespace WAL {
    class WALWriter {
    private:
        /** A unique identifier for this Writer */
        size_t writerId_;

        /** WAL File directory */
        std::string walDir_;

        /** Current file ofstream */
        std::ofstream currentOutStream_;

        /** Max file byte size to be stored */
        size_t maxFileSize_;

        /** Current file number */
        std::atomic<uint32_t> currentFileNumber_;

        /** Current file size in bytes */
        size_t currentFileSize_;

        // Mutexes
        std::mutex writeMutex_;

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

        bool init() {
            this->currentFileNumber_ = 1;
            const std::string filePath = this->filePath(this->currentFileNumber_);
            this->currentOutStream_.open(filePath, std::ios::out | std::ios::app | std::ios::binary);
            if (!this->currentOutStream_.good()) return false;

            return true;
        }

    public:
        explicit WALWriter(size_t writerId, std::string walDir, const size_t maxFileByteSize): writerId_(writerId),
            walDir_(std::move(walDir)),
            maxFileSize_(maxFileByteSize),
            currentFileNumber_(0),
            currentFileSize_(0) {
            if (!init()) {
                std::cerr << "Failed to initialize WALWriter" << std::endl;
                return;
            }
        }

        /**
         * Appends a new entry to the current Write-Ahead Log (WAL) file. The method
         * ensures thread safety by using a mutex to synchronize writes. After appending
         * the entry, the total number of bytes written is updated and logged.
         *
         * @param entry The Entry object to append to the current WAL file.
         * @return true if the entry was successfully written to the WAL.
         */
        bool append(const Entry &entry);

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

        /**
         * Flushes the current Write-Ahead Log (WAL) file stream to ensure that
         * any buffered data is written to the underlying storage. This method
         * is thread-safe and utilizes a mutex to prevent concurrent writes during
         * the flushing operation.
         *
         * This ensures the durability of the log data by synchronizing the buffer
         * state with the permanent storage.
         */
        void flush();

        /**
         * Returns the current file number of the WAL file.
         *
         * @return The current file number of the WAL file.
         */
        uint32_t getCurrentFileNumber() const {
            return this->currentFileNumber_;
        }
    };
} // namespace WAL

#endif //WAL_WRITER_HPP
