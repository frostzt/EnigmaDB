//
// Created by aiden on 7/13/2025.
//

#include "memtable.hpp"

#include <mutex>

namespace memtable {
    void MemTable::put(const core::Entry &entry) const {
        std::unique_lock lock(this->rwMutex_);
        if (this->frozen_) throw std::runtime_error("cannot insert into a frozen MemTable");
        this->tree_->insert(entry);
    }

    void MemTable::applyEntry(const core::Entry &entry) const {
        std::unique_lock lock(this->rwMutex_);
        if (this->frozen_) throw std::runtime_error("cannot insert into a frozen MemTable");
        this->tree_->insert(entry);
    }


    std::optional<core::Entry> MemTable::get(const core::Key &key) const {
        std::shared_lock lock(this->rwMutex_);

        const core::Entry dummy{this->tableName_, key, {}, false};
        const auto node = this->tree_->search(dummy);
        if (!node) return std::nullopt;

        return node->key;
    }

    void MemTable::del(const core::Key &key, const uint64_t timestamp) const {
        std::unique_lock lock(this->rwMutex_);
        if (this->frozen_) throw std::runtime_error("cannot delete from a frozen MemTable");

        if (const auto entryFound = this->get(key); !entryFound) return;

        const core::Entry tombstone{this->tableName_, key, {}, true, timestamp};
        this->tree_->insert(tombstone);
    }

    void MemTable::freeze() {
        std::unique_lock lock(this->rwMutex_);
        this->frozen_ = true;
    }

    bool MemTable::isFrozen() const {
        return this->frozen_;
    }

    std::vector<core::Entry> MemTable::orderedEntries() const {
        std::shared_lock lock(this->rwMutex_);
        return this->tree_->toSortedVector();
    }

    size_t MemTable::size() const {
        std::shared_lock lock(this->rwMutex_);
        return this->tree_->toSortedVector().size();
    }
} // namespace MemTable
