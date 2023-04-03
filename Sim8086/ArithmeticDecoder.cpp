#include "Decode.h"


u8 arithrmrDecode(mCodeItr &data)
{
	string regStr{}, addrStr{};

	const u8 d = rmrDecoder(data, regStr, addrStr);

	if (d)
	{
		cout << format("add {}, {}", regStr, addrStr) << endl;
	}
	else
	{
		cout << format("add {}, {}", addrStr, regStr) << endl;
	}

	return 0;
}


u8 arithirmDecode(mCodeItr &data)
{
	string addrStr{}, immStr{};
	const u8 instr = irmDecoder(data, addrStr, immStr);

	cout << format("add {}, {}", addrStr, immStr) << endl;
	return 0;
}


u8 arithiaDecode(mCodeItr &data)
{
	cout << "arith ia" << endl;
	return 0;
}
