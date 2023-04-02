#include "Decode.h"


u8 movDecode(mCodeItr &data)
{
	const u8 opCode = (*data & 0xfa);
	const u8 d      = (*data & 0x02) >> 1;
	const u8 w      = *data & 0x01;

	++data;

#ifdef DEBUG
	std::cout << format("opCode val: {:02x}\tD: {:02x}\tW: {:02x}", opCode, d, w) << std::endl;
#endif

	cout << "mov ";

	const u8 reg = (*data & 0x38) >> 3;
	const u8 rm  = (*data & 0x07);

#ifdef DEBUG
	cout << format("[decode 2] - data: {:02x}\treg: {:02x}\tR/M: {:02x}\tW: {:02x}", *data, reg, rm, w) << endl;
#endif


	cout << registerNamesW[rm][w] << ", " << registerNamesW[reg][w] << endl;

	++data;

	return 0;
}
