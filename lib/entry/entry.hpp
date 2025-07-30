#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <cstdint>
#include <vector>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <optional>

#include "lib/abstract/timestamp_generator.hpp"

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
        // Generate timestamp for this entry
        TimestampGenerator tsGen;
        this->timestamp_ = tsGen.next();
    }

    std::vector<std::byte> serialize() const;

    static std::optional<Entry> deserialize(const std::byte *data, size_t length);

    std::string toHex() const;

    static bool compareEntries(const Entry &e1, const Entry &e2) {
        if (e1.tableName != e2.tableName) return false;
        if (e1.primaryKey_ != e2.primaryKey_) return false;
        if (e1.isTombstone_ != e2.isTombstone_) return false;
        if (e1.timestamp_ != e2.timestamp_) return false;
        if (e1.rowData_ != e2.rowData_) return false;
        return true;
    }

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
