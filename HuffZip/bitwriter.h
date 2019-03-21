#pragma once
#include <vector>
#include <fstream>

typedef bool Bit;
typedef char Byte;

class BitFileWriter
{
	static const int NR_OF_BITS_IN_BYTE = 8;
	static const int BASE_TWO = 2;

	struct WorkingByte
	{
		Byte memory = 0;
		int capacity = 0;
	};

	WorkingByte workingByte;
	std::ofstream file;

public:
	void open(std::string filePath);
	void close();

	void writeBits(Bit bitToBeWritten);
	void writeBits(Byte byteToBeWritten);
	void writeBits(size_t size);
	void writeBits(const std::vector<Bit> & bits);
	void writeTheRemainingByte();

private:
	void recursivelyConvertByteToBits(std::vector<Bit> & bits, Byte byteToBeWritten);
	void addZerosUntilBitsAreEight(std::vector<Bit> & bits);
	void checkIfWorkingByteIsFull();
	void appendCurrentBitToWorkingByte(Bit bitToBeAppended);
	void shiftBitToNextFreePosition(Byte & convertedBitToByte);
};

