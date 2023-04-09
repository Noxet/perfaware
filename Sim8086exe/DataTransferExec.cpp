#include "DataTransferExec.h"
#include "Cpu.h"


u8 dataTransferExec(const instruction *instr)
{
	switch (instr->Op)
	{
	case Op_mov:
		{
			cout << printf("%s ", Sim86_MnemonicFromOperationType(instr->Op)) << endl;
			const instruction_operand dest = instr->Operands[0];
			if (dest.Type == Operand_Register)
			{
				printf("idx: %d, offset: %d, count: %d\n", instr->Operands[0].Register.Index,
				       instr->Operands[0].Register.Offset, instr->Operands[0].Register.Count);

				s32 val{};
				const instruction_operand src = instr->Operands[1];
				if (src.Type == Operand_Immediate)
				{
					val = src.Immediate.Value;
				}
				else if (src.Type == Operand_Register)
				{
					val = REGS[src.Register.Index];
				}

				if (dest.Register.Count == 1)
				{
					val = val & 0xff;
				}
				else
				{
					val = val & 0xffff;
				}

				REGS[dest.Register.Index - 1] = val;
			}
		}
		break;
	}

	return 0;
}
