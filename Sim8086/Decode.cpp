#include "Decode.h"


u8 decode1(u8 data)
{
	const u8 opCode = (data & 0xfa) >> 2;
	std::cout << format("opCode val: {:02x}", opCode) << std::endl;

	cout << opName.at(opCode) << " ";

	return data & 0x01; // W
}


u8 decode2(u8 data, u8 word)
{
	const u8 reg = (data & 0x38) >> 3;
	const u8 rm = (data & 0x03);

	if (word == 1)
	{
		cout << registerNamesW1[rm] << ", " << registerNamesW1[reg] << endl;
	}
	else
	{
		cout << registerNamesW0[rm] << ", " << registerNamesW0[reg] << endl;
	}

	return 0;
}
