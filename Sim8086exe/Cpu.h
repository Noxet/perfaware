#pragma once

#include <iostream>
#include <cstdint>
#include <format>
#include <vector>

#include "sim86_shared.h"

#define DEBUGN

using namespace std;

using std::cout;
using std::endl;

using mCodeItr = std::vector<u8>::iterator;

//enum Register
//{
//	AX = 0,
//	BX,
//	CX,
//	DX,
//	SP,
//	BP,
//	SI,
//	DI
//};

enum CpuFlags
{
	CF = (1 << 0),
	PF = (1 << 1),
	AF = (1 << 2),
	ZF = (1 << 3),
	SF = (1 << 4),
};


typedef struct
{
	u16 REGS[8];
	u16 MEM[1024 * 1024];
	u16 flags;
	u16 ip;
} cpu_t;

extern cpu_t cpu;

void printRegs();
void printFlags();
void printMem(size_t offset, size_t size);
