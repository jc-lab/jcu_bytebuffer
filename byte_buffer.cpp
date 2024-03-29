/**
 * @file	byte_buffer.cpp
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/07/23
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include "byte_buffer.hpp"

namespace jcu {

	bool ByteBuffer::check_flow(int size) {
		if ((pos_ + size) > limit_) {
			flowed_ = true;
		}
		return flowed_;
	}

	ByteBuffer::ByteBuffer(Endian endian)
		: endian_(endian), buffer_(NULL), pos_(0), limit_(0), flowed_(false), mode_(false)
	{
	}

	void ByteBuffer::wrapReadMode(const void* buffer, size_t size)
	{
		mode_ = false;
		buffer_ = (unsigned char*)buffer;
		limit_ = size;
		pos_ = 0;
	}

	void ByteBuffer::wrapWriteMode(void* buffer, size_t size)
	{
		mode_ = true;
		buffer_ = (unsigned char*)buffer;
		limit_ = size;
		pos_ = 0;
	}

	bool ByteBuffer::flowed() const {
		return flowed_;
	}

	uint8_t ByteBuffer::getUint8() {
		if (check_flow(sizeof(uint8_t)))
			return 0;
		return (uint8_t)buffer_[pos_++];
	}
	int8_t ByteBuffer::getInt8() {
		if (check_flow(sizeof(int8_t)))
			return 0;
		return (int8_t)buffer_[pos_++];
	}
	uint16_t ByteBuffer::getUint16() {
		return getInteger<uint16_t>();
	}
	int16_t ByteBuffer::getInt16() {
		return getInteger<uint16_t>();
	}
	uint32_t ByteBuffer::getUint32() {
		return getInteger<uint32_t>();
	}
	int32_t ByteBuffer::getInt32() {
		return getInteger<int32_t>();
	}
	uint64_t ByteBuffer::getUint64() {
		return getInteger<uint64_t>();
	}
	int64_t ByteBuffer::getInt64() {
		return getInteger<int64_t>();
	}
	bool ByteBuffer::putUint8(uint8_t value) {
		if (!mode_)
			return false;
		if (check_flow(sizeof(uint8_t)))
			return false;
		buffer_[pos_++] = value;
		return true;
	}
	bool ByteBuffer::putInt8(int8_t value) {
		if (!mode_)
			return false;
		if (check_flow(sizeof(int8_t)))
			return false;
		buffer_[pos_++] = (unsigned char)value;
		return true;
	}
	bool ByteBuffer::putUint16(uint16_t value) {
		return putInteger<uint16_t>(value);
	}
	bool ByteBuffer::putInt16(int16_t value) {
		return putInteger<int16_t>(value);
	}
	bool ByteBuffer::putUint32(uint32_t value) {
		return putInteger<uint32_t>(value);
	}
	bool ByteBuffer::putInt32(int32_t value) {
		return putInteger<int32_t>(value);
	}
	bool ByteBuffer::putUint64(uint64_t value) {
		return putInteger<uint64_t>(value);
	}
	bool ByteBuffer::putInt64(int64_t value) {
		return putInteger<int64_t>(value);
	}

	template<typename T>
	T ByteBuffer::getInteger()
	{
		int i;
		std::make_unsigned<T>::type value = 0;
		if (check_flow(sizeof(T)))
			return 0;
		if (endian_ == BIG_ENDIAN)
		{
			for (i = 0; i < sizeof(T); i++) {
				value <<= 8;
				value |= (unsigned char)buffer_[pos_++];
			}
		} else {
			for (i = 0; i < sizeof(T); i++) {
				value |= ((std::make_unsigned<T>::type)((unsigned char)buffer_[pos_++])) << (i * 8);
			}
		}
		return (std::make_unsigned<T>::type)value;
	}

	template<typename T>
	bool ByteBuffer::putInteger(T value)
	{
		int i;
		if (!mode_)
			return false;
		if (check_flow(sizeof(T)))
			return false;
		if (endian_ == BIG_ENDIAN)
		{
			int shift = (sizeof(T) - 1) * 8;
			for (i = 0; i < sizeof(T); i++) {
				buffer_[pos_++] = (unsigned char)(value >> shift);
				shift -= 8;
			}
		} else {
			for (i = 0; i < sizeof(T); i++) {
				buffer_[pos_++] = (unsigned char)(value);
				value >>= 8;
			}
		}
		return true;
	}

	bool ByteBuffer::get(unsigned char* outbuf, size_t len) {
		if (check_flow(len))
			return false;
		memcpy(outbuf, &buffer_[pos_], len);
		pos_ += len;
		return true;
	}

	bool ByteBuffer::put(const unsigned char* inbuf, size_t len) {
		if (check_flow(len))
			return false;
		memcpy(&buffer_[pos_], inbuf, len);
		pos_ += len;
		return true;
	}

} // namespace asymsecurefile
