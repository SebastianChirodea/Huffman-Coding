#pragma once
#include <map>
#include "iterator.h"
#include "bitwriter.h"

class HuffmanCompressor
{
	static const Bit LEFT_BRANCH = 0;
	static const Bit RIGHT_BRANCH = 1;

	static const bool IS_LEAF = true;
	static const bool IS_NOT_LEAF = false;

	BitFileWriter bitWriter;
	std::map<Byte, std::vector<Bit>> compressionCode;
	std::vector<Bit> eofCode;

public:
	HuffmanCompressor() {}
	~HuffmanCompressor() {}

	void compress(const std::string & sourceFilePath, const std::string & destinationFilePath);

private:
	void memorizeCharacterCodes(const HuffmanTree & tree);
	void updateBinaryCodes(std::vector<Bit> & binaryCode, const HuffmanTreeIterator::NodeInfo & node);
	void memorizeByteCode(std::vector<Bit> & binaryCode, const HuffmanTreeIterator::NodeInfo & node);
	void removeAllConsecutiveRightBranches(std::vector<Bit> & binaryCode);
	bool canRemoveRightBranches(std::vector<Bit> & binaryCode);
	void addRightBranchIfBinaryCodeNotEmpty(std::vector<Bit> & binaryCode);

	void writeHuffmanTree(const HuffmanTree & tree);
	void iterateTreeForWriting(HuffmanTreeIteratorPreorder & iterator, Bit branch);
	void writeNodeCharacteristicsIfNotEOF(HuffmanTreeIteratorPreorder & iterator, 
										  const HuffmanTreeIterator::NodeInfo & currentNode, Bit branch);
	void continueIterationIfIteratorIsValid(HuffmanTreeIteratorPreorder & iterator, Bit branch);
	
	void writeContent(const std::string & sourceFilePath);
};
