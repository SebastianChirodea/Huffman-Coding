#pragma once
#include <map>

typedef char Byte;

class ByteFrequency
{
	std::map<Byte, int> frequency;

public:
	ByteFrequency() {}
	ByteFrequency(std::string text);
	~ByteFrequency() {}

	void compute(std::string text);
	void compute(Byte byte);
	const int operator[](Byte byte);
	std::map<Byte, int>::iterator begin();
	std::map<Byte, int>::iterator end();
};
