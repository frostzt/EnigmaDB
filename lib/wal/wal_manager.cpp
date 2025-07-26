//
// Created by frostzt on 7/24/2025.
//

#include <sys/stat.h>

#include "wal_manager.hpp"
#include "wal_codec.hpp"

namespace WAL {
    size_t WALManager::getFileSize(const std::string &path) {
        struct stat fileStat{}; /* POSIX ONLY */
        if (stat(path.c_str(), &fileStat) < 0) {
            return 0;
        }

        return fileStat.st_size;
    }

    std::string WALManager::filePath(const uint32_t fileId) const {
        std::string filepath = walDir_ + "/" + std::to_string(fileId) + ".wal";
        return filepath;
    }

    bool WALManager::append(const Entry &entry) {
        std::lock_guard guard(this->writeMutex_);

        const auto bytesWritten = WAL::writeRecord(this->currentOut_, entry, WAL::FlushMode::FORCE_FLUSH);
        this->currentBytesWritten_ += bytesWritten;

        if (this->currentBytesWritten_ >= this->maxFileSizeBytes_) {
            this->rotate(); // Rotate the WAL file
        }

        // Utility::Logger::debug("WAL: Wrote {} bytes to WAL file {} (total: {})", bytesWritten, this->currentFileId_,
        //               this->currentBytesWritten_);

        return true;
    }

    void WALManager::rotate() {
        if (this->currentOut_.is_open()) {
            this->currentOut_.flush(); // Flush the current buffer, append NOT necessarily forces flush
            this->currentOut_.close();
        }

        // Update the file counter
        this->currentFileId_++;

        // Open the new WAL file and set it as current ofstream
        const std::string path = this->filePath(this->currentFileId_);
        this->currentOut_.open(path, std::ios::out | std::ios::app | std::ios::binary);
        if (!this->currentOut_.good()) {
            // Utility::Logger::error("WAL: Failed to open WAL file '{}' for writing!", path);
            throw std::runtime_error("WAL: Failed to open WAL file for writing!");
        }

        // Reset fields for the new file
        this->currentBytesWritten_ = 0;

        // Utility::Logger::info("WAL: Rotated WAL file to {}", path);
    }

    void WALManager::close() {
        std::lock_guard guard(this->writeMutex_);

        if (this->currentOut_.is_open()) {
            this->currentOut_.flush();
            this->currentOut_.close();
        }
    }
} // namespace WAL
