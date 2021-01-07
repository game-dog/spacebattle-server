#ifndef __BIT_STREAM_H__
#define __BIT_STREAM_H__

#include <cstdint>
#include <cstdlib>
#include <string>
#include <algorithm>

class BitStream {
public:
	virtual void Serialize(const void* ioData, uint32_t inByteCount) = 0;
	virtual bool IsInput() const = 0;
};

class InputBitStream {
public:
	InputBitStream(uint8_t* inBuffer, uint32_t inBitCount) :
		mBuffer(inBuffer),
		mBitCapacity(inBitCount),
		mBitHead(0) {}
	~InputBitStream() {
		if (mBuffer != nullptr) {
			// std::free(mBuffer);
		}
	}

	const uint8_t* GetBufferPtr() const { return mBuffer; }
	uint32_t GetRemainingBitCount() { return mBitCapacity - mBitHead; }

	void ReadBits(uint8_t& outData, size_t inBitCount);
	void ReadBits(void* outData, size_t inBitCount);
	void ReadBytes(void* outData, uint32_t inByteCount) {
		ReadBits(outData, inByteCount << 3);
	}

	template<typename T>
	void Read(void* outData, uint32_t inBitCount = sizeof(T) * 8) {
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Read only supports primitive data types");

		ReadBits(&outData, inBitCount);
	}

	void Read(bool& outData) { ReadBits(&outData, 1); }
	void Read(std::string& inString) {

	}

private:
	uint8_t* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
	//bool     mIsBufferOwner;
};

class OutputBitStream {
public:
	OutputBitStream() :
		mBitHead(0),
		mBuffer(nullptr) {
		ReallocBuffer(256);
	}
	~OutputBitStream() {
		if (mBuffer != nullptr) {
			std::free(mBuffer);
		}
	}

	const uint8_t* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetByteLength() const { return (mBitHead + 7) >> 3; }

	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);

	void WriteBytes(const void* inData, size_t inByteCount) {
		WriteBits(inData, inByteCount << 3);
	}

	template<typename T>
	void Write(T inData, size_t inBitCount = sizeof(T) * 8) {
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		WriteBits(&inData, inBitCount);
	}

	void Write(bool inData) { WriteBits(&inData, 1); }
	void Write(const std::string& inString) {
		uint32_t elementCount = static_cast<uint32_t>(inString.size());
		Write(elementCount);
		for (const auto& element : inString) {
			Write(element);
		}
	}

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);

	uint8_t* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};

#endif