#include "ControlExec.h"

#include "Cpu.h"


u8 ControlExec(const instruction *instr)
{
	printf("%s ", Sim86_MnemonicFromOperationType(instr->Op));

	bool jump = false;

	switch (instr->Op)
	{
	case Op_jne:
		if (!(cpu.flags & ZF)) jump = true;
		break;
	}

	if (jump)
	{
		auto inc = instr->Operands[0].Immediate.Value;
		cpu.ip   = (u16)((s32)cpu.ip + inc);
	}

	printf(" IP: %d\n", cpu.ip);

	return 0;
}
