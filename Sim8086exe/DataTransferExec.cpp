#include "DataTransferExec.h"
#include "Cpu.h"

#define REGIDX(x) (x.Register.Index - 1)


u8 dataTransferExec(const instruction *instr)
{
	switch (instr->Op)
	{
	case Op_mov:
		{
			const instruction_operand dest = instr->Operands[0];
			if (dest.Type == Operand_Register)
			{
				s32 val{};
				const instruction_operand src = instr->Operands[1];
				if (src.Type == Operand_Immediate)
				{
					val = src.Immediate.Value;
				}
				else if (src.Type == Operand_Register)
				{
					val = REGS[REGIDX(src)];
				}

				if (dest.Register.Count == 1)
				{
					val = val & 0xff;
				}
				else
				{
					val = val & 0xffff;
				}

				REGS[REGIDX(dest)] = val;
			}
		}
		break;
	}

	return 0;
}
