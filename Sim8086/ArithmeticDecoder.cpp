#include "Decode.h"


/**
 * GENERIC FUNCTIONS
 */

u8 arithirmDecode(mCodeItr &data)
{
	string addrStr{}, immStr{};
	const u8 instr = irmSDecoder(data, addrStr, immStr);
	Instr i{ instr };

	string instrStr{};
	switch (i)
	{
	case Instr::ADD:
		instrStr = "add";
		break;
	case Instr::SUB:
		instrStr = "sub";
		break;
	case Instr::CMP:
		instrStr = "cmp";
		break;
	default:
		instrStr = "UNKNOWN INSTRUCTION";
		break;
	}

	cout << format("{} {}, {}", instrStr, addrStr, immStr) << endl;
	return 0;
}


u8 arithrmrDecode(mCodeItr &data, const string &instr)
{
	string regStr{}, addrStr{};

	const u8 d = rmrDecoder(data, regStr, addrStr);

	if (d)
	{
		cout << format("{} {}, {}", instr, regStr, addrStr) << endl;
	}
	else
	{
		cout << format("{} {}, {}", instr, addrStr, regStr) << endl;
	}

	return 0;
}


/**
 * INSTRUCTION SPECIFIC FUNCTIONS
 */


u8 addrmrDecode(mCodeItr &data)
{
	arithrmrDecode(data, "add");
	return 0;
}


u8 addiaDecode(mCodeItr &data)
{
	string immStr{};
	const u8 w = iaDecoder(data, immStr);

	const string acc = w ? "ax" : "al";

	cout << format("add {}, {}", acc, immStr) << endl;
	return 0;
}


u8 subrmrDecode(mCodeItr &data)
{
	arithrmrDecode(data, "sub");
	return 0;
}


u8 subiaDecode(mCodeItr &data)
{
	string immStr{};
	const u8 w = iaDecoder(data, immStr);

	const string acc = w ? "ax" : "al";

	cout << format("sub {}, {}", acc, immStr) << endl;
	return 0;
}


u8 cmprmrDecode(mCodeItr &data)
{
	arithrmrDecode(data, "cmp");
	return 0;
}


u8 cmpiaDecode(mCodeItr &data)
{
	string immStr{};
	const u8 w = iaDecoder(data, immStr);

	const string acc = w ? "ax" : "al";

	cout << format("cmp {}, {}", acc, immStr) << endl;
	return 0;
}
