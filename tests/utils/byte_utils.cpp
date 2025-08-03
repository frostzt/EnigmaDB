//
// Created by aiden on 7/17/2025.
//

#include "byte_utils.hpp"
#include "lib/entry/entry.hpp"
#include "tests/test_utils.hpp"

bool TestConvertingToByes::execute() const {
    const core::Entry entry("customer", core::Key{{makeField("cid")}}, {}, false);
    auto serialized = entry.serialize();
    auto de = entry.toHex();
    return true;
}
