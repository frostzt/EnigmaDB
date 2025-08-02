//
// Created by frostzt on 8/1/2025.
//

#ifndef UUID_HPP
#define UUID_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>

namespace core::datatypes {
    struct UUID {
        std::string _typename = "UUID";
        std::array<uint8_t, 16> _value{};

        explicit UUID(const std::string &value): _value(stringToUuid(value)) {
        };

        explicit UUID(const std::array<uint8_t, 16> &value): _value(value) {
        };

        [[nodiscard]] std::string toString() const {
            return uuidToString(_value);
        }

        bool operator==(const UUID &other) const {
            return std::memcmp(this->_value.data(), other._value.data(), 16) == 0;
        }

        bool operator<(const UUID &other) const {
            return std::memcmp(this->_value.data(), other._value.data(), 16) < 0;
        }

        /**
         * Converts a UUID string into a 16-byte array representation.
         * The input string is expected to be in the standard UUID format (e.g., "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx").
         *
         * @param str The input string representing a UUID.
         * @return A 16-byte array corresponding to the UUID.
         * @throw std::runtime_error If the input string is not in a valid UUID format.
         */
        static std::array<uint8_t, 16> stringToUuid(const std::string &str) {
            std::array<uint8_t, 16> uuid{};

            int j = 0;
            for (size_t i = 0; i < str.size();) {
                if (str[i] == '-') {
                    i++;
                    continue;
                }

                // Check and make sure that the next byte can be accessed
                if (i + 2 > str.size() || j >= 16) {
                    throw std::runtime_error("Invalid UUID format!");
                }

                const std::string subString = str.substr(i, 2);
                uuid[j++] = std::stoi(subString, nullptr, 16);
                i += 2;
            }

            if (j != 16) throw std::runtime_error("Invalid UUID format!");
            return uuid;
        }

        /**
         * Converts a 16-byte array representation of a UUID to its standard string format.
         * The output string is in the format: "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
         *
         * @param uuid The input 16-byte array representing a UUID.
         * @return A string representing the UUID in the standard format.
         */
        static std::string uuidToString(const std::array<uint8_t, 16> &uuid) {
            std::ostringstream oss;
            for (size_t i = 0; i < 16; ++i) {
                if (i == 4 || i == 6 || i == 8 || i == 10) {
                    oss << '-';
                }
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid[i]);
            }
            return oss.str();
        }
    };
} // namespace core::datatypes

#endif //UUID_HPP
