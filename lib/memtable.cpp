//
// Created by aiden on 7/13/2025.
//

#include "memtable.hpp"

#include "timestamp_generator.hpp"

void MemTable::put(const Entry &entry) {
    tree_.insert(entry);
    currentSize_++;
}

void MemTable::remove(const std::string &primaryKey) {
    TimestampGenerator tsGen;
    const Entry tombstone("", primaryKey, {}, true, tsGen.next());
    tree_.insert(tombstone);
}

Node<Entry> *MemTable::get(const std::string &primaryKey) const {
    return tree_.search(Entry("", primaryKey, {}));
}

bool MemTable::shouldFlush() const {
    return currentSize_ > sizeThreshold_;
}

std::vector<Entry> MemTable::flush() {
    std::vector<Entry> sorted = tree_.toSortedVector();
    tree_.clear();
    currentSize_ = 0;
    return sorted;
}
