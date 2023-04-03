#pragma once

#include "Common.h"

#include <string>
#include <map>

using decodeFunc = u8 (*)(mCodeItr &);

/*
 * MOV decoders
 */
u8 movDecode(mCodeItr &data);
u8 movrmrDecode(mCodeItr &data);
u8 movirmDecode(mCodeItr &data);
u8 movirDecode(mCodeItr &data);
u8 movmaDecode(mCodeItr &data);
u8 movamDecode(mCodeItr &data);

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

const std::map<u8, decodeFunc> opName =
{
	{ MOVRMR, movrmrDecode },
	{ MOVIRM, movirmDecode },
	{ MOVIR, movirDecode },
	{ MOVMA, movmaDecode },
	{ MOVAM, movamDecode },
};
