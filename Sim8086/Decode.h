#pragma once

#include "Common.h"

#include <string>
#include <map>

using decodeFunc = void (*)();

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


const std::map<u8, std::string> opName =
{
	{ 0b10001000, "mov" },
	{ 0b11000110, "mov" },
	{ 0b10110000, "mov" },
	{ 0b10100000, "mov" },
	{ 0b10100010, "mov" },

};

u8 decode(mCodeItr &data);
