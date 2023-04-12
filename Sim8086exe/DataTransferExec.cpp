#include "DataTransferExec.h"
#include "Cpu.h"

#define REGIDX(x) (x.Register.Index - 1)


u8 dataTransferExec(const instruction *instr)
{
	printf("%s ", Sim86_MnemonicFromOperationType(instr->Op));

	s32 val{};
	instruction_operand dest = instr->Operands[0];
	if (dest.Type == Operand_Register)
	{
		instruction_operand src = instr->Operands[1];
		printf("%s, ", Sim86_RegisterNameFromOperand(&dest.Register));

		if (src.Type == Operand_Immediate)
		{
			val = src.Immediate.Value;
			printf("%d ", val);
		}
		else if (src.Type == Operand_Register)
		{
			val = cpu.REGS[REGIDX(src)];
			printf("%s, ", Sim86_RegisterNameFromOperand(&src.Register));
		}

		if (dest.Register.Count == 1)
		{
			val = val & 0xff;
		}
		else
		{
			val = val & 0xffff;
		}
	}

	switch (instr->Op)
	{
	case Op_mov:
		{
			cpu.REGS[REGIDX(dest)] = val;
		}
		break;
	}

	printf(" IP: %d", cpu.ip);
	printf("\n");

	return 0;
}
