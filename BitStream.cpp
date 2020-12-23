#include "BitStream.h"

/**
 * InputBitStream
 */
void InputBitStream::ReadBits(uint8_t& outData, size_t inBitCount) {
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount) {
		outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	outData &= (~(0x00ff << inBitCount));
	mBitHead += inBitCount;
}

void InputBitStream::ReadBits(void* outData, size_t inBitCount) {
	uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);

	while (inBitCount > 8) {
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		ReadBits(*destByte, inBitCount);
	}
}

/**
 * OutputBitStream
 */
void OutputBitStream::WriteBits(uint8_t inData, size_t inBitCount) {
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
	if (nextBitHead > mBitCapacity) {
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}

	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;

	if (bitsFreeThisByte < inBitCount) {
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}

void OutputBitStream::WriteBits(const void* inData, size_t inBitCount) {
	const uint8_t* srcByte = static_cast<const uint8_t*>(inData);

	while (inBitCount > 8) {
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		WriteBits(*srcByte, inBitCount);
	}
}

void OutputBitStream::ReallocBuffer(uint32_t inNewBitCapacity) {
	mBuffer = static_cast<uint8_t*>(std::realloc(mBuffer, 1 + ((inNewBitCapacity - 1) / 8)));
	if (mBuffer == nullptr) {
		// TODO: 메모리 할당 실패 시 에러 처리
	}
	mBitCapacity = inNewBitCapacity;
}