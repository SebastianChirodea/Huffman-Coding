#include <crtdbg.h>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../HuffZip/bytefreq.cpp"
#include "../HuffZip/tree.cpp"
#include "../HuffZip/compressor.cpp"
#include "../HuffZip/iterator.cpp"
#include "../HuffZip/filetreebuilder.cpp"
#include "../HuffZip/bitwriter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CompressorTests
{
	TEST_CLASS(CompressionTests)
	{
		HuffmanCompressor compressor;
		string filePathWitoutExtension;
		ifstream compressedFile;

		vector<Byte> expectedContent;
		vector<Byte> actualContent;

		void initializeActualContent(string filePath)
		{
			filePathWitoutExtension = filePath;
			compressAndOpen();
			readContent();
		}

		void compressAndOpen()
		{
			compressor.compress(filePathWitoutExtension + ".txt", filePathWitoutExtension + ".huf");
			compressedFile.open(filePathWitoutExtension + ".huf", ios::binary | ios::in);
		}

		void readContent()
		{
			ifstream sourceFile;
			Byte byteToBeRead;

			sourceFile.open(filePathWitoutExtension + ".huf", ios::binary | ios::in);

			while (true)
			{
				sourceFile.read(&byteToBeRead, 1);
				if (sourceFile.eof()) break;
				actualContent.push_back(byteToBeRead);
			}
			sourceFile.close();
		}

		void assertContentsAreEqual()
		{
			Assert::AreEqual(expectedContent.size(), actualContent.size());
			for (int i = 0; i < expectedContent.size(); i++)
				Assert::AreEqual(expectedContent[i], actualContent[i]);
		}

		TEST_METHOD_CLEANUP(clearContentsAndCloseFile)
		{
			expectedContent.clear();
			actualContent.clear();
			compressedFile.close();
		}

		TEST_METHOD(noCharactersTest)
		{
			initializeActualContent("D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\nochar");
			assertContentsAreEqual();
		}

		TEST_METHOD(oneCharacterTest)
		{
			initializeActualContent("D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\onechar");

			expectedContent.push_back(0xD8);
			expectedContent.push_back(0x40);
			expectedContent.push_back(0x5F);
			expectedContent.push_back(0xF8);
			
			assertContentsAreEqual();
		}

		TEST_METHOD(twoCharactersTest)
		{
			initializeActualContent("D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\twochar");

			expectedContent.push_back(0x36);
			expectedContent.push_back(0x1D);
			expectedContent.push_back(0x88);
			expectedContent.push_back(0x08);

			expectedContent.push_back(0x57);
			expectedContent.push_back(0xE0);

			assertContentsAreEqual();
		}

		TEST_METHOD(multipleScatteredCharacters)
		{
			initializeActualContent("D:\\Side Projects\\C++\\HuffZip\\CompressorTests\\testfiles\\compression\\multichar");

			expectedContent.push_back(0x59);
			expectedContent.push_back(0x60);
			expectedContent.push_back(0xDC);
			expectedContent.push_back(0xF7);

			expectedContent.push_back(0x4C);
			expectedContent.push_back(0x80);
			expectedContent.push_back(0x12);
			expectedContent.push_back(0x28);

			expectedContent.push_back(0x26);
			expectedContent.push_back(0xFF);
			expectedContent.push_back(0xC0);

			assertContentsAreEqual();
		}
	};

	TEST_CLASS(DecompressionTests)
	{

	};
}
