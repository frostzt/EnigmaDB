//
// Created by aiden on 7/13/2025.
//

#ifndef MEMTABLE_HPP
#define MEMTABLE_HPP

#include <shared_mutex>

#include "lib/abstract/avl.hpp"
#include "lib/entry/entry.hpp"

namespace memtable {
    class MemTable {
    public:
        using KeyType = std::string;

        explicit MemTable(std::string tableName): tableName_(std::move(tableName)) {
        }

        void put(const core::Entry &entry) const;

        std::optional<core::Entry> get(const std::string &key) const;

        void del(const std::string &key, uint64_t timestamp);

        void freeze();

        bool isFrozen() const;

        std::vector<core::Entry> orderedEntries() const;

        void applyEntry(const core::Entry &entry);

        size_t size() const;

    private:
        std::string tableName_;
        std::unique_ptr<AVLTree<core::Entry> > tree_;
        mutable std::shared_mutex rwMutex_;
        std::atomic<bool> frozen_;
    };
} // namespace memtable

#endif //MEMTABLE_HPP
