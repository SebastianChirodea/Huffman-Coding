#include <fstream>
#include "filetreebuilder.h"

using namespace std;

HuffmanTree FileHuffmanTreeBuilder::builtTree(string filePath)
{
	this->filePath = filePath;
	initializeQueue();
	if (queue.size() == ONE_BECAUSE_ONLY_EOF_WAS_PUSHED)
		return HuffmanTree();
	mergeAllTrees();
	return queue.top();
}

void FileHuffmanTreeBuilder::initializeQueue()
{
	computeFileBinaryFrequencies();
	pushBinaryTrees();
}

void FileHuffmanTreeBuilder::computeFileBinaryFrequencies()
{
	ifstream file;
	Byte byte;

	file.open(filePath, ios::binary | ios::in);
	while (true)
	{
		file.read(&byte, 1);
		if (file.eof()) break;
		frequency.compute(byte);
	}
	file.close();
}

void FileHuffmanTreeBuilder::pushBinaryTrees()
{
	pushEOFBitwise();
	for (auto pair : frequency)
	{
		HuffmanTree tree(pair.first, pair.second);
		queue.push(tree);
	}
}

void FileHuffmanTreeBuilder::pushEOFBitwise() 
{ 
	queue.push(HuffmanTree(NON_IMPORTANT_BYTE_BUT_NEEDED_FOR_CONSTRUCTOR, EOF_FREQUENCY_IDENTIFIER)); 
}

void FileHuffmanTreeBuilder::mergeAllTrees()
{
	while (queue.size() >= MINIMUM_NR_OF_TREES_TO_MERGE)
	{
		HuffmanTree firstTree(queue.top());
		queue.pop();
		HuffmanTree secondTree(queue.top());
		queue.pop();

		firstTree.merge(secondTree);
		queue.push(firstTree);
	}
}