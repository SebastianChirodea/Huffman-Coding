#include "stdafx.h"
#include "CppUnitTest.h"
#include "../HuffZip/tree.cpp"
#include "../HuffZip/iterator.cpp"
#include "../HuffZip/filetreebuilder.cpp"
#include "../HuffZip/bytefreq.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HuffmanBinaryTreeTests
{
	static const bool IS_LEAF = true;
	static const bool IS_NOT_LEAF = false;

	static const bool IS_EOF = true;
	static const bool IS_NOT_EOF = false;

	vector<HuffmanTreeIterator::NodeInfo> expectedResult;

	void addToExpectedResults(Byte expectedByte, bool isLeaf, bool isEOF)
	{
		HuffmanTreeIterator::NodeInfo nodeInfo = HuffmanTreeIterator::createNodeInfo(expectedByte, isLeaf, isEOF);
		expectedResult.push_back(nodeInfo);
	}

	void assertNodeInfosAreEqual(const HuffmanTreeIterator::NodeInfo & expected, const HuffmanTreeIterator::NodeInfo & actual)
	{
		if (expected.isLeaf && !expected.isEOF)
			Assert::AreEqual(expected.byte, actual.byte);
		Assert::AreEqual(expected.isLeaf, actual.isLeaf);
		Assert::AreEqual(expected.isEOF, actual.isEOF);
	}

	void assertIteratorMatchesExpectedResult(const HuffmanTree & tree)
	{
		HuffmanTreeIteratorPreorder iterator(tree);
		size_t index = 0;

		while (iterator.isValid())
		{
			assertNodeInfosAreEqual(expectedResult[index], iterator.getCurrent());
			iterator.next();
			index++;
		}

		Assert::AreEqual(expectedResult.size(), index);
	}

	TEST_CLASS(RuleOfFiveTests)
	{
		HuffmanTree functionThatReturnsRValue(Byte byte, int frequency)
		{
			return HuffmanTree(byte, frequency);
		}

		TEST_METHOD_CLEANUP(clearExpectedResults) { expectedResult.clear(); }

		TEST_METHOD(destructor)
		{
			HuffmanTree stackTree('a', 3);
			HuffmanTree * heapTree = new HuffmanTree('b', 5);
			free(heapTree);
		}

		TEST_METHOD(copyConstructor)
		{
			HuffmanTree copiedTree('a', 3);
			HuffmanTree tree(copiedTree);

			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(tree);
		}

		TEST_METHOD(copyAssignmentOperator)
		{
			HuffmanTree firstTree('a', 3);
			HuffmanTree firstCopiedTree = firstTree;

			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(firstCopiedTree);

			HuffmanTree secondTree('b', 10);
			HuffmanTree secondCopiedTree('c', 5);

			assertIteratorMatchesExpectedResult(firstTree);

			expectedResult.clear();
			addToExpectedResults('b', IS_LEAF, IS_NOT_EOF);

			firstCopiedTree = secondCopiedTree = secondTree;

			assertIteratorMatchesExpectedResult(firstCopiedTree);
			assertIteratorMatchesExpectedResult(secondCopiedTree);
		}

		TEST_METHOD(moveConstructor)
		{
			HuffmanTree tree(functionThatReturnsRValue('a', 3));
			HuffmanTree expectedTree('a', 3);

			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(tree);
		}

		TEST_METHOD(moveAssignmentOperator)
		{
			HuffmanTree firstTree = functionThatReturnsRValue('a', 3);
			HuffmanTree secondTree('c', 5);

			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);
			assertIteratorMatchesExpectedResult(firstTree);

			expectedResult.clear();
			addToExpectedResults('b', IS_LEAF, IS_NOT_EOF);

			firstTree = secondTree = functionThatReturnsRValue('b', 10);

			assertIteratorMatchesExpectedResult(firstTree);
			assertIteratorMatchesExpectedResult(secondTree);
		}
	};

	TEST_CLASS(MergeAndIteratorTests)
	{
		TEST_METHOD_CLEANUP(clearExpectedResults) { expectedResult.clear(); }

		TEST_METHOD(oneLevelTest)
		{
			HuffmanTree firstTree('a', 3);
			HuffmanTree secondTree('b', 5);

			//For non-leaf nodes byte doesn't matter, so I put \0 as convention
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('b', IS_LEAF, IS_NOT_EOF);

			firstTree.merge(secondTree);
		}

		TEST_METHOD(oneLevelTwoLevelTest)
		{
			HuffmanTree firstTree('f', 9);
			HuffmanTree secondTree('r', 1);
			HuffmanTree thirdTree('k', 12);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('f', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('r', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('k', IS_LEAF, IS_NOT_EOF);

			firstTree.merge(secondTree);
			firstTree.merge(thirdTree);
		}

		TEST_METHOD(twoLevelsTests)
		{
			HuffmanTree firstTree(' ', 200);
			HuffmanTree secondTree('@', 104);
			HuffmanTree thirdTree('3', 123);
			HuffmanTree forthTree('\n', 99);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults(' ', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('@', IS_LEAF, IS_NOT_EOF);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('3', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('\n', IS_LEAF, IS_NOT_EOF);

			firstTree.merge(secondTree);
			thirdTree.merge(forthTree);
			firstTree.merge(thirdTree);
		}
	};

	TEST_CLASS(FileHuffmanTreeBuilderTests)
	{
		FileHuffmanTreeBuilder builder;

		TEST_METHOD_CLEANUP(clearExpectedResults) { expectedResult.clear(); }

		TEST_METHOD(noCharactersTest)
		{
			std::string sourceFilePath = "D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\nochar.txt";
			HuffmanTree tree = builder.builtTree(sourceFilePath);

			assertIteratorMatchesExpectedResult(tree);
		}

		TEST_METHOD(oneCharacterTest)
		{
			std::string sourceFilePath = "D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\onechar.txt";
			HuffmanTree tree = builder.builtTree(sourceFilePath);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_LEAF, IS_EOF);
			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(tree);
		}

		TEST_METHOD(twoCharactersTest)
		{
			std::string sourceFilePath = "D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\twochar.txt";
			HuffmanTree tree = builder.builtTree(sourceFilePath);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_LEAF, IS_EOF);
			addToExpectedResults('a', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('b', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(tree);
		}

		TEST_METHOD(multipleScatteredCharacters)
		{
			std::string sourceFilePath = "D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\multichar.txt";
			HuffmanTree tree = builder.builtTree(sourceFilePath);

			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('e', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_NOT_LEAF, IS_NOT_EOF);
			addToExpectedResults('\0', IS_LEAF, IS_EOF);
			addToExpectedResults('s', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults('t', IS_LEAF, IS_NOT_EOF);
			addToExpectedResults(' ', IS_LEAF, IS_NOT_EOF);

			assertIteratorMatchesExpectedResult(tree);
		}
	};
}