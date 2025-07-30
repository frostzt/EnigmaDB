//
// Created by aiden on 7/28/2025.
//

#include <iomanip>
#include <sys/stat.h>

#include "wal_writer.hpp"
#include "wal_codec.hpp"

namespace WAL {
    std::string WALWriter::filePath(const uint32_t fileId) const {
        std::ostringstream oss;
        oss << this->walDir_ << "/" << "w_" << std::to_string(this->writerId_) << "_" << std::setw(8) << std::setfill('0') <<
                fileId <<
                ".wal";
        return oss.str();
    }

    void WALWriter::flush() {
        std::lock_guard guard(this->writeMutex_);
        this->currentOutStream_.flush();
    }

    size_t WALWriter::getFileSize(const std::string &path) {
        struct stat fileStat{}; /* POSIX ONLY */
        if (stat(path.c_str(), &fileStat) < 0) {
            return 0;
        }

        return fileStat.st_size;
    }

    bool WALWriter::append(const Entry &entry, const FlushMode flushMode = FlushMode::FORCE_FLUSH) {
        std::lock_guard guard(this->writeMutex_);

        const auto bytesWritten = WAL::writeRecord(this->currentOutStream_, entry, flushMode);
        this->currentFileSize_ += bytesWritten;

        // Soft rotation, this might exceed file size, but that would happen at max by 1 entry
        if (this->currentFileSize_ >= this->maxFileSize_) {
            this->rotate(); // Rotate the WAL file
        }

        return true;
    }

    void WALWriter::close() {
        std::lock_guard guard(this->writeMutex_);

        if (this->currentOutStream_.is_open()) {
            this->currentOutStream_.flush();
            this->currentOutStream_.close();
        }
    }

    void WALWriter::rotate() {
        if (this->currentOutStream_.is_open()) {
            this->currentOutStream_.flush(); // Flush the current buffer, append NOT necessarily forces flush
            this->currentOutStream_.close();
        }

        // Open the new WAL file and set it as current ofstream
        const std::string path = this->filePath(++this->currentFileNumber_); // update the file number
        this->currentOutStream_.open(path, std::ios::out | std::ios::app | std::ios::binary);
        if (!this->currentOutStream_.good()) {
            throw std::runtime_error("WAL: Failed to open WAL file for writing!");
        }

        // Reset fields for the new file
        this->currentFileSize_ = 0;
    }
} // namespace WAL
