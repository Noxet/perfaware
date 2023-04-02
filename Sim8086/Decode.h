#pragma once

#include "Common.h"

#include <string>
#include <map>

#define OP_CODE(x) 

const std::string registerNamesW0[] =
{
	"al", // 000
	"cl", // 001
	"dl", // 010
	"bl", // 011
	"ah", // 100
	"ch", // 101
	"dh", // 110
	"bh"  // 111
};

const std::string registerNamesW1[] =
{
	"ax", // 000
	"cx", // 001
	"dx", // 010
	"bx", // 011
	"sp", // 100
	"bp", // 101
	"si", // 110
	"di"  // 111
};

const std::map<u8, std::string> opName =
{
	{ 0b100010, "mov" },
};

u8 decode1(u8 data);
u8 decode2(u8 data, u8 word);
