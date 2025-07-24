//
// Created by aiden on 7/17/2025.
//

#include "byte_utils.hpp"
#include "lib/entry/entry.hpp"

bool TestConvertingToByes::execute() const {
    const Entry entry("customer", "cid", {}, false);

    auto serialized = entry.serialize();

    auto de = entry.toHex();
    return true;
}
