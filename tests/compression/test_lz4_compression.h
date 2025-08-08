//
// Created by frostzt on 8/9/2025.
//

#ifndef LSMTREE_TEST_LZ4_COMPRESSION_H
#define LSMTREE_TEST_LZ4_COMPRESSION_H

#include <vector>
#include <cstdint>

#include "lib/compression/lz_4_compressor.hpp"
#include "tests/test_abs.hpp"

namespace TESTS {
    class TestLZ4CompressorFullTrip final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[COMPRESSION][LZ4]";
        }

        [[nodiscard]] std::string name() const override {
            return "should compress and return data successfully";
        }

        [[nodiscard]] bool execute() const override {
            const std::vector<uint8_t> orig{'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
            std::vector<uint8_t> compressed{};
            std::vector<uint8_t> decompressed{};

            compression::LZ4Compressor compressor;
            compressor.compress(orig.data(), orig.size(), compressed);
            if (compressed.size() == 0) {
                return failTest("compression failed");
            }

            compressor.decompress(compressed.data(), compressed.size(), decompressed);
            if (decompressed != orig) {
                return failTest("decompression failed or did not match expected value");
            }

            return true;
        }
    };

    class TestLZ4CompressorEmptyInput final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[COMPRESSION][LZ4]";
        }

        [[nodiscard]] std::string name() const override {
            return "should be able to handle empty input";
        }

        [[nodiscard]] bool execute() const override {
            constexpr std::vector<uint8_t> empty{};
            std::vector<uint8_t> compressed{};
            std::vector<uint8_t> decompressed{};

            compression::LZ4Compressor compressor;
            compressor.compress(empty.data(), empty.size(), compressed);
            compressor.decompress(compressed.data(), compressed.size(), decompressed);
            if (decompressed != empty) {
                return failTest("decompression failed or did not match expected value");
            }

            return true;
        }
    };

    class TestLZ4CompressorWithHighlyCompressibleInput final : public TestCase {
        [[nodiscard]] std::string suiteName() const override {
            return "[COMPRESSION][LZ4]";
        }

        [[nodiscard]] std::string name() const override {
            return "should compress a highly compressible input";
        }

        [[nodiscard]] bool execute() const override {
            const std::vector<uint8_t> zeros(1000, 0);
            std::vector<uint8_t> compressed{};
            std::vector<uint8_t> decompressed{};

            compression::LZ4Compressor compressor;
            compressor.compress(zeros.data(), zeros.size(), compressed);
            if (compressed.size() == 0) {
                return failTest("compression failed");
            }

            if (compressed.size() >= zeros.size() + 4) {
                return failTest("compression against a highly compressible input did not return a compressed size");
            }

            compressor.decompress(compressed.data(), compressed.size(), decompressed);
            if (decompressed != zeros) {
                return failTest("decompression failed or did not match expected value");
            }

            return true;
        }
    };
} // namespace TESTS

#endif //LSMTREE_TEST_LZ4_COMPRESSION_H
