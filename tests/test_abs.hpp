//
// Created by frostzt on 7/16/2025.
//

#ifndef TEST_ABS_HPP
#define TEST_ABS_HPP

#include <string>

class TestCase {
public:
    virtual ~TestCase() = default;

    [[nodiscard]] virtual std::string suiteName() const = 0;
    [[nodiscard]] virtual std::string name() const = 0;
    [[nodiscard]] virtual bool execute() const = 0;
};

#endif //TEST_ABS_HPP
