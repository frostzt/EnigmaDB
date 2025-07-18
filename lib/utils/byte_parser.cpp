//
// Created by aiden on 7/17/2025.
//

#include "byte_parser.hpp"

namespace Utility {
	void ByteParser::writeUint16(std::vector<std::byte> &out, const uint16_t value) {
		out.push_back(static_cast<std::byte>(value & 0xFF)); // lower byte
		out.push_back(static_cast<std::byte>((value >> 8) & 0xFF)); // upper byte
	}

	uint16_t ByteParser::readUint16() {
		const auto lsb = static_cast<uint8_t>(this->data_[this->cursor_]);
		const auto msb = static_cast<uint8_t>(this->data_[this->cursor_ + 1]);
		this->cursor_ += 2;

		return static_cast<uint16_t>(lsb | (msb << 8));
	}

	void ByteParser::writeUint32(std::vector<std::byte> &out, const uint32_t value) {
		for (int i = 0; i < 4; i++) {
			out.push_back(static_cast<std::byte>((value >> 8 * i) & 0xFF));
		}
	}

	uint32_t ByteParser::readUint32() {
		uint32_t result = 0;
		for (int i = 0; i < 4; i++) {
			result |= static_cast<uint32_t>(static_cast<uint8_t>(this->data_[this->cursor_ + i])) << (i * 8);
		}

		this->cursor_ += 4;

		return result;
	}

	void ByteParser::writeUint64(std::vector<std::byte> &out, const uint64_t value) {
		for (int i = 0; i < 8; ++i) {
			out.push_back(static_cast<std::byte>((value >> 8 * i) & 0xFF));
		}
	}

	uint64_t ByteParser::readUint64() {
		uint64_t result = 0;
		for (int i = 0; i < 8; i++) {
			result |= static_cast<uint64_t>(static_cast<uint8_t>(this->data_[i + this->cursor_])) << (i * 8);
		}

		this->cursor_ += 8;

		return result;
	}

	void ByteParser::patchUint64(std::vector<std::byte> &out, const size_t offset, const uint64_t value) {
		for (int i = 0; i < 8; ++i) {
			out[offset + i] = static_cast<std::byte>((value >> 8 * i) & 0xFF);
		}
	}

	void ByteParser::writeString(std::vector<std::byte> &out, const std::string &s) {
		assert(s.size() <= UINT16_MAX);
		writeUint16(out, static_cast<uint16_t>(s.size()));
		out.insert(out.end(),
		           reinterpret_cast<const std::byte *>(s.data()),
		           reinterpret_cast<const std::byte *>(s.data() + s.size()));
	}

	std::string ByteParser::readString() {
		const uint16_t size = readUint16();

		std::string str(reinterpret_cast<const char *>(this->data_ + this->cursor_), size);
		this->cursor_ += size;

		return str;
	}

	void ByteParser::writeVariant(std::vector<std::byte> &out, const Utility::Field &value) {
		if (std::holds_alternative<int>(value)) {
			out.push_back(static_cast<std::byte>(FieldTag::INT)); // int flag
			const int v = std::get<int>(value);
			for (int i = 0; i < 4; i++) {
				out.push_back(static_cast<std::byte>((v >> (8 * i)) & 0xFF));
			}
		} else if (std::holds_alternative<std::string>(value)) {
			out.push_back(static_cast<std::byte>(FieldTag::STRING)); // std::string flag
			writeString(out, std::get<std::string>(value));
		} else if (std::holds_alternative<double>(value)) {
			out.push_back(static_cast<std::byte>(FieldTag::DOUBLE)); // double flag
			const double v = std::get<double>(value);
			uint8_t bytes[sizeof(double)];
			std::memcpy(bytes, &v, sizeof(double));
			out.insert(out.end(),
			           reinterpret_cast<const std::byte *>(&v),
			           reinterpret_cast<const std::byte *>(&v) + sizeof(double));
		}
	}

	std::optional<Field> ByteParser::readVariant() {
		const int typeFlag = static_cast<int>(this->data_[this->cursor_]);
		cursor_++;

		if (typeFlag == FieldTag::INT) {
			return static_cast<int>(readUint32());
		} else if (typeFlag == FieldTag::STRING) {
			return readString();
		} else if (typeFlag == FieldTag::DOUBLE) {
			double d;
			std::memcpy(&d, this->data_ + this->cursor_, sizeof(double));
			this->cursor_ += sizeof(double);
			return d;
		} else {
			return std::nullopt;
		}
	}

	std::byte ByteParser::readByte() {
		return this->data_[this->cursor_++];
	}

	void ByteParser::writeMagicBytes(std::vector<std::byte> &out) {
		out.insert(out.end(),
		           reinterpret_cast<const std::byte *>(magic.data()),
		           reinterpret_cast<const std::byte *>(magic.data() + magic.size())
		);
	}
}

