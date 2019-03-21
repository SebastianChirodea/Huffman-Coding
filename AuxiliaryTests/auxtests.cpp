#include "stdafx.h"
#include "CppUnitTest.h"
#include "../HuffZip/bytefreq.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AuxiliaryTests
{
	TEST_CLASS(CharacterFrequencyTests)
	{
	public:

		TEST_METHOD(computeOnlyLetters)
		{
			ByteFrequency frequency("ThisIsATextWithOnlyLetters");
			Assert::AreEqual(2, frequency['T']);
			Assert::AreEqual(4, frequency['t']);
			Assert::AreEqual(0, frequency['w']);
			Assert::AreEqual(0, frequency['.']);
			Assert::AreEqual(0, frequency['3']);
		}

		TEST_METHOD(computeOnlyNumbers)
		{
			ByteFrequency frequency("12231320000789");
			Assert::AreEqual(2, frequency['1']);
			Assert::AreEqual(3, frequency['2']);
			Assert::AreEqual(4, frequency['0']);
			Assert::AreEqual(0, frequency['-']);
			Assert::AreEqual(0, frequency['a']);
		}

		TEST_METHOD(computeOnlySymbols)
		{
			ByteFrequency frequency("*@##@.--';@.(&$@@");
			Assert::AreEqual(5, frequency['@']);
			Assert::AreEqual(2, frequency['.']);
			Assert::AreEqual(0, frequency['8']);
			Assert::AreEqual(0, frequency[' ']);
			Assert::AreEqual(0, frequency['f']);
		}

		TEST_METHOD(computeSpecialCharacters)
		{
			ByteFrequency frequency("\n\n\b\t\n\t\t\b");
			Assert::AreEqual(3, frequency['\n']);
			Assert::AreEqual(3, frequency['\t']);
			Assert::AreEqual(0, frequency['1']);
			Assert::AreEqual(0, frequency['(']);
			Assert::AreEqual(0, frequency['f']);
		}

		TEST_METHOD(computeMixed)
		{
			ByteFrequency frequency("This is mixed test.\nIt contains numbers like 123 and spaces.");
			Assert::AreEqual(5, frequency['i']);
			Assert::AreEqual(9, frequency[' ']);
			Assert::AreEqual(1, frequency['\n']);
			Assert::AreEqual(1, frequency['1']);
			Assert::AreEqual(0, frequency['@']);
			Assert::AreEqual(0, frequency['R']);
		}

		TEST_METHOD(computeConsecutively)
		{
			ByteFrequency frequency("\n\n\b\t\n\t\t\b");
			frequency.compute("*@##@.--';@.(&$@@");
			frequency.compute("This is mixed test.\nIt contains numbers like 123 and spaces.");
			Assert::AreEqual(5, frequency['@']);
			Assert::AreEqual(3, frequency['\t']);
		}

		TEST_METHOD(computeOneCharacterAtATime)
		{
			ByteFrequency frequency;
			frequency.compute('a');
			frequency.compute('b');
			frequency.compute('a');
			frequency.compute('c');

			Assert::AreEqual(2, frequency['a']);
			Assert::AreEqual(1, frequency['b']);
			Assert::AreEqual(1, frequency['c']);
		}

	};
}