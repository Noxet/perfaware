#include "Decode.h"


u8 movrmrDecode(mCodeItr &data)
{
	const u8 d = (*data & 0x02) >> 1;
	const u8 w = *data & 0x01;
	++data;

	const u8 mod = (*data & 0xc0) >> 6;
	const u8 reg = (*data & 0x38) >> 3;
	const u8 rm  = (*data & 0x07);
	++data;

	string regStr{}, addrStr{}, dispStr{};

	regStr  = registerNamesW[reg][w];
	addrStr = "[" + addressNamesMod[rm];

	switch (mod)
	{
	case 0:

		break;
	case 1:
		// 8 bit displacement
		dispStr = to_string(*data);
		++data;
		break;
	case 2:
		{
			u16 disp = *data;
			++data;
			disp |= (*data << 8);
			++data;
			dispStr = to_string(disp);
		}
		break;
	case 3:
		addrStr = registerNamesW[rm][w];
		break;
	default:
		cout << "Not valid MOD value" << endl;
		return -1;
	}

	// add displacement, if it exists
	if (!dispStr.empty())
	{
		addrStr += " + " + dispStr + "]";
	}
	else if (mod != 3)
	{
		// only use address calculation for MOD != 3
		addrStr += "]";
	}

	if (d)
	{
		cout << format("mov {}, {}", regStr, addrStr) << endl;
	}
	else
	{
		cout << format("mov {}, {}", addrStr, regStr) << endl;
	}


#ifdef DEBUG
	cout << format("[mov rmr] - d: {:02x}\tw: {:02x}\tmod: {:02x}\treg: {:02x}\trm: {:02x}", d, w, mod, reg,
	               rm) << endl;
#endif

	return 0;
}


u8 movirmDecode(mCodeItr &data)
{
	// TODO: fix this func
	cout << "mov ";

	const u8 w = *data & 0x01;
	++data;

	const u8 mod = (*data & 0xc0) >> 6;
	const u8 rm  = *data & 0x07;
	++data;

	string regStr{}, addrStr{}, dispStr{};

	addrStr = "[" + addressNamesMod[rm];
	switch (mod)
	{
	case 0:
		break;
	case 1:
		// 8 bit displacement
		dispStr = to_string(*data);
		++data;
		break;
	case 2:
		{
			u16 disp = *data;
			++data;
			disp |= (*data << 8);
			dispStr = to_string(disp);
		}
		break;
	default:
		cout << "Not valid MOD value" << endl;
		return -1;
	}

	cout << format("mov {} {} TODO") << endl;

#ifdef DEBUG
	cout << format("[mov irm] - w: {:02x}\tmod: {:02x}", w, mod) << endl;
#endif

	return 0;
}


u8 movirDecode(mCodeItr &data)
{
	const u8 w   = (*data & 0x08) >> 3;
	const u8 reg = *data & 0x07;
	++data;

	u16 regVal{};
	if (w)
	{
		regVal = *data; // load lower 8 bits
		++data;
		regVal |= (*data << 8);
		++data;
	}
	else
	{
		regVal = *data;
		++data;
	}

	cout << "mov " << registerNamesW[reg][w] << ", " << regVal << endl;

#ifdef DEBUG
	cout << format("[mov ir] - w: {:02x}\treg: {:02x}\timmediate: {:02x}", w, reg, regVal) << endl;
#endif
	return 0;
}


u8 movmaDecode(mCodeItr &data)
{
	cout << "MOV MA" << endl;
	return 0;
}


u8 movamDecode(mCodeItr &data)
{
	cout << "MOV AM" << endl;
	return 0;
}
