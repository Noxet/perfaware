#include "Cpu.h"

cpu_t cpu{};


static const char *g_cpuFlagStr[] =
{
	"C",
	"P",
	"A",
	"Z",
	"S"
};


void printRegs()
{
	printf("\nRegisters:\n");
	string regsStr[] = { "ax", "bx", "cx", "dx", "sp", "bp", "si", "di" };

	for (int i = 0; i < 8; ++i)
	{
		cout << std::format("{}: {:04x}", regsStr[i], cpu.REGS[i]) << endl;
	}
}


void printFlags()
{
	// don't print if no flag is set
	if (cpu.flags == 0) return;

	for (int i = 0; i < 5; ++i)
	{
		if (cpu.flags & (1 << i))
		{
			printf("%s", g_cpuFlagStr[i]);
		}
	}
}


void printMem(size_t offset, size_t size)
{
	printf("\nMemory:\n");
	if (offset + size >= sizeof(cpu.MEM))
	{
		cout << "Can't print outside memory boundary" << endl;
		return;
	}

	for (size_t i = offset; i < offset + size; ++i)
	{
		cout << format("[{}]: {:04x}\n", i, cpu.MEM[i]);
	}
}
