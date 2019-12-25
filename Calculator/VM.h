#pragma once
// VirtualMachine.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <cassert>
#include <stack>
#include <fstream>
#include <map>
#include <string>
#include <set>
using namespace std;

struct Variable
{
	enum
	{
		INT,
		BOOL,
		FLOAT
	};
	int type;
	union
	{
		int ival;
		double fval;
		bool bval;
	};
};
struct Instruction
{
	enum
	{
		HALT,
		WTGM,
		WTSM,
		RFGM,
		RFSM,
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
	};
	int opcode = Instruction::HALT;
	union
	{
		int ival;
		double fval;
		bool bval;
	};
};

class VirtualMachine
{
public:
	
	map<int, string> strins;// for debugging
	void executeFile(const char* file, int param = 0)
	{
		cout << "loading instructions...\n";
		init();
		loadInstructions(file);
		cout << "instructions loaded sucessfully\n";
		Variable var;
		var.type = Variable::INT;
		var.ival = param;
		m_opnds.push(var);
		cout << "param " + param << endl;
		var.ival = m_ins.size() - 1;
		m_opnds.push(var);
		cout << "return addr " << var.ival << " Instruction:" + strins[var.ival] << endl;
		executeInstructions();
		cout << "executing instructions...\n";
		cout << "done\n";
	}
	void loadInstructions(const char* file)
	{

		std::map<std::string, int> mp =
		{
			{"halt",Instruction::HALT},
			{"incfp",Instruction::INCFP},
			{"decfp",Instruction::DECFP},
			{"rfsm",Instruction::RFSM},
			{"wtsm",Instruction::WTSM},
			{"wtgm",Instruction::WTGM},
			{"rfgm",Instruction::RFGM},
			{"rfsm",Instruction::RFSM},
			{"jal",Instruction::JAL},
			{"jump",Instruction::JUMP},
			{"jl",Instruction::JL},
			{"jiff",Instruction::JIFF},
			{"printi",Instruction::PRINTI},
			{"printf",Instruction::PRINTF},
			{"printb",Instruction::PRINTB},
			{"pushi",Instruction::PUSHI},
			{"pushf",Instruction::PUSHF},
			{"pushb",Instruction::PUSHB},
			{"pop",Instruction::POP},
			{"toi",Instruction::TOI},
			{"tof",Instruction::TOF},
			{"tob",Instruction::TOB},
			{"nagi",Instruction::NAGI},
			{"nagf",Instruction::NAGF},
			{"or",Instruction::OR},
			{"and",Instruction::AND},
			{"eqi",Instruction::EQI},
			{"eqf",Instruction::EQF},
			{"eqb",Instruction::EQB},
			{"gti",Instruction::GTI},
			{"gtf",Instruction::GTF},
			{"lti",Instruction::LTI},
			{"ltf",Instruction::LTF},
			{"addi",Instruction::ADDI},
			{"addf",Instruction::ADDF},
			{"subi",Instruction::SUBI},
			{"subf",Instruction::SUBF},
			{"muli",Instruction::MULI},
			{"mulf",Instruction::MULF},
			{"divi",Instruction::DIVI},
			{"divf",Instruction::DIVF}
		};

		std::set<int> needInt =
		{

			Instruction::JAL,
			Instruction::JIFF,
			Instruction::PUSHI,
		};
		string instr;
		ifstream in;
		int codeAddr = 0;
		in.open(file);
		if (!in.is_open())
		{
			cout << "fail to open the file\n";
			return;
		}
		while (true)
		{
			Instruction i;

			string str;
			if (!(in >> str))
				break;
			if (str == "#")
			{
				std::getline(in, str);
				continue;
			}
			cout << str;
			if (str == "lable")
			{
				int lable;
				in >> lable;
				cout << " " << lable << endl;
				m_lable2addr.insert(make_pair(lable, codeAddr));
				continue;
			}
			instr = str;
			assert(mp.find(str) != mp.end());
			i.opcode = mp[str];
			if (i.opcode == Instruction::PUSHB)
			{
				in >> str;
				instr += " " + str;
				cout << " " + str;
				if (str == "false")
					i.bval = false;
				else if (str == "true")
					i.bval = true;
				else
					assert(0);
			}
			if (needInt.find(i.opcode) != needInt.end())
			{
				int value;
				in >> value;
				i.ival = value;
				cout << " " + to_string(value);
				instr += " " + to_string(value);

			}
			if (i.opcode == Instruction::PUSHF)
			{
				double value;
				in >> value;
				i.fval = value;
				cout << " " + to_string(value);
				instr += " " + to_string(value);
			}
			strins.insert(make_pair(codeAddr, instr));
			m_ins.push_back(i);
			codeAddr++;
			cout << endl;
		}
		Instruction i;
		i.opcode = Instruction::HALT;
		m_ins.push_back(i);
		strins.insert(make_pair(codeAddr, "halt"));
		in.close();
	}
	void executeInstructions()
	{
		//m_pc = m_lable2addr[0];
		mainloop();
	}
private:
	void init()
	{
		m_lable2addr.clear();
		m_opnds = stack<Variable>();
		for (int i = 0; i < 1000; i++)
		{
			m_globalVariables.push_back(Variable());
			m_memory.push_back(Variable());
		}


		m_ins.clear();
		m_pc = 0;
		m_fp = 0;
	}
	Variable getGlobalVariable(int idx)
	{
		cout << "fetch global variable at " << idx << " its value is ";
		if (m_globalVariables[idx].type == Variable::BOOL)
		{
			cout << m_globalVariables[idx].bval ? "true\n" : "false\n";
		}
		if (m_globalVariables[idx].type == Variable::INT)
		{
			cout << m_globalVariables[idx].ival << endl;
		}
		if (m_globalVariables[idx].type == Variable::FLOAT)
		{
			cout << m_globalVariables[idx].bval << endl;
		}
		return m_globalVariables[idx];
	}
	void setGlobalVariable(int idx, Variable var)
	{
		cout << "set global variable at " << idx << " to ";
		if (var.type == Variable::BOOL)
		{
			cout << var.bval ? "true\n" : "false\n";
		}
		if (var.type == Variable::INT)
		{
			cout << var.ival << endl;
		}
		if (var.type == Variable::FLOAT)
		{
			cout << var.bval << endl;
		}
		m_globalVariables[idx] = var;
	}
	Variable getLocalVariable(int idx)
	{
		cout << "fetch local variable at " << idx << " its value is ";
		if (m_memory[idx].type == Variable::BOOL)
		{
			cout << m_memory[idx].bval ? "true\n" : "false\n";
		}
		if (m_memory[idx].type == Variable::INT)
		{
			cout << m_memory[idx].ival << endl;
		}
		if (m_memory[idx].type == Variable::FLOAT)
		{
			cout << m_memory[idx].bval << endl;
		}
		return m_memory[idx];
	}
	void setLocalVariable(int idx, Variable var)
	{
		cout << "set local variable at " << idx << " to ";
		if (var.type == Variable::BOOL)
		{
			cout << var.bval ? "true\n" : "false\n";
		}
		if (var.type == Variable::INT)
		{
			cout << var.ival << endl;
		}
		if (var.type == Variable::FLOAT)
		{
			cout << var.bval << endl;
		}
		m_memory[idx] = var;
	}
	map<int, int> m_lable2addr;
	stack<Variable> m_opnds;
	vector<Variable> m_memory;
	vector<Variable> m_globalVariables;
	vector<Instruction> m_ins;
	int m_fp;
	int m_pc;
	void mainloop()
	{
		while (true)
		{
			//cout << "PC: " << m_pc;
			//cout << " ,Instruction: " + strins[m_pc] << endl;
			auto in = m_ins[m_pc++];
			if (in.opcode == Instruction::HALT)
				break;
			dispatch(in);
		}
	}
private:
	void dispatch(Instruction in)
	{
		switch (in.opcode)
		{
		case Instruction::DECFP:
			doDecfp();
			break;
		case Instruction::INCFP:
			doIncfp();
			break;
		case Instruction::WTSM:
			doWtsm();
			break;
		case Instruction::RFSM:
			doRfsm();
			break;
		case Instruction::WTGM:
			doWtgm();
			break;
		case Instruction::RFGM:
			doRfgm();
			break;
		case Instruction::JAL:
			doJal(in.ival);
			break;
		case Instruction::JUMP:
			doJump();
			break;
		case Instruction::JL:
			doJl();
			break;
		case Instruction::JIFF:
			doJiff(in.ival);
			break;
		case Instruction::PRINTI:
			doPrinti();
			break;
		case Instruction::PRINTF:
			doPrintf();
			break;
		case Instruction::PRINTB:
			doPrintb();
			break;
		case Instruction::PUSHI:
			doPushi(in.ival);
			break;
		case Instruction::PUSHF:
			doPushf(in.fval);
			break;
		case Instruction::PUSHB:
			doPushb(in.bval);
			break;
		case Instruction::POP:
			doPop();
			break;
		case Instruction::TOI:
			doToi();
			break;
		case Instruction::TOF:
			doTof();
			break;
		case Instruction::TOB:
			doTob();
			break;
		case Instruction::NAGI:
			doNagi();
			break;
		case Instruction::NAGF:
			doNagf();
			break;
		case Instruction::OR:
			doOr();
			break;
		case Instruction::AND:
			doAnd();
			break;
		case Instruction::EQI:
			doEqi();
			break;
		case Instruction::EQF:
			doEqf();
			break;
		case Instruction::EQB:
			doEqb();
			break;
		case Instruction::GTI:
			doGti();
			break;
		case Instruction::GTF:
			doGtf();
			break;
		case Instruction::LTI:
			doLti();
			break;
		case Instruction::LTF:
			doLtf();
			break;
		case Instruction::ADDI:
			doAddi();
			break;
		case Instruction::ADDF:
			doAddf();
			break;
		case Instruction::SUBI:
			doSubi();
			break;
		case Instruction::SUBF:
			doSubf();
			break;
		case Instruction::MULI:
			doMuli();
			break;
		case Instruction::MULF:
			doMulf();
			break;
		case Instruction::DIVI:
			doDivi();
			break;
		case Instruction::DIVF:
			doDivf();
			break;
		default:
			assert(0);
			break;
		}
	}

