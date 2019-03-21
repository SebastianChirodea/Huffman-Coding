#pragma once
#include <string>

typedef char Byte;

class HuffmanTree
{
	friend class HuffmanTreeIterator;
	friend class HuffmanTreeIteratorPreorder;

	struct Node
	{
		Byte byte;
		int frequency;
		std::unique_ptr<Node> leftBranch;
		std::unique_ptr<Node> rightBranch;
	};

	std::unique_ptr<Node> root;

public:
	HuffmanTree();
	HuffmanTree(Byte byte, int frequency);

	HuffmanTree(const HuffmanTree & tree);
	HuffmanTree(HuffmanTree && tree);
	~HuffmanTree() {}

	HuffmanTree & operator=(const HuffmanTree & tree);
	HuffmanTree & operator=(HuffmanTree && tree);

	void merge(const HuffmanTree & tree);
	int frequency() const;
	bool isEmpty() const;

private:
	void recursiveConstructor(std::unique_ptr<Node> & currentNode, const std::unique_ptr<Node> & copiedNode);
	void copyAndSwap(const HuffmanTree tree);

	void createNode(std::unique_ptr<Node> & node, Byte byte, int frequency);
	static bool isLeaf(const std::unique_ptr<HuffmanTree::Node> & node);
};

inline bool operator>(const HuffmanTree & lhs, const HuffmanTree & rhs) { return lhs.frequency() > rhs.frequency(); }