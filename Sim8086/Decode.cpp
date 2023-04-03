#include "Decode.h"


u8 rmrDecoder(mCodeItr &data, string &regStr, string &addrStr)
{
	const u8 d = (*data & 0x02) >> 1;
	const u8 w = *data & 0x01;
	++data;

	const u8 mod = (*data & 0xc0) >> 6;
	const u8 reg = (*data & 0x38) >> 3;
	const u8 rm  = (*data & 0x07);
	++data;

	string dispStr{};

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
		break;
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


#ifdef DEBUG
	cout << format("[rmr] - d: {:02x}\tw: {:02x}\tmod: {:02x}\treg: {:02x}\trm: {:02x}", d, w, mod, reg,
	               rm) << endl;
#endif

	return d;
}


u8 irmDecoder(mCodeItr &data, string &addrStr, string &immStr)
{
	// TODO: fix this func

	const u8 w = *data & 0x01;
	++data;

	const u8 mod   = (*data & 0xc0) >> 6;
	const u8 instr = (*data & 0x38) >> 3;
	const u8 rm    = *data & 0x07;
	++data;

	string dispStr{};

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
		if (w)
		{
			u8 imm = *data;
			++data;
			immStr = to_string(imm);
		}
		else
		{
			u16 imm = *data;
			++data;
			imm |= (*data << 8);
			++data;
			immStr = to_string(imm);
		}
		break;
	default:
		cout << "Not valid MOD value" << endl;
		break;
	}

	//cout << format("mov {} {} TODO") << endl;

#ifdef DEBUG
	cout << format("[irm] - w: {:02x}\tmod: {:02x}\tinstr: {:02x}\trm: {:02x}", w, mod, instr, rm) << endl;
#endif

	return 0;
}


u8 movrmrDecode(mCodeItr &data)
{
	string regStr{}, addrStr{};

	const u8 d = rmrDecoder(data, regStr, addrStr);

	if (d)
	{
		cout << format("mov {}, {}", regStr, addrStr) << endl;
	}
	else
	{
		cout << format("mov {}, {}", addrStr, regStr) << endl;
	}

	return 0;
}


u8 movirmDecode(mCodeItr &data)
{
	// TODO: call irmDecoder

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
