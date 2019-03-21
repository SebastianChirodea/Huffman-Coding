#include "compressor.h"
#include "filetreebuilder.h"

using namespace std;

void HuffmanCompressor::compress(const string & sourceFilePath, const string & destinationFilePath = "")
{
	HuffmanTree tree;
	FileHuffmanTreeBuilder builder;

	tree = builder.builtTree(sourceFilePath);
	if (tree.isEmpty())
		return;

	bitWriter.open(destinationFilePath);

	memorizeCharacterCodes(tree);
	writeHuffmanTree(tree);
	writeContent(sourceFilePath);
	bitWriter.writeTheRemainingByte();

	bitWriter.close();
}

void HuffmanCompressor::memorizeCharacterCodes(const HuffmanTree & tree)
{
	HuffmanTreeIteratorPreorder iterator(tree);
	HuffmanTreeIterator::NodeInfo node;
	vector<Bit> binaryCode;

	binaryCode.push_back(LEFT_BRANCH);
	iterator.next();
	
	while (iterator.isValid())
	{
		node = iterator.getCurrent();
		updateBinaryCodes(binaryCode, node);
		iterator.next();
	}
}

void HuffmanCompressor::updateBinaryCodes(vector<Bit> & binaryCode, const HuffmanTreeIterator::NodeInfo & node)
{
	if (node.isLeaf)
	{
		memorizeByteCode(binaryCode, node);
		removeAllConsecutiveRightBranches(binaryCode);
		addRightBranchIfBinaryCodeNotEmpty(binaryCode);
	}
	else
		binaryCode.push_back(LEFT_BRANCH);
}

void HuffmanCompressor::memorizeByteCode(vector<Bit> & binaryCode, const HuffmanTreeIterator::NodeInfo & node)
{
	if (node.isEOF)
		eofCode = binaryCode;
	else
		compressionCode[node.byte] = binaryCode;
}

void HuffmanCompressor::removeAllConsecutiveRightBranches(vector<Bit> & binaryCode)
{
	while (canRemoveRightBranches(binaryCode))
		binaryCode.pop_back();
}

bool HuffmanCompressor::canRemoveRightBranches(vector<Bit> & binaryCode)
{
	if (binaryCode.empty())
		return false;
	if (binaryCode.back() == RIGHT_BRANCH)
		return true;
	return false;
}

void HuffmanCompressor::addRightBranchIfBinaryCodeNotEmpty(vector<Bit> & binaryCode)
{
	if (!binaryCode.empty())
	{
		binaryCode.pop_back();
		binaryCode.push_back(RIGHT_BRANCH);
	}
}

void HuffmanCompressor::writeHuffmanTree(const HuffmanTree & tree)
{
	HuffmanTreeIteratorPreorder iterator(tree);
	iterator.next();

	iterateTreeForWriting(iterator, LEFT_BRANCH); 

	bitWriter.writeBits(eofCode.size());
	bitWriter.writeBits(eofCode);
}

void HuffmanCompressor::iterateTreeForWriting(HuffmanTreeIteratorPreorder & iterator, Bit branch)
{
	HuffmanTreeIterator::NodeInfo currentNode = iterator.getCurrent();
	iterator.next();

	if (currentNode.isLeaf)
		writeNodeCharacteristicsIfNotEOF(iterator, currentNode, branch);
	else
	{
		bitWriter.writeBits(branch);
		bitWriter.writeBits(IS_NOT_LEAF);
		iterateTreeForWriting(iterator, LEFT_BRANCH);
	}
		
}

void HuffmanCompressor::writeNodeCharacteristicsIfNotEOF(HuffmanTreeIteratorPreorder & iterator, 
														 const HuffmanTreeIterator::NodeInfo & currentNode, Bit branch)
{
	if (!currentNode.isEOF)
	{
		bitWriter.writeBits(branch);
		bitWriter.writeBits(IS_LEAF);
		bitWriter.writeBits(currentNode.byte);
		continueIterationIfIteratorIsValid(iterator, branch);
	}
	else
		iterateTreeForWriting(iterator, RIGHT_BRANCH);
}

void HuffmanCompressor::continueIterationIfIteratorIsValid(HuffmanTreeIteratorPreorder & iterator, Bit branch)
{
	// A Huffman Tree always has odd numbers of nodes and ends with leaf
	if (iterator.isValid())
		iterateTreeForWriting(iterator, RIGHT_BRANCH);
}

void HuffmanCompressor::writeContent(const string & sourceFilePath)
{
	ifstream sourceFile;
	Byte byte;

	sourceFile.open(sourceFilePath, ios::binary | ios::in);

	while (true)
	{
		sourceFile.read(&byte, 1);
		if (sourceFile.eof()) break;
		bitWriter.writeBits(compressionCode[byte]);
	}
	
	bitWriter.writeBits(eofCode);
}