	void doWtsm()
	{
		auto offset = m_opnds.top().ival;
		m_opnds.pop();
		setLocalVariable(m_fp + offset, m_opnds.top());
	}
	void doRfsm()
	{
		auto offset = m_opnds.top().ival;
		m_opnds.pop();
		m_opnds.push(getLocalVariable(m_fp + offset));
	}
	void doWtgm()
	{
		auto offset = m_opnds.top().ival;
		m_opnds.pop();
		setGlobalVariable(offset, m_opnds.top());
	}
	void doRfgm()
	{
		auto offset = m_opnds.top().ival;
		m_opnds.pop();
		m_opnds.push(getGlobalVariable(offset));
	}

	void doDecfp()
	{
		m_fp -= m_opnds.top().ival;
		m_opnds.pop();
	}
	void doIncfp()
	{
		m_fp += m_opnds.top().ival;
		m_opnds.pop();
	}
	void doJal(int lable)
	{
		Variable var;
		var.type = Variable::INT;
		var.ival = m_pc;
		m_opnds.push(var);
		m_pc = m_lable2addr[lable];
	}
	void doJump()
	{
		m_pc = m_opnds.top().ival;
		m_opnds.pop();
	}
	void doJl()
	{
		m_pc = m_lable2addr[m_opnds.top().ival];
		m_opnds.pop();
	}
	void doJiff(int lable)
	{
		auto condition = m_opnds.top();
		m_opnds.pop();
		if (!condition.bval)
			m_pc = m_lable2addr[lable];
	}
	void doPrinti()
	{
		cout << m_opnds.top().ival << endl;
		m_opnds.pop();
	}
	void doPrintf()
	{
		cout << m_opnds.top().fval << endl;
		m_opnds.pop();
	}
	void doPrintb()
	{
		if (m_opnds.top().bval)
			cout << "true\n";
		else
			cout << "false\n";
		m_opnds.pop();
	}
	void doPushi(int value)
	{
		Variable opnd;
		opnd.type = Variable::INT;
		opnd.ival = value;
		m_opnds.push(opnd);
	}
	void doPushf(double value)
	{
		Variable opnd;
		opnd.type = Variable::FLOAT;
		opnd.fval = value;
		m_opnds.push(opnd);
	}
	void doPushb(bool value)
	{
		Variable opnd;
		opnd.type = Variable::BOOL;
		opnd.bval = value;
		m_opnds.push(opnd);
	}
	void doPop()
	{
		m_opnds.pop();
	}
	void doToi()
	{
		auto o = m_opnds.top();
		m_opnds.pop();
		Variable n = o;
		if (o.type == Variable::BOOL)
			n.ival = o.bval;
		if (o.type == Variable::FLOAT)
			n.ival = o.fval;
		m_opnds.push(n);
	}
	void doTof()
	{
		auto o = m_opnds.top();
		m_opnds.pop();
		Variable n = o;
		if (o.type == Variable::BOOL)
			n.fval = o.bval;
		if (o.type == Variable::INT)
			n.fval = o.ival;
		m_opnds.push(n);
	}
	void doTob()
	{
		auto o = m_opnds.top();
		m_opnds.pop();
		Variable n = o;
		if (o.type == Variable::INT)
			n.bval = o.ival;
		if (o.type == Variable::FLOAT)
			n.bval = o.fval;
		m_opnds.push(n);
	}
	void doNot()
	{
		m_opnds.top().bval = !m_opnds.top().bval;
	}
	void doNagi()
	{
		m_opnds.top().ival = -m_opnds.top().ival;
	}
	void doNagf()
	{
		m_opnds.top().fval = -m_opnds.top().fval;
	}
	void doOr()
	{
		auto o = m_opnds.top();
		m_opnds.pop();
		m_opnds.top().bval = m_opnds.top().bval || o.bval;
	}
	void doAnd()
	{
		auto o = m_opnds.top();
		m_opnds.pop();
		m_opnds.top().bval = m_opnds.top().bval && o.bval;
	}
	void doEqi()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.ival == o2.ival;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doEqf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval == o2.fval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doEqb()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.bval == o2.bval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doGti()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.bval > o2.bval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doGtf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval > o2.fval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doLti()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.bval < o2.bval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doLtf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval < o2.fval;
		n.type = Variable::BOOL;
		m_opnds.push(n);
	}
	void doAddi()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.ival = o1.ival + o2.ival;
		n.type = Variable::INT;
		m_opnds.push(n);
	}
	void doAddf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval + o2.fval;
		n.type = Variable::FLOAT;
		m_opnds.push(n);
	}
	void doSubi()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.ival = o1.ival - o2.ival;
		n.type = Variable::INT;
		m_opnds.push(n);
	}
	void doSubf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval - o2.fval;
		n.type = Variable::FLOAT;
		m_opnds.push(n);
	}
	void doMuli()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.ival = o1.ival * o2.ival;
		n.type = Variable::INT;
		m_opnds.push(n);
	}
	void doMulf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval * o2.fval;
		n.type = Variable::FLOAT;
		m_opnds.push(n);
	}
	void doDivi()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.ival = o1.ival / o2.ival;
		n.type = Variable::INT;
		m_opnds.push(n);
	}
	void doDivf()
	{
		auto o2 = m_opnds.top();
		m_opnds.pop();
		auto o1 = m_opnds.top();
		m_opnds.pop();
		Variable n;
		n.bval = o1.fval / o2.fval;
		n.type = Variable::FLOAT;
		m_opnds.push(n);
	}
};


//int main()
//{
//	VirtualMachine vm;
//	const char* p = "C:\\Users\\wen\\Desktop\\tcc\\asm.txt";
//	vm.executeFile(p);
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
