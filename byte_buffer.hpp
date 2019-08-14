/**
 * @file	byte_buffer.hpp
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/07/23
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_BYTEBUFFER_H__
#define __JCU_BYTEBUFFER_H__

#include <stdint.h>
#include <string.h>
#include <type_traits>

namespace jcu {

	class ByteBuffer
	{
	public:
		enum Endian {
			BIG_ENDIAN = 1,
			LITTLE_ENDIAN = 2,
		};

	private:
		Endian endian_;
		bool mode_; // false:read-only, true:write

		unsigned char* buffer_;
		size_t pos_;
		size_t limit_;

		bool flowed_;

		bool check_flow(int size);

		template<typename T>
		T getInteger();

		template<typename T>
		bool putInteger(T value);

	public:
		ByteBuffer(Endian endian = BIG_ENDIAN);

		void wrapReadMode(const void* buffer_, size_t size);
		void wrapWriteMode(void* buffer_, size_t size);

		bool flowed() const;
		uint8_t getUint8() {
			if (check_flow(sizeof(uint8_t)))
				return 0;
			return (uint8_t)buffer_[pos_++];
		}
		int8_t getInt8() {
			if (check_flow(sizeof(int8_t)))
				return 0;
			return (int8_t)buffer_[pos_++];
		}
		uint16_t getUint16() {
			return getInteger<uint16_t>();
		}
		int16_t getInt16() {
			return getInteger<uint16_t>();
		}
		uint32_t getUint32() {
			return getInteger<uint32_t>();
		}
		int32_t getInt32() {
			return getInteger<int32_t>();
		}
		uint64_t getUint64() {
			return getInteger<uint64_t>();
		}
		int64_t getInt64() {
			return getInteger<int64_t>();
		}
		bool putUint8(uint8_t value) {
			if (!mode_)
				return false;
			if (check_flow(sizeof(uint8_t)))
				return false;
			buffer_[pos_++] = value;
		}
		bool putInt8(int8_t value) {
			if (!mode_)
				return false;
			if (check_flow(sizeof(int8_t)))
				return false;
			buffer_[pos_++] = (unsigned char)value;
		}
		bool putUint16(uint16_t value) {
			return putInteger<uint16_t>(value);
		}
		bool putInt16(int16_t value) {
			return putInteger<int16_t>(value);
		}
		bool putUint32(uint32_t value) {
			return putInteger<uint32_t>(value);
		}
		bool putInt32(int32_t value) {
			return putInteger<int32_t>(value);
		}
		bool putUint64(uint64_t value) {
			return putInteger<uint64_t>(value);
		}
		bool putInt64(int64_t value) {
			return putInteger<int64_t>(value);
		}

		bool get(unsigned char* outbuf, size_t len);
		bool put(const unsigned char* inbuf, size_t len);

		size_t position() const {
			return pos_;
		}

		size_t remaining() const {
			return limit_ - pos_;
		}

		const unsigned char* readPtr() const {
			return &buffer_[pos_];
		}

		unsigned char* writePtr() {
			if (!mode_)
				return NULL;
			return &buffer_[pos_];
		}

		void increasePosition(size_t inc) {
			pos_ += inc;
		}

	}; // class ByteBuffer

} // namespace asymsecurefile

#endif // namespace __JCU_BYTEBUFFER_H__
