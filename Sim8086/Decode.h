#pragma once

#include "Common.h"

#include <string>
#include <map>

#define OP_CODE(x) 

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
	{ 0b100010, "mov" },
};

u8 decode1(u8 data);
u8 decode2(u8 data, u8 word);
