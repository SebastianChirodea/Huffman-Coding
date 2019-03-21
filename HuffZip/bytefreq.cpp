#include "bytefreq.h"

ByteFrequency::ByteFrequency(std::string text) { compute(text); }

void ByteFrequency::compute(Byte byte) { frequency[byte]++; }

void ByteFrequency::compute(std::string text)
{
	for (Byte byte : text)
		frequency[byte]++;
}

const int ByteFrequency::operator[](Byte byte) { return frequency[byte]; }

std::map<Byte, int>::iterator ByteFrequency::begin() { return frequency.begin(); }

std::map<Byte, int>::iterator ByteFrequency::end() { return frequency.end(); }