//
// Created by frostzt on 8/9/2025.
//

#include <vector>
#include <cstdint>

#include "catch2/catch_test_macros.hpp"
#include "lib/compression/lz_4_compressor.hpp"

TEST_CASE("lz4 should compress and return data successfully", "[COMPRESSION]") {
    const std::vector<uint8_t> orig{'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
    std::vector<uint8_t> compressed{};
    std::vector<uint8_t> decompressed{};

    compression::LZ4Compressor compressor;
    compressor.compress(orig.data(), orig.size(), compressed);
    REQUIRE(!compressed.empty());

    compressor.decompress(compressed.data(), compressed.size(), decompressed);
    REQUIRE(decompressed.size() == orig.size());
};

TEST_CASE("lz4 should be able to handle empty input", "[COMPRESSION]") {
    constexpr std::vector<uint8_t> empty{};
    std::vector<uint8_t> compressed{};
    std::vector<uint8_t> decompressed{};

    compression::LZ4Compressor compressor;
    compressor.compress(empty.data(), empty.size(), compressed);
    compressor.decompress(compressed.data(), compressed.size(), decompressed);
    REQUIRE(decompressed == empty);
};

TEST_CASE("lz4 should compress a highly compressible input", "[COMPRESSION]") {
    const std::vector<uint8_t> zeros(1000, 0);
    std::vector<uint8_t> compressed{};
    std::vector<uint8_t> decompressed{};

    compression::LZ4Compressor compressor;
    compressor.compress(zeros.data(), zeros.size(), compressed);
    REQUIRE(!compressed.empty());
    REQUIRE(compressed.size() < zeros.size());

    compressor.decompress(compressed.data(), compressed.size(), decompressed);
    REQUIRE(decompressed == zeros);
};
