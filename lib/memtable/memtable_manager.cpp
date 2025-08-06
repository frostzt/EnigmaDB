//
// Created by frostzt on 8/4/2025.
//

#include "memtable_manager.hpp"

#include <ranges>

namespace memtable {
    void MemTableManager::apply(const core::Entry &entry) {
        this->maybeRotate();
        this->active_->applyEntry(entry);
    }

    void MemTableManager::maybeRotate() {
        if (this->active_->size() < kMaxEntries) return;

        this->active_->freeze();

        std::lock_guard lock(this->frozenMutex_);

        this->frozen_.push_back(this->active_);
        this->active_ = std::make_shared<MemTable>(this->tableName_);
    }

    std::optional<core::Entry> MemTableManager::get(const core::Key &key) const {
        if (auto found = this->active_->get(key); found) return found;

        std::lock_guard lock(this->frozenMutex_);
        for (const auto & it : std::ranges::reverse_view(this->frozen_)) {
            if (auto found = it->get(key); found) return found;
        }

        return std::nullopt;
    }

    std::shared_ptr<MemTable> MemTableManager::flushOldestFrozen() {
        std::lock_guard lock(this->frozenMutex_);
        if (this->frozen_.empty()) return nullptr;

        const auto oldest = this->frozen_.front();
        this->frozen_.pop_front();
        return oldest;
    }
} // namespace memtable
