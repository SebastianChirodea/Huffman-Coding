#include "iterator.h"

HuffmanTreeIteratorPreorder::HuffmanTreeIteratorPreorder(const HuffmanTree & tree)
{
	constructor(tree);
}

void HuffmanTreeIterator::constructor(const HuffmanTree & tree)
{
	buildVector(tree.root);
	index = 0;
}

void HuffmanTreeIterator::next() { index++; }

HuffmanTreeIterator::NodeInfo HuffmanTreeIterator::getCurrent() { return nodesVector[index]; }

HuffmanTreeIterator::NodeInfo HuffmanTreeIterator::createNodeInfo(Byte byte, bool isLeaf, bool isEOF)
{
	NodeInfo nodeInfo;
	nodeInfo.byte = byte;
	nodeInfo.isLeaf = isLeaf;
	nodeInfo.isEOF = isEOF;
	return nodeInfo;
}

bool HuffmanTreeIterator::isValid() const { return index != nodesVector.size(); }

void HuffmanTreeIteratorPreorder::buildVector(const std::unique_ptr<HuffmanTree::Node> & node)
{
	if (node != nullptr)
	{
		bool isEOF = (node->frequency == EOF_FREQUENCY_IDENTIFIER);
		NodeInfo nodeInfo = createNodeInfo(node->byte, HuffmanTree::isLeaf(node), isEOF);
		nodesVector.push_back(nodeInfo);
		buildVector(node->leftBranch);
		buildVector(node->rightBranch);
	}
}

size_t HuffmanTreeIterator::nodesCount() { return nodesVector.size(); }