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

extern u16 REGS[8];
extern u16 MEM[1024 * 1024];

void printRegs();
void printMem(size_t offset, size_t size);
