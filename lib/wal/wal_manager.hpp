//
// Created by frostzt on 7/24/2025.
//

#ifndef WAL_MANAGER_HPP
#define WAL_MANAGER_HPP

#include <functional>
#include <memory>

#include "wal_writer.hpp"
#include "lib/entry/entry.hpp"

namespace WAL {
    /**
     * Represents a manager for Write-Ahead Logging (WAL). This class
     * handles operations such as appending entries, rotating WAL files,
     * and loading entries from WAL files. It ensures thread safety for
     * read and write operations.
     */
    class WALManager {
    private:
        std::string walDir_;

        std::vector<std::unique_ptr<WALWriter> > writers_;
        size_t writersCount_;

    public:
        explicit WALManager(const size_t numWriters, const size_t maxFileSize, std::string &walDir) {
            // Store the wal dir for later use
            this->walDir_ = walDir;

            for (size_t i = 0; i < numWriters; i++) {
                writers_.emplace_back(std::make_unique<WALWriter>(i + 1, walDir, maxFileSize));
            }

            this->writersCount_ = numWriters;
        }

        [[nodiscard]] bool append(const Entry &entry) const;

        void close() const;

        void loadAll(const std::function<void(Entry &&)>& replyFn) const;

        [[nodiscard]] std::vector<Entry> loadAll() const;

        [[nodiscard]] uint32_t getWritersMetaData() const {
            uint32_t total = 0;
            for (int i = 0; i < this->writersCount_; i++) {
                const auto writer = this->writers_[i].get();
                const auto fileNumber = writer->getCurrentFileNumber();
                std::cout << "WRITER [ " << i << " ] " << "File Number: " << fileNumber << std::endl;

                total += fileNumber;
            }

            return total;
        };
    };
} // namespace WAL


#endif //WAL_MANAGER_HPP
