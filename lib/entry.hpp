#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <cstdint>
#include <vector>
#include <iostream>
#include <variant>
#include <unordered_map>

#include "timestamp_generator.hpp"

using Field = std::variant<int, std::string, double>;
using Row = std::unordered_map<std::string, Field>;

// TODO: Composite keys
class Entry {
public:
    std::string tableName;
    std::string primaryKey_;
    Row rowData_;
    bool isTombstone_{false};
    std::uint64_t timestamp_{};

    Entry() = default;

    Entry(std::string table, std::string pk, Row data, const bool tombstone = false, const uint64_t ts = 0)
        : tableName(std::move(table)), primaryKey_(std::move(pk)), rowData_(std::move(data)),
          isTombstone_(tombstone) {
        TimestampGenerator tsGen;
        this->timestamp_ = tsGen.next();
    }

    std::vector<std::byte> serialize() const;

    std::string toHex() const;

    // AVL Tree needs to compare Entries based on primaryKey
    bool operator<(const Entry &other) const {
        return primaryKey_ < other.primaryKey_;
    }

    bool operator==(const Entry &other) const {
        return primaryKey_ == other.primaryKey_;
    }


    friend std::ostream &operator<<(std::ostream &os, const Entry &e) {
        os << "[" << e.tableName << "] " << e.primaryKey_ << " {";

        for (const auto &[k, v]: e.rowData_) {
            os << k << ": ";
            std::visit([&](auto &&arg) {
                os << arg;
            }, v);
            os << ", ";
        }

        os << "} " << (e.isTombstone_ ? "[DELETED]" : "") << " @" << e.timestamp_;
        return os;
    }
};

#endif // ENTRY_HPP
