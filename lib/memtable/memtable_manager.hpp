//
// Created by frostzt on 8/4/2025.
//

#ifndef MEMTABLE_MANAGER_HPP
#define MEMTABLE_MANAGER_HPP

#include <memory>
#include <deque>

#include "memtable.hpp"

namespace memtable {
    class MemTableManager {
    private:
        std::string tableName_;
        std::shared_ptr<MemTable> active_;
        std::deque<std::shared_ptr<MemTable> > frozen_;

        mutable std::mutex frozenMutex_;
        static constexpr size_t kMaxEntries = 5000;

    public:
        void maybeRotate();

        void apply(const core::Entry &);

        std::optional<core::Entry> get(const core::Key &) const;

        std::shared_ptr<MemTable> flushOldestFrozen();
    };
} // namespace memtable

#endif //MEMTABLE_MANAGER_HPP
