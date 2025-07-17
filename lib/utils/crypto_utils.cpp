//
// Created by frostzt on 7/17/2025.
//

#include "crypto_utils.hpp"

namespace Utility {
	uint32_t computeCRC32(const std::vector<std::byte> &data, const size_t offset, size_t length) {
		if (length == 0) length = data.size() - offset;

		uint32_t crc = 0xFFFFFFFFU;
		for (size_t i = offset; i < offset + length; ++i) {
			const auto byte = static_cast<uint8_t>(data[i]);
			crc = (crc >> 8) ^ crc32Table[(crc ^ byte) & 0xFF];
		}
		return ~crc;
	}

	uint32_t computeCRC32(const std::byte *data, const size_t offset, const size_t length) {
		uint32_t crc = 0xFFFFFFFFU;

		for (size_t i = 0; i < length; ++i) {
			const auto byte = static_cast<uint8_t>(data[offset + i]);
			crc = (crc >> 8) ^ crc32Table[(crc ^ byte) & 0xFF];
		}

		return ~crc;
	}
}
