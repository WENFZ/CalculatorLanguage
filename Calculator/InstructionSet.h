#pragma once
#include<string>
class InstructionSet
{
public:
	std::string Halt()
	{
		return "halt";
	}
	std::string Jal(int lable)
	{
		return "jal " + std::to_string(lable);
	}
	std::string Lable(int i)
	{
		return "lable " + std::to_string(i);
	}
	std::string Jiff(int lable)
	{
		return "jiff "+std::to_string(lable);
	}
	std::string Jump()
	{
		return "jump";
	}
	std::string Jl()
	{
		return "jl";
	}
	std::string Pushi(int value)
	{
		return "pushi " + std::to_string(value);
	}
	std::string Pushf(double value)
	{
		return "pushf " + std::to_string(value);
	}
	std::string Pushb(bool value)
	{
		if (value)
		{
			return "pushb true";
		}
		else
		{
			return "pushb false";
		}
	}
	std::string Pop()
	{
		return "pop";
	}

	std::string Printi()
	{
		return "printi";
	}
	std::string Printf()
	{
		return "printf";
	}
	std::string Printb()
	{
		return "printb";
	}
	// unary expression
	std::string Toi()
	{
		return "toi";
	}
	std::string Tof()
	{
		return "tof";
	}
	std::string Tob()
	{
		return "tob";
	}
	std::string Not()
	{
		return "not";
	}
	std::string Nagi()
	{
		return "nagi";
	}
	std::string Nagf()
	{
		return "nagf";
	}
	// bianry expression

	std::string Or()
	{
		return "or";
	}
	std::string And()
	{
		return "and";
	}
	
	std::string Eqi()
	{
		return "eqi";
	}
	std::string Eqf()
	{
		return "eqi";
	}
	std::string Eqb()
	{
		return "eqi";
	}

	std::string Gti()
	{
		return "gti";
	}
	std::string Gtf()
	{
		return "gtf";
	}

	std::string Lti()
	{
		return "lti";
	}
	std::string Ltf()
	{
		return "ltf";
	}

	std::string Addi()
	{
		return "addi";
	}
	std::string Addf()
	{
		return "addf";
	}
	std::string Subi()
	{
		return "subi";
	}
	std::string Subf()
	{
		return "subf";
	}
	std::string Muli()
	{
		return "muli";
	}
	std::string Mulf()
	{
		return "mulf";
	}
	std::string Divi()
	{
		return "divi";
	}
	std::string Divf()
	{
		return "divf";
	}
	std::string Rfsm()
	{
		return "rfsm";// opnd=stack memory[fp+offset]
	}
	std::string Wtsm()
	{
		return "wtsm";// stack memory[fp+offset]=res
	}
	std::string Rfgm()
	{
		return "rfgm";// res=global memory[fp+r1]
	}
	std::string Wtgm()
	{
		return "wtgm";// global memory[fp+r1]=res
	}
	std::string IncFP()
	{
		return "incfp";
	}
	std::string DecFP()
	{
		return "decfp";
	}

};

