#pragma once

#include "Common.h"

#include <string>
#include <map>

using decodeFunc = u8 (*)(mCodeItr &);

/*
 * Generic decoders
 */
u8 rmrDecoder(mCodeItr &data, string &regStr, string &addrStr);
u8 irmDecoder(mCodeItr &data, string &addrStr, string &immStr);

/*
 * MOV decoders
 */
u8 movDecode(mCodeItr &data);
u8 movrmrDecode(mCodeItr &data);
u8 movirmDecode(mCodeItr &data);
u8 movirDecode(mCodeItr &data);
u8 movmaDecode(mCodeItr &data);
u8 movamDecode(mCodeItr &data);

/*
 * Arithmetic decoders
 */
u8 arithrmrDecode(mCodeItr &data);
u8 arithirmDecode(mCodeItr &data);
u8 arithiaDecode(mCodeItr &data);


/* index: [register][W] */
const std::string registerNamesW[][2] =
{
	{ "al", "ax" }, // 000
	{ "cl", "cx" }, // 001
	{ "dl", "dx" }, // 010
	{ "bl", "bx" }, // 011
	{ "ah", "sp" }, // 100
	{ "ch", "bp" }, // 101
	{ "dh", "si" }, // 110
	{ "bh", "di" }  // 111
};

/*
 * index: [register][mod]
 * mod = 01 and 10 store the same strings, the only difference is the immediate, which is given from the instruction
 */
const std::string addressNamesMod[] =
{
	"bx + si",
	"bx + di",
	"bp + si",
	"bp + di",
	"si",
	"di",
	"bp", // aware this value for MOD = 00
	"bx",
};

constexpr u8 MOVRMR = 0b10001000;
constexpr u8 MOVIRM = 0b11000110;
constexpr u8 MOVIR  = 0b10110000;
constexpr u8 MOVMA  = 0b10100000;
constexpr u8 MOVAM  = 0b10100010;

constexpr u8 ARITHRMR = 0b00000000;
constexpr u8 ARITHIRM = 0b10000000;
constexpr u8 ARITHIA  = 0b00000100;


const std::map<u8, decodeFunc> opName =
{
	{ MOVRMR, movrmrDecode },
	{ MOVIRM, movirmDecode },
	{ MOVIR, movirDecode },
	{ MOVMA, movmaDecode },
	{ MOVAM, movamDecode },
	{ ARITHRMR, arithrmrDecode },
	{ ARITHIRM, arithirmDecode },
	{ ARITHIA, arithiaDecode },
};
