#include "Decode.h"


u8 decode1(u8 data)
{
	const u8 opCode = (data & 0xfa) >> 2;
	const u8 d = (data & 0x02) >> 1;
	const u8 w = data & 0x01;


#ifdef DEBUG
	std::cout << format("opCode val: {:02x}\tD: {:02x}\tW: {:02x}", opCode, d, w) << std::endl;
#endif

	cout << opName.at(opCode) << " ";


	return w;
}


u8 decode2(u8 data, u8 word)
{
	const u8 reg = (data & 0x38) >> 3;
	const u8 rm = (data & 0x07);

#ifdef DEBUG
	cout << format("[decode 2] - data: {:02x}\treg: {:02x}\tR/M: {:02x}\tW: {:02x}", data, reg, rm, word) << endl;
#endif

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
