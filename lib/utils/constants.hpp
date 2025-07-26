//
// Created by aiden on 7/26/2025.
//

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr size_t operator"" _MB(const unsigned long long int mb) {
    return mb * 1024 * 1024;
}

constexpr size_t operator"" _KB(const unsigned long long int kb) {
    return kb * 1024;
}

#endif //CONSTANTS_HPP
