//
// Created by aiden on 7/13/2025.
//

#include "memtable.hpp"

#include "../abstract/timestamp_generator.hpp"

namespace memtable {
    void MemTable::put(const Entry &entry) const {
        this->tree_->insert(entry);
    }

    std::optional<Entry> MemTable::get(const std::string &key) const {
        // return this->tree_->search(key);
    }
} // namespace MemTable
