#include <memory>
#include "tree.h"

using namespace std;

HuffmanTree::HuffmanTree() { root = nullptr; }

HuffmanTree::HuffmanTree(Byte byte, int frequency)
{
	createNode(root, byte, frequency);
}

HuffmanTree::HuffmanTree(const HuffmanTree & tree)
{
	recursiveConstructor(this->root, tree.root);
}

void HuffmanTree::recursiveConstructor(unique_ptr<Node> & currentNode, const unique_ptr<Node> & copiedNode)
{
	if (copiedNode == nullptr)
		currentNode = nullptr;
	else
	{
		createNode(currentNode, copiedNode->byte, copiedNode->frequency);
		recursiveConstructor(currentNode->leftBranch, copiedNode->leftBranch);
		recursiveConstructor(currentNode->rightBranch, copiedNode->rightBranch);
	}
}

void HuffmanTree::createNode(unique_ptr<Node> & node, Byte byte, int frequency)
{
	unique_ptr<Node> auxNode = make_unique<Node>();
	swap(node, auxNode);
	node->byte = byte;
	node->frequency = frequency;
	node->leftBranch = nullptr;
	node->rightBranch = nullptr;
}

HuffmanTree & HuffmanTree::operator=(const HuffmanTree & tree)
{
	this->copyAndSwap(tree);
	return *this;
}

void HuffmanTree::copyAndSwap(HuffmanTree tree)
{
	swap(this->root, tree.root);
}

HuffmanTree::HuffmanTree(HuffmanTree && tree)
{
	swap(this->root, tree.root);
}

HuffmanTree & HuffmanTree::operator=(HuffmanTree && tree)
{
	swap(this->root, tree.root);
	return *this;
}

void HuffmanTree::merge(const HuffmanTree & tree)
{
	unique_ptr<Node> newRoot = std::make_unique<Node>();
	HuffmanTree treeCopy(tree);

	newRoot->frequency = this->frequency() + tree.frequency();
	swap(newRoot->leftBranch, this->root);
	swap(newRoot->rightBranch, treeCopy.root);

	swap(this->root, newRoot);
}

int HuffmanTree::frequency() const { return root->frequency; }

bool HuffmanTree::isLeaf(const unique_ptr<HuffmanTree::Node> & node)
{
	return node->leftBranch == nullptr && node->rightBranch == nullptr;
}

bool HuffmanTree::isEmpty() const { return root == nullptr; }