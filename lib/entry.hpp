#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <iostream>
#include <variant>
#include <unordered_map>

using Field = std::variant<int, std::string, double>;
using Row = std::unordered_map<std::string, Field>;

// TODO: Composite keys

class Entry {
public:
    std::string tableName;
    std::string primaryKey_;
    Row rowData_;
    bool isTombstone_;
    uint64_t timestamp_;

    bool operator<(const Entry &other) const;
    bool operator==(const Entry &other) const;
};

#endif // ENTRY_HPP
