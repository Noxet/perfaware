#include <iostream>
#include <format>
#include <fstream>
#include <vector>

#include "Common.h"
#include "Decode.h"


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << format("Usage: {} <input binary file>", argv[0]) << endl;
		return -1;
	}

	const string filename = argv[1];

	cout << "filename: " << filename << endl;

	ifstream inFile(filename, ios::binary);

	if (!inFile.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return -1;
	}

	u8 byte1;
	u8 byte2;
	inFile.read((char*)&byte1, sizeof(byte1));
	inFile.read((char*)&byte2, sizeof(byte2));


	u8 w = decode1(byte1);
	decode2(byte2, w);
}
