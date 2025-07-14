//
// Created by aiden on 7/13/2025.
//

#ifndef MEMTABLE_HPP
#define MEMTABLE_HPP

#include "avl.hpp"
#include "entry.hpp"

class MemTable {
private:
    BinarySearchTree<Entry> tree_;
    size_t sizeThreshold_;
    size_t currentSize_;

public:
    explicit MemTable(const size_t threshold = 1000): sizeThreshold_(threshold), currentSize_(0) {
    }

    void put(const Entry &entry);

    void remove(const std::string &primaryKey);

    Node<Entry> *get(const std::string &primaryKey) const;

    [[nodiscard]] bool shouldFlush() const;

    std::vector<Entry> flush();
};

#endif //MEMTABLE_HPP
