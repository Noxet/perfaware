#include "DataTransferExec.h"
#include "Cpu.h"

#define REGIDX(x) (x.Register.Index - 1)


u8 dataTransferExec(const instruction *instr)
{
	printf("%s ", Sim86_MnemonicFromOperationType(instr->Op));

	instruction_operand dest = instr->Operands[0];
	instruction_operand src  = instr->Operands[1];

	char srcStr[10] = {};

	s32 val{};
	if (src.Type == Operand_Immediate)
	{
		val = src.Immediate.Value;
		snprintf(srcStr, sizeof(srcStr), "%d", val);
	}
	else if (src.Type == Operand_Register)
	{
		val = cpu.REGS[REGIDX(src)];
		snprintf(srcStr, sizeof(srcStr), "%s", Sim86_RegisterNameFromOperand(&src.Register));
	}
	else if (src.Type == Operand_Memory)
	{
		const u32 idx1 = src.Address.Terms[0].Register.Index;
		const u32 idx2 = src.Address.Terms[1].Register.Index;
		u16 regVal1    = 0;
		u16 regVal2    = 0;

		if (idx1 > 0 && idx2 > 0)
		{
			snprintf(srcStr, sizeof(srcStr), "[%s + %s]",
			         Sim86_RegisterNameFromOperand(&src.Address.Terms[0].Register),
			         Sim86_RegisterNameFromOperand(&src.Address.Terms[1].Register));
			regVal1 = cpu.REGS[idx1 - 1];
			regVal2 = cpu.REGS[idx2 - 1];
		}
		else if (idx1 > 0)
		{
			snprintf(srcStr, sizeof(srcStr), "[%s + %d]",
			         Sim86_RegisterNameFromOperand(&src.Address.Terms[0].Register),
			         src.Address.Displacement);
			regVal1 = cpu.REGS[idx1 - 1];
		}
		else
		{
			snprintf(srcStr, sizeof(srcStr), "[%d]", src.Address.Displacement);
		}

		val = cpu.MEM[regVal1 + regVal2 + src.Address.Displacement];
	}


	if (dest.Type == Operand_Register)
	{
		printf("%s, ", Sim86_RegisterNameFromOperand(&dest.Register));

		if (dest.Register.Count == 1)
		{
			val = val & 0xff;
		}
		else
		{
			val = val & 0xffff;
		}

		cpu.REGS[REGIDX(dest)] = val;
	}
	else if (dest.Type == Operand_Memory)
	{
		const u32 idx1 = dest.Address.Terms[0].Register.Index;
		const u32 idx2 = dest.Address.Terms[1].Register.Index;
		u16 regVal1    = 0;
		u16 regVal2    = 0;

		if (idx1 > 0 && idx2 > 0)
		{
			printf("[%s + %s], ", Sim86_RegisterNameFromOperand(&dest.Address.Terms[0].Register),
			       Sim86_RegisterNameFromOperand(&dest.Address.Terms[1].Register));
			regVal1 = cpu.REGS[idx1 - 1];
			regVal2 = cpu.REGS[idx2 - 1];
		}
		else if (idx1 > 0)
		{
			printf("[%s + %d], ", Sim86_RegisterNameFromOperand(&dest.Address.Terms[0].Register),
			       dest.Address.Displacement);
			regVal1 = cpu.REGS[idx1 - 1];
		}
		else
		{
			printf("[%d], ", dest.Address.Displacement);
		}

		const int addr = regVal1 + regVal2 + dest.Address.Displacement;
		cpu.MEM[addr]  = val;
	}

	// print src
	printf("%s", srcStr);

	printf(" IP: %d", cpu.ip);
	printf("\n");

	return 0;
}
