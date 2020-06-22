#include "VirualMachine.h"

void VirtualMachine::executeFile(const char* file, int param)
{
	cout << "loading instructions...\n";
	init();
	loadInstructions(file);
	cout << "instructions loaded sucessfully\n";
	Variable var;
	var.type = Variable::INT;
	var.ival = param;
	m_opnds.push_back(var);
	cout << "param " + param << endl;
	var.ival = m_ins.size() - 1;
	m_opnds.push_back(var);
	cout << "return addr " << var.ival << " Instruction:" + strins[var.ival] << endl;
	executeInstructions();
	cout << "executing instructions...\n";
	cout << "done\n";
}

inline void VirtualMachine::loadInstructions(const char* file)
{

	std::map<std::string, int> mp =
	{
		{ "halt",Instruction::HALT },
		{ "incfp",Instruction::INCFP },
		{ "decfp",Instruction::DECFP },
		{ "rfsm",Instruction::RFSM },
		{ "wtsm",Instruction::WTSM },
		{ "wtgm",Instruction::WTGM },
		{ "rfgm",Instruction::RFGM },
		{ "rfsm",Instruction::RFSM },
		{ "jal",Instruction::JAL },
		{ "jump",Instruction::JUMP },
		{ "jl",Instruction::JL },
		{ "jiff",Instruction::JIFF },
		{ "printi",Instruction::PRINTI },
		{ "printf",Instruction::PRINTF },
		{ "printb",Instruction::PRINTB },
		{ "pushi",Instruction::PUSHI },
		{ "pushf",Instruction::PUSHF },
		{ "pushb",Instruction::PUSHB },
		{ "pop",Instruction::POP },
		{ "toi",Instruction::TOI },
		{ "tof",Instruction::TOF },
		{ "tob",Instruction::TOB },
		{ "nagi",Instruction::NAGI },
		{ "nagf",Instruction::NAGF },
		{ "not",Instruction::NOT },
		{ "or",Instruction::OR },
		{ "and",Instruction::AND },
		{ "eqi",Instruction::EQI },
		{ "eqf",Instruction::EQF },
		{ "eqb",Instruction::EQB },
		{ "gti",Instruction::GTI },
		{ "gtf",Instruction::GTF },
		{ "lti",Instruction::LTI },
		{ "ltf",Instruction::LTF },
		{ "addi",Instruction::ADDI },
		{ "addf",Instruction::ADDF },
		{ "subi",Instruction::SUBI },
		{ "subf",Instruction::SUBF },
		{ "muli",Instruction::MULI },
		{ "mulf",Instruction::MULF },
		{ "divi",Instruction::DIVI },
		{ "divf",Instruction::DIVF },
		{ "new",Instruction::NEW },
		{ "ma",Instruction::MA },
		{ "rfhm",Instruction::RFHM },
		{ "wthm",Instruction::WTHM }
	};

	std::set<int> needInt =
	{
		Instruction::JAL,
		Instruction::JIFF,
		Instruction::PUSHI,
		Instruction::NEW,
		Instruction::MA
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
	int line = 0;
	while (true)
	{
		line++;

		Instruction i;

		string str;
		if (!(in >> str))
			break;
		if (str == "#" || str == "{" || str == "}")
		{
			std::getline(in, str);
			continue;
		}
		cout << str;

		if (str == "btd")
		{
			StructureMetaBuilder::beginStructureDeclaration();
			int id;
			in >> id;

			while (true)
			{
				line++;
				int memberType = -1;
				in >> str;
				if (str == "bool")
					memberType = 1;
				if (str == "float")
					memberType = 2;
				if (str == "int")
					memberType = 3;
				if (str == "structure")
					memberType = 4;
				if (str == "etd")
					break;
				assert(memberType != -1);
				StructureMetaBuilder::addMemeber(memberType);
			}
			int eid;
			in >> eid;
			assert(id == eid);
			auto meta = StructureMetaBuilder::build();
			assert(m_structureMetas.find(id) == m_structureMetas.end());
			m_structureMetas.insert(make_pair(id, meta));
			continue;
		}





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
		pc2line.insert(make_pair(codeAddr, line));

		codeAddr++;
		cout << endl;
	}
	Instruction i;
	i.opcode = Instruction::HALT;
	m_ins.push_back(i);
	strins.insert(make_pair(codeAddr, "halt"));
	in.close();
}

inline void VirtualMachine::executeInstructions()
{
	//m_pc = m_lable2addr[0];
	mainloop();
}

inline void VirtualMachine::init()
{
	m_lable2addr.clear();
	m_opnds = list<Variable>();
	for (int i = 0; i < 1000; i++)
	{
		m_globalVariables.push_back(Variable());
		m_memory.push_back(Variable());
	}
	m_ins.clear();
	m_pc = 0;
	m_fp = 0;
}

inline void VirtualMachine::mainloop()
{
	while (true)
	{
		if (verbose)
			cout << "Line: " << pc2line[m_pc] << "," << strins[m_pc] << endl;
		cout << " PC: " << m_pc;
		cout << " ,Instruction: " + strins[m_pc] << endl;
		auto in = m_ins[m_pc++];
		if (in.opcode == Instruction::HALT)
			break;
		dispatch(in);
	}
}

inline void VirtualMachine::dispatch(Instruction in)
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
	case Instruction::RFHM:
		doRfhm();
		break;
	case Instruction::WTHM:
		doWthm();
		break;
	case Instruction::NEW:
		doNew(in.ival);
		break;
	case Instruction::MA:
		doMa(in.ival);
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
	case Instruction::NOT:
		doNot();
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
