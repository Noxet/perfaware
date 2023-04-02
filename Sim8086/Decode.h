#pragma once

#include "Common.h"

#include <string>
#include <map>

using decodeFunc = u8 (*)(mCodeItr &);

// functions
u8 decode(mCodeItr &data);
u8 movDecode(mCodeItr &data);

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

constexpr u8 MOVRMR = 0b10001000;
constexpr u8 MOVIRM = 0b11000110;
constexpr u8 MOVIR  = 0b10110000;
constexpr u8 MOVMA  = 0b10100000;
constexpr u8 MOVAM  = 0b10100010;

const std::map<u8, decodeFunc> opName =
{
	{ MOVRMR, movDecode },
	{ MOVIRM, movDecode },
	{ MOVIR, movDecode },
	{ MOVMA, movDecode },
	{ MOVAM, movDecode },
};
