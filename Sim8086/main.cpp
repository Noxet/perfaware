#include <iostream>
#include <format>
#include <fstream>

#include "Common.h"
#include "Decode.h"


void test(mCodeItr &mCodePtr)
{
	cout << *mCodePtr << endl;
	++mCodePtr;
}


constexpr u8 g_masks[] = { 0xfc, 0xfe, 0xf0, 0xff };


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << format("Usage: {} <input binary file>", argv[0]) << endl;
		return -1;
	}

	const string filename = argv[1];

	ifstream inFile(filename, ios::binary);

	if (!inFile.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return -1;
	}

	inFile.seekg(0, inFile.end);
	const int length = inFile.tellg();
	inFile.seekg(0, inFile.beg);

	// read the machine code into vector
	std::vector<u8> mCode(length);
	inFile.read(reinterpret_cast<char*>(mCode.data()), length);


	auto mCodePtr = mCode.begin(); // keep track of where we are in array

	// force assembler to generate "original" 16-bit assembly
	cout << "bits 16" << endl;
	while (mCodePtr != mCode.end())
	{
		// check all potential op codes (with masking) to find a decoder
		for (const auto &mask : g_masks)
		{
			//cout << format("Looking for opcode: {:08b}", *mCodePtr & mask) << endl;
			if (opName.contains(*mCodePtr & mask))
			{
				// find the decoder function, e.g., mov, push etc.
				const decodeFunc decoder = opName.at(*mCodePtr & mask);
				decoder(mCodePtr);
				break;
			}
		}
		//getchar();
	}
}
