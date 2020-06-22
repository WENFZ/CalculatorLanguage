#pragma once
struct Instruction
{
	enum
	{
		HALT,
		WTGM,
		WTSM,
		WTHM,
		RFGM,
		RFSM,
		RFHM,
		INCFP,
		DECFP,
		JAL,
		JUMP,
		JL,
		JIFF,
		PRINTI,
		PRINTF,
		PRINTB,
		PUSHI,
		PUSHF,
		PUSHB,
		POP,
		TOI,
		TOF,
		TOB,
		NAGI,
		NAGF,
		OR,
		AND,
		NOT,
		EQI,
		EQF,
		EQB,
		GTI,
		GTF,
		LTI,
		LTF,
		ADDI,
		ADDF,
		SUBI,
		SUBF,
		MULI,
		MULF,
		DIVI,
		DIVF,
		NEW,
		MA,
	};
	int opcode = Instruction::HALT;
	union
	{
		int ival;
		double fval;
		bool bval;
	};
};