#include "ArithmeticExec.h"

#include "Cpu.h"

#define REGIDX(x) (x.Register.Index - 1)


u8 ArithmeticExec(const instruction *instr)
{
	printf("%s ", Sim86_MnemonicFromOperationType(instr->Op));

	instruction_operand dest = instr->Operands[0];
	s32 val{};
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
	case Op_add:
		{
			cpu.REGS[REGIDX(dest)] += val;
		}
		break;
	case Op_sub:
		{
			cpu.REGS[REGIDX(dest)] -= val;
		}
		break;
	case Op_cmp:
		// TODO: fix this
		break;
	}

	if (cpu.REGS[REGIDX(dest)] & 0x8000) cpu.flags |= SF;
	else cpu.flags &= ~SF;


	if (cpu.REGS[REGIDX(dest)] == 0) cpu.flags |= ZF;
	else cpu.flags &= ~ZF;

	printFlags();
	printf("\n");

	return 0;
}
