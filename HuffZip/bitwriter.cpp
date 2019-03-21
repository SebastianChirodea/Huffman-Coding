#include "bitwriter.h"
using namespace std;

void BitFileWriter::open(std::string filePath) { file.open(filePath, ios::binary | ios::out); }

void BitFileWriter::close() { file.close(); }

void BitFileWriter::writeBits(Bit bitToBeWritten)
{
	vector<Bit> bits;
	bits.push_back(bitToBeWritten);
	writeBits(bits);
}

void BitFileWriter::writeBits(size_t size)
{
	Byte size_tConvertedToByte = (Byte)size;
	writeBits(size_tConvertedToByte);
}

void BitFileWriter::writeBits(Byte byte)
{

	vector<Bit> bits;
	recursivelyConvertByteToBits(bits, byte);
	addZerosUntilBitsAreEight(bits);
	writeBits(bits);
}

void BitFileWriter::recursivelyConvertByteToBits(vector<Bit> & bits, Byte byte)
{
	if (byte != 0)
	{
		recursivelyConvertByteToBits(bits, byte / BASE_TWO);
		bits.push_back(byte % BASE_TWO);
	}
}

void BitFileWriter::addZerosUntilBitsAreEight(vector<Bit> & bits)
{
	while (bits.size() < NR_OF_BITS_IN_BYTE)
		bits.insert(bits.begin(), 0);
}

void BitFileWriter::writeBits(const vector<Bit> & bits)
{
	for (int i = 0; i < bits.size(); i++)
	{
		appendCurrentBitToWorkingByte(bits[i]);
		checkIfWorkingByteIsFull();
	}
}

void BitFileWriter::checkIfWorkingByteIsFull()
{
	if (workingByte.capacity == NR_OF_BITS_IN_BYTE)
	{
		file.write(&workingByte.memory, 1);
		workingByte.memory = workingByte.capacity = 0;
	}
}

void BitFileWriter::appendCurrentBitToWorkingByte(Bit bitToBeAppended)
{
	Byte convertedBitToByte = (Byte)bitToBeAppended;
	shiftBitToNextFreePosition(convertedBitToByte);
	workingByte.memory = workingByte.memory | convertedBitToByte;
	workingByte.capacity++;
}

void BitFileWriter::shiftBitToNextFreePosition(Byte & convertedBit)
{
	convertedBit = convertedBit << (NR_OF_BITS_IN_BYTE - workingByte.capacity - 1);
}

void BitFileWriter::writeTheRemainingByte()
{
	while (workingByte.capacity != 0)
		writeBits((Bit)0);
}