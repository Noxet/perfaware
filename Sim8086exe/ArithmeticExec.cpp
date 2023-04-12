#include "ArithmeticExec.h"

#include "Cpu.h"

#define REGIDX(x) (x.Register.Index - 1)


u8 ArithmeticExec(const instruction *instr)
{
	printf("%s ", Sim86_MnemonicFromOperationType(instr->Op));

	instruction_operand dest = instr->Operands[0];
	u16 val{};
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


	u16 result{};
	switch (instr->Op)
	{
	case Op_add:
		{
			cpu.REGS[REGIDX(dest)] += val;
			result = cpu.REGS[REGIDX(dest)];
		}
		break;
	case Op_sub:
		{
			cpu.REGS[REGIDX(dest)] -= val;
			result = cpu.REGS[REGIDX(dest)];
		}
		break;
	case Op_cmp:
		// do a subtraction, but do not update registers
		result = cpu.REGS[REGIDX(dest)] - val;
		break;
	}

	printf(" Flags: ");
	printFlags();

	if (result & 0x8000) cpu.flags |= SF;
	else cpu.flags &= ~SF;


	if (result == 0) cpu.flags |= ZF;
	else cpu.flags &= ~ZF;

	printf(" -> ");
	printFlags();
	printf("\n");

	return 0;
}
