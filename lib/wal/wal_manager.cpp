//
// Created by frostzt on 7/24/2025.
//


#include "wal_manager.hpp"

#include <thread>

namespace WAL {
    bool WALManager::append(const Entry &entry) const {
        const auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
        const size_t writerIdx = threadId % this->writersCount_;
        return this->writers_[writerIdx]->append(entry);
    }

    void WALManager::close() const {
        for (const auto &writer : this->writers_) {
            writer->close();
        }
    }
} // namespace WAL
