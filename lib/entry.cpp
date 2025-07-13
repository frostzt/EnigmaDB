//
// Created by frostzt on 7/13/25.
//

#include "entry.hpp"

bool Entry::operator<(const Entry &other) const {
    return primaryKey_ < other.primaryKey_;
}

bool Entry::operator==(const Entry &other) const {
    return primaryKey_ == other.primaryKey_;
}
