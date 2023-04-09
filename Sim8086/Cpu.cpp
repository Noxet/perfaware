#include <format>

#include 


u16 REGS[8];
u16 MEM[1024 * 1024];


void printRegs()
{
	string regsStr[] = { "ax", "bx", "cx", "dx", "sp", "bp", "si", "di" };

	for (int i = 0; i < 8; ++i)
	{
		cout << std::format("{}: {:04x}", regsStr[i], REGS[i]) << endl;
	}
}


void printMem(size_t offset, size_t size)
{
	if (offset + size >= sizeof(MEM))
	{
		cout << "Can't print outside memory boundary" << endl;
		return;
	}

	for (size_t i = offset; i < offset + size; ++i)
	{
		cout << format("[{}]: {:04x}", i, MEM[i]);
	}
}
