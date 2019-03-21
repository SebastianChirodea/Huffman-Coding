#pragma once
#include "tree.h"
#include <vector>

class HuffmanTreeIterator
{
public:
	struct NodeInfo
	{
		Byte byte;
		bool isLeaf;
		bool isEOF;
	};

private:
	int index;

protected:
	static const int EOF_FREQUENCY_IDENTIFIER = 0;

	std::vector<HuffmanTreeIterator::NodeInfo> nodesVector;

public:
	HuffmanTreeIterator() {}
	~HuffmanTreeIterator() {}

	void next();
	HuffmanTreeIterator::NodeInfo getCurrent();
	bool isValid() const;

	size_t nodesCount();

	static NodeInfo createNodeInfo(Byte byte, bool isLeaf, bool isEOF);

protected:
	virtual void constructor(const HuffmanTree & tree);
	virtual void buildVector(const std::unique_ptr<HuffmanTree::Node> & node) = 0;
};

class HuffmanTreeIteratorPreorder : public HuffmanTreeIterator
{
public:
	HuffmanTreeIteratorPreorder(const HuffmanTree & tree);
	~HuffmanTreeIteratorPreorder() {}

protected:
	void buildVector(const std::unique_ptr<HuffmanTree::Node> & node);
};