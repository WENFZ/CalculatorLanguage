#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <stack>
#include <fstream>
#include <map>
#include <string>
#include <set>
#include "HeapManager.h"
#include "Instruction.h"

using namespace std;

//namespace VM {
	class VirtualMachine
	{
	public:
		map<int, vector<int>> m_structureMetas;
		bool verbose;
		map<int, string> strins;// for debugging
		map<int, int> pc2line;
		void executeFile(const char* file, int param);
		void loadInstructions(const char* file);
		void executeInstructions();
	//private:
		void init();

		map<int, int> m_lable2addr;
		list<Variable> m_opnds;
		vector<Variable> m_memory;
		vector<Variable> m_globalVariables;
		vector<Instruction> m_ins;
		MemoryManager* m_mm;
		int m_fp;
		int m_pc;


		void mainloop();
		void dispatch(Instruction in);

		set<Object*> getRootset()
		{
			set<Object*> res;
			for (auto it : m_rootSet)
			{
				res.insert(it.first);
			}
			for (auto it : m_opnds)
			{
				if (it.type == Variable::REFERENCE)
				{
					res.insert(it.oval);
				}
			}
			return res;
		}
	private:
		
		Variable getGlobalVariable(int idx)
		{
			if (verbose)
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
				if (m_globalVariables[idx].type == Variable::REFERENCE)
				{
					cout << "object\n";
				}
			}
			return m_globalVariables[idx];
		}
		void setGlobalVariable(int idx, Variable var)
		{
			if (verbose)
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
				if (var.type == Variable::REFERENCE)
				{
					cout << "object\n";
				}
			}
			if (m_globalVariables[idx].type == Variable::REFERENCE)
			{
				auto it = m_rootSet.find(m_globalVariables[idx].oval);
				assert(it != m_rootSet.end());
				it->second--;
				if (it->second == 0)
				{
					m_rootSet.erase(it);
				}
			}
			if (var.type == Variable::REFERENCE)
			{
				auto it = m_rootSet.find(var.oval);
				if (it == m_rootSet.end())
				{
					m_rootSet.insert(make_pair(var.oval, 1));
				}
				else
				{
					it->second++;
				}
			}
			m_globalVariables[idx] = var;
		}
		Variable getLocalVariable(int idx)
		{
			if (verbose)
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
				if (m_memory[idx].type == Variable::REFERENCE)
				{
					cout << "object\n";
				}
			}
			return m_memory[idx];
		}
		void setLocalVariable(int idx, Variable var)
		{
			if (verbose)
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
				if (var.type == Variable::REFERENCE)
				{
					cout << "object\n";
				}
			}
			if (m_memory[idx].type == Variable::REFERENCE)
			{
				auto it = m_rootSet.find(m_memory[idx].oval);
				assert(it != m_rootSet.end());
				it->second--;
				if (it->second == 0)
				{
					m_rootSet.erase(it);
				}
			}
			if (var.type == Variable::REFERENCE)
			{
				auto it = m_rootSet.find(var.oval);
				if (it == m_rootSet.end())
				{
					m_rootSet.insert(make_pair(var.oval, 1));
				}
				else
				{
					it->second++;
				}
			}
			m_memory[idx] = var;
		}


		map<Object*,int> m_rootSet;

	private:
		void doWthm()
		{
			auto addr = m_opnds.back();
			m_opnds.pop_back();
			auto var = m_opnds.back();
			assert(addr.type == Variable::ADDR);
			auto realAddr = addr.addr;
			*realAddr = var;
		}
		void doRfhm()
		{
			auto opnd = m_opnds.back();
			m_opnds.pop_back();
			assert(opnd.type == Variable::ADDR);
			auto addr = opnd.addr;
			auto var = *addr;
			m_opnds.push_back(var);
		}
		void doNew(int id)
		{
			auto meta = m_structureMetas.at(id);
			auto obj = m_mm->newObject(meta);
			Variable var;
			var.type = Variable::REFERENCE;
			var.oval = obj;
			
			m_opnds.push_back(var);
		}
		void doMa(int offset)
		{
			auto base = m_opnds.back();
			m_opnds.pop_back();
			assert(base.type == Variable::REFERENCE);
			auto var = m_mm->memberAccess(base.oval, offset);
			m_opnds.push_back(var);
		}
		void doWtsm()
		{
			auto offset = m_opnds.back().ival;
			m_opnds.pop_back();
			setLocalVariable(m_fp + offset, m_opnds.back());
		}
		void doRfsm()
		{
			auto offset = m_opnds.back().ival;
			m_opnds.pop_back();
			m_opnds.push_back(getLocalVariable(m_fp + offset));
		}
		void doWtgm()
		{
			auto offset = m_opnds.back().ival;
			m_opnds.pop_back();
			setGlobalVariable(offset, m_opnds.back());
		}
		void doRfgm()
		{
			auto offset = m_opnds.back().ival;
			m_opnds.pop_back();
			m_opnds.push_back(getGlobalVariable(offset));
		}

		void doDecfp()
		{
			m_fp -= m_opnds.back().ival;
			m_opnds.pop_back();
		}
		void doIncfp()
		{
			m_fp += m_opnds.back().ival;
			m_opnds.pop_back();
		}
		void doJal(int lable)
		{
			Variable var;
			var.type = Variable::INT;
			var.ival = m_pc;
			m_opnds.push_back(var);
			m_pc = m_lable2addr[lable];
		}
		void doJump()
		{
			m_pc = m_opnds.back().ival;
			m_opnds.pop_back();
		}
		void doJl()
		{
			auto label = m_opnds.back().ival;
			assert(m_lable2addr.find(label) != m_lable2addr.end());
			m_pc = m_lable2addr[label];
			m_opnds.pop_back();
		}
		void doJiff(int lable)
		{
			auto condition = m_opnds.back();
			m_opnds.pop_back();
			if (!condition.bval)
				m_pc = m_lable2addr[lable];
		}
		void doPrinti()
		{
			cout << m_opnds.back().ival << endl;
			m_opnds.pop_back();
		}
		void doPrintf()
		{
			cout << m_opnds.back().fval << endl;
			m_opnds.pop_back();
		}
		void doPrintb()
		{
			if (m_opnds.back().bval)
				cout << "true\n";
			else
				cout << "false\n";
			m_opnds.pop_back();
		}
		void doPushi(int value)
		{
			Variable opnd;
			opnd.type = Variable::INT;
			opnd.ival = value;
			m_opnds.push_back(opnd);
		}
		void doPushf(double value)
		{
			Variable opnd;
			opnd.type = Variable::FLOAT;
			opnd.fval = value;
			m_opnds.push_back(opnd);
		}
		void doPushb(bool value)
		{
			Variable opnd;
			opnd.type = Variable::BOOL;
			opnd.bval = value;
			m_opnds.push_back(opnd);
		}
		void doPop()
		{
			m_opnds.pop_back();
		}
		void doToi()
		{
			auto o = m_opnds.back();
			m_opnds.pop_back();
			Variable n = o;
			if (o.type == Variable::BOOL)
				n.ival = o.bval;
			if (o.type == Variable::FLOAT)
				n.ival = o.fval;
			m_opnds.push_back(n);
		}
		void doTof()
		{
			auto o = m_opnds.back();
			m_opnds.pop_back();
			Variable n = o;
			if (o.type == Variable::BOOL)
				n.fval = o.bval;
			if (o.type == Variable::INT)
				n.fval = o.ival;
			m_opnds.push_back(n);
		}
		void doTob()
		{
			auto o = m_opnds.back();
			m_opnds.pop_back();
			Variable n = o;
			if (o.type == Variable::INT)
				n.bval = o.ival;
			if (o.type == Variable::FLOAT)
				n.bval = o.fval;
			m_opnds.push_back(n);
		}
		void doNot()
		{
			m_opnds.back().bval = !m_opnds.back().bval;
		}
		void doNagi()
		{
			m_opnds.back().ival = -m_opnds.back().ival;
		}
		void doNagf()
		{
			m_opnds.back().fval = -m_opnds.back().fval;
		}
		void doOr()
		{
			auto o = m_opnds.back();
			m_opnds.pop_back();
			m_opnds.back().bval = m_opnds.back().bval || o.bval;
		}
		void doAnd()
		{
			auto o = m_opnds.back();
			m_opnds.pop_back();
			m_opnds.back().bval = m_opnds.back().bval && o.bval;
		}
		void doEqi()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.ival == o2.ival;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doEqf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval == o2.fval;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doEqb()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.bval == o2.bval;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doGti()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.ival > o2.ival;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doGtf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval > o2.fval;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doLti()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.ival < o2.ival;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doLtf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval < o2.fval;
			n.type = Variable::BOOL;
			m_opnds.push_back(n);
		}
		void doAddi()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.ival = o1.ival + o2.ival;
			n.type = Variable::INT;
			m_opnds.push_back(n);
		}
		void doAddf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval + o2.fval;
			n.type = Variable::FLOAT;
			m_opnds.push_back(n);
		}
		void doSubi()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.ival = o1.ival - o2.ival;
			n.type = Variable::INT;
			m_opnds.push_back(n);
		}
		void doSubf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval - o2.fval;
			n.type = Variable::FLOAT;
			m_opnds.push_back(n);
		}
		void doMuli()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.ival = o1.ival * o2.ival;
			n.type = Variable::INT;
			m_opnds.push_back(n);
		}
		void doMulf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval * o2.fval;
			n.type = Variable::FLOAT;
			m_opnds.push_back(n);
		}
		void doDivi()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.ival = o1.ival / o2.ival;
			n.type = Variable::INT;
			m_opnds.push_back(n);
		}
		void doDivf()
		{
			auto o2 = m_opnds.back();
			m_opnds.pop_back();
			auto o1 = m_opnds.back();
			m_opnds.pop_back();
			Variable n;
			n.bval = o1.fval / o2.fval;
			n.type = Variable::FLOAT;
			m_opnds.push_back(n);
		}
	};
//}

