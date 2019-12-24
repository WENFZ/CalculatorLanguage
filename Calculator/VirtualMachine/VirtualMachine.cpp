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
		WTMI,
		WTMF,
		WTMB,
		RFMI,
		RFMF,
		RFMB,
		JR,
		JAL,
		JUMP,
		JIFF,
		PRINTI,
		PRINTF,
		PRINTB,
		PUSHI,
		PUSHF,
		PUSHB,
		POPI,
		POPF,
		POPB,
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
	int opcode;
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
	void load(const char* file)
	{
		std::map<std::string, int> mp =
		{
			{"wtmi",Instruction::WTMI},
			{"wtmf",Instruction::WTMI},
			{"wtmb",Instruction::WTMB},
			{"rfmi",Instruction::RFMI},
			{"rfmf",Instruction::RFMF},
			{"rfmb",Instruction::RFMB},
			{"jr",Instruction::JR},
			{"jal",Instruction::JAL},
			{"jump",Instruction::JUMP},
			{"jiff",Instruction::JIFF},
			{"printi",Instruction::PRINTI},
			{"printf",Instruction::PRINTF},
			{"printb",Instruction::PRINTB},
			{"pushi",Instruction::PUSHI},
			{"pushf",Instruction::PUSHF},
			{"pushb",Instruction::PUSHB},
			{"popi",Instruction::POPI},
			{"popf",Instruction::POPF},
			{"popb",Instruction::POPB},
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
		std::set<int> needBool =
		{
			Instruction::PUSHB
		};
		std::set<int> needInt =
		{
			Instruction::WTMI,
			Instruction::WTMF,
			Instruction::WTMB,
			Instruction::RFMI,
			Instruction::RFMF,
			Instruction::RFMB,
			Instruction::JAL,
			Instruction::JUMP,
			Instruction::JIFF,
			Instruction::PUSHI,
		};
		std::set<int> needFloat =
		{
			Instruction::PUSHF
		};
		ifstream in;
		in.open(file);
		if (!in.is_open())
		{
			cout << "fail to open the file\n";
			return;
		}
		while (true)
		{
			Instruction i;
			char opcode[16];
			int len = scanf("%s", opcode);
			if (len == 0)
			{
				i.opcode = Instruction::HALT;
				m_ins.push_back(i);
				break;
			}
			i.opcode = mp[std::string(opcode)];
			if (needBool.find(i.opcode) != needBool.end())
			{
				char value[8];
				int len = scanf("%s", value);
				assert(len == 4 || len == 5);
				if (strcmp("false", value) == 0)
					i.bval = false;
				else if (strcmp("true", value) == 0)
					i.bval = true;
				else
					assert(0);
			}
			if (needInt.find(i.opcode) != needInt.end())
			{
				int value;
				int len=scanf("%d", &value);
				assert(len != 0);
				i.ival = value;
			}
			if (needFloat.find(i.opcode) != needFloat.end())
			{
				double value;
				int len = scanf("%f", &value);
				assert(len != 0);
				i.fval = value;
			}
			m_ins.push_back(i);
		}
		in.close();
	}
private:
	stack<Variable> m_opnds;
	vector<Variable> m_memory;
	vector<Variable> m_globalVariables;
	vector<Instruction> m_ins;
	int m_ra;
	int m_pc;
	void mainloop()
	{
		while (true)
		{
			auto in = m_ins[m_pc++];
			if (dispatch(in))
				break;
		}
		cout << "执行完成\n";
	}
private:
	bool dispatch(Instruction in)
	{
		switch (in.opcode)
		{
		case Instruction::HALT:
			return true;
			break;
		case Instruction::WTMI:
			doWtmi(in.ival);
			break;
		case Instruction::WTMF:
			doWtmi(in.ival);
			break;
		case Instruction::WTMB:
			doWtmb(in.ival);
			break;
		case Instruction::RFMI:
			doRfmi(in.ival);
			break;
		case Instruction::RFMF:
			doRfmf(in.ival);
			break;
		case Instruction::RFMB:
			doRfmb(in.ival);
			break;
		case Instruction::JR:
			doJr();
			break;
		case Instruction::JAL:
			doJal(in.ival);
			break;
		case Instruction::JUMP:
			doJump(in.ival);
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
		case Instruction::POPI:
			doPopi();
			break;
		case Instruction::POPF:
			doPopf();
			break;
		case Instruction::POPB:
			doPopb();
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
		return false;
	}
	void doWtmi(int addr)
	{
		m_memory[addr] = m_opnds.top();
		m_opnds.pop();
	}
	void doWtmf(int addr)
	{
		m_memory[addr] = m_opnds.top();
		m_opnds.pop();
	}
	void doWtmb(int addr)
	{
		m_memory[addr] = m_opnds.top();
		m_opnds.pop();
	}
	void doRfmi(int addr)
	{
		m_opnds.push(m_memory[addr]);
	}
	void doRfmf(int addr)
	{
		m_opnds.push(m_memory[addr]);
	}
	void doRfmb(int addr)
	{
		m_opnds.push(m_memory[addr]);
	}
	void doJr()
	{
		m_pc = m_ra;
	}
	void doJal(int addr)
	{
		m_ra = m_pc;
		m_pc = addr;
	}
	void doJump(int addr)
	{
		m_pc = addr;
	}
	void doJiff(int addr)
	{
		auto condition = m_opnds.top();
		m_opnds.pop();
		if (!condition.bval)
			m_pc = addr;
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
	void doPopi()
	{
		m_opnds.pop();
	}
	void doPopf()
	{
		m_opnds.pop();
	}
	void doPopb()
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
		n.bval = o1.bval + o2.bval;
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
		n.bval = o1.bval - o2.bval;
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
		n.bval = o1.bval * o2.bval;
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
		n.bval = o1.bval / o2.bval;
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


int main()
{
	std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
