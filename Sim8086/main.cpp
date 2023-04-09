#include <iostream>
#include <format>
#include <fstream>

#include "Common.h"
#include "Decode.h"
#include "Cpu.h"


/**
 * This lovely macro decodes the jump instructions, without having to use labels in asm.
 * For nasm, one can write, e.g., jnz $-4, to refer to "jnz -6", so we need a constant offset of 2.
 * Also, we need to use signs (+-) for all cases, even 0
 */
#define JUMPINSTR(x) {++mCodePtr; \
	string sign = (s8)(*mCodePtr + 2) >= 0 ? "+" : ""; \
	cout << (x) << " $" << sign << to_string((s8)(*mCodePtr++ + 2)) << endl;\
	found = true;}


/*
 * masks to use when testing machine codes.
 * NOTE: we must check masks in order, i.e., longest mask first (only 1s)
 * else we might match with another instruction first
 */
constexpr u8 g_masks[] = { 0xff, 0xfe, 0xfc, 0xf0 };


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
		bool found = false;
		for (const auto &mask : g_masks)
		{
			const u8 mCodeMasked = *mCodePtr & mask;

			//cout << format("Looking for opcode: {:08b}, masked: {:08b}", *mCodePtr, *mCodePtr & mask) << endl;
			if (opName.contains(mCodeMasked))
			{
				// find the decoder function, e.g., mov, push etc.
				const decodeFunc decoder = opName.at(mCodeMasked);
				decoder(mCodePtr);
				found = true;
				break;
			}

			switch (mCodeMasked)
			{
			case JE:
				JUMPINSTR("je")
				break;
			case JL:
				JUMPINSTR("jl")
				break;
			case JLE:
				JUMPINSTR("jle")
				break;
			case JB:
				JUMPINSTR("jb")
				break;
			case JBE:
				JUMPINSTR("jbe")
				break;
			case JP:
				JUMPINSTR("jp")
				break;
			case JO:
				JUMPINSTR("jo")
				break;
			case JS:
				JUMPINSTR("js")
				break;
			case JNE:
				JUMPINSTR("jne")
				break;
			case JNL:
				JUMPINSTR("jnl")
				break;
			case JNLE:
				JUMPINSTR("jnle")
				break;
			case JNB:
				JUMPINSTR("jnb")
				break;
			case JNBE:
				JUMPINSTR("jnbe")
				break;
			case JNP:
				JUMPINSTR("jnp")
				break;
			case JNO:
				JUMPINSTR("jno")
				break;
			case JNS:
				JUMPINSTR("jns")
				break;
			case LOOP:
				JUMPINSTR("loop")
				break;
			case LOOPZ:
				JUMPINSTR("loopz")
				break;
			case LOOPNZ:
				JUMPINSTR("loopnz")
				break;
			case JCXZ:
				JUMPINSTR("jcxz")
				break;
			default:
				cout << "UNKOWN INSTRUCTION" << endl;
				break;
			}

			// do not check other masks if we found the instruction, else we might also match with something else
			if (found) break;
		}

		//getchar();
	}
}
