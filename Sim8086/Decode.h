#pragma once

#include "Common.h"

#include <string>
#include <map>

using decodeFunc = u8 (*)(mCodeItr &);

/*
 * Generic decoders
 */
u8 rmrDecoder(mCodeItr &data, string &regStr, string &addrStr);
u8 irmDecoder(mCodeItr &data, string &addrStr, string &immStr);  // immediate to register/memory
u8 irmSDecoder(mCodeItr &data, string &addrStr, string &immStr); // immediate to register/memory (sign-ext bit)
u8 iaDecoder(mCodeItr &data, string &immStr);

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
u8 arithirmDecode(mCodeItr &data);
u8 arithrmrDecode(mCodeItr &data, const string &instr);

u8 addrmrDecode(mCodeItr &data);
u8 addiaDecode(mCodeItr &data);
u8 subrmrDecode(mCodeItr &data);
u8 subiaDecode(mCodeItr &data);
u8 cmprmrDecode(mCodeItr &data);
u8 cmpiaDecode(mCodeItr &data);


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

enum class Instr
{
	ADD = 0,
	UN1,
	ADC,
	SBB,
	UN4,
	SUB,
	UN6,
	CMP,
};

constexpr u8 MOVRMR = 0b10001000;
constexpr u8 MOVIRM = 0b11000110;
constexpr u8 MOVIR  = 0b10110000;
constexpr u8 MOVMA  = 0b10100000;
constexpr u8 MOVAM  = 0b10100010;

/* Arithmetic */
constexpr u8 ARITHIRM = 0b10000000;
constexpr u8 ADDRMR   = 0b00000000;
constexpr u8 ADDIA    = 0b00000100;
constexpr u8 SUBRMR   = 0b00101000;
constexpr u8 SUBIA    = 0b00101100;
constexpr u8 CMPRMR   = 0b00111000;
constexpr u8 CMPIA    = 0b00111100;

/* Conditional jumps */
constexpr u8 JE     = 0b01110100;
constexpr u8 JL     = 0b01111100;
constexpr u8 JLE    = 0b01111110;
constexpr u8 JB     = 0b01110010;
constexpr u8 JBE    = 0b01110110;
constexpr u8 JP     = 0b01111010;
constexpr u8 JO     = 0b01110000;
constexpr u8 JS     = 0b01111000;
constexpr u8 JNE    = 0b01110101;
constexpr u8 JNL    = 0b01111101;
constexpr u8 JNLE   = 0b01111111;
constexpr u8 JNB    = 0b01110011;
constexpr u8 JNBE   = 0b01110111;
constexpr u8 JNP    = 0b01111011;
constexpr u8 JNO    = 0b01110001;
constexpr u8 JNS    = 0b01111001;
constexpr u8 LOOP   = 0b11100010;
constexpr u8 LOOPZ  = 0b11100001;
constexpr u8 LOOPNZ = 0b11100000;
constexpr u8 JCXZ   = 0b11100011;


const std::map<u8, decodeFunc> opName =
{
	{ MOVRMR, movrmrDecode },
	{ MOVIRM, movirmDecode },
	{ MOVIR, movirDecode },
	{ MOVMA, movmaDecode },
	{ MOVAM, movamDecode },
	{ ARITHIRM, arithirmDecode },
	{ ADDRMR, addrmrDecode },
	{ ADDIA, addiaDecode },
	{ SUBRMR, subrmrDecode },
	{ SUBIA, subiaDecode },
	{ CMPRMR, cmprmrDecode },
	{ CMPIA, cmpiaDecode }
};
