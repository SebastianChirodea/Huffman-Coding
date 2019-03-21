#pragma once

#include <queue>
#include <string>
#include "tree.h"
#include "bytefreq.h"

class FileHuffmanTreeBuilder
{
	static const size_t MINIMUM_NR_OF_TREES_TO_MERGE = 2;
	static const Byte NON_IMPORTANT_BYTE_BUT_NEEDED_FOR_CONSTRUCTOR = 0;
	static const int EOF_FREQUENCY_IDENTIFIER = '\0';
	static const size_t ONE_BECAUSE_ONLY_EOF_WAS_PUSHED = 1;

	std::priority_queue<HuffmanTree, std::vector<HuffmanTree>, std::greater<HuffmanTree>> queue;
	std::string filePath;
	ByteFrequency frequency;

public:

	FileHuffmanTreeBuilder() {}
	HuffmanTree builtTree(std::string filePath);

private:
	void initializeQueue();

	void computeFileBinaryFrequencies();
	void pushBinaryTrees();
	void pushEOFBitwise();

	void mergeAllTrees();
};