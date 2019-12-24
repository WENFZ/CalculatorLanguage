#include "CodeGenerator.h"
#include "AST.h"

void CodeGenerator::visitBinaryExpression(BinaryExpression* exp)
{
	//  OR = Token::OR,
	//	AND = Token::AND,

	//	EQ = Token::EQ,
	//	NE = Token::NE,
	//	GE = Token::GE,				// >=
	//	LE = Token::LE,				// <=
	//	GT = Token::GT,				// >
	//	LT = Token::LT,				// <

	//	ADD = Token::ADD,			// +
	//	SUB = Token::SUB,			// -
	//	MUL = Token::MUL,			// *
	//	DIV = Token::DIV,			// /


	//	ASSIGN = Token::ASSIGN,			// =
	output << "# bianry expression:" + exp->name;
	output << "# calculate opnd1";
	exp->m_opnd1->accept(this);
	output << "# calculate opnd2";
	exp->m_opnd2->accept(this);
	output << "# calculate result";
	int optr = exp->m_optr;
	switch (optr)
	{
	case BinaryExpression::OR:
	{
		output << is.Or();
		break;
	}
	case BinaryExpression::AND:
	{
		output << is.And();
		break;
	}
	case BinaryExpression::EQ:
	case BinaryExpression::NE:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Eqi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Eqf();
		else if (exp->getType()->toBool() != nullptr)
			output << is.Eqb();
		else
			assert(0);

		if (optr == BinaryExpression::NE)
			output << is.Not();
		break;
	}
	case BinaryExpression::GT:
	case BinaryExpression::LE:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Gti();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Gtf();
		else
			assert(0);

		if (optr == BinaryExpression::LE)
			output << is.Not();
		break;
	}
	case BinaryExpression::LT:
	case BinaryExpression::GE:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Lti();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Ltf();
		else
			assert(0);

		if (optr == BinaryExpression::GE)
			output << is.Not();
		break;
	}
	case BinaryExpression::ADD:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Addf();
		else
			assert(0);
		break;
	}
	case BinaryExpression::SUB:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Addf();
		else
			assert(0);
		break;
	}
	case BinaryExpression::MUL:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Addf();
		else
			assert(0);
		break;
	}
	case BinaryExpression::DIV:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Addf();
		else
			assert(0);
		break;
	}
	case BinaryExpression::ASSIGN:
	{
		exp->m_opnd2->accept(this);// rval
		exp->m_opnd1->accept(this);// addr
		if (m_localVariables->hasVariable(m_lastVisitedVar))
			output << is.Wtsm();
		else
			output << is.Wtgm();
	}
	default:
		assert(0);
		break;
	}
	output << "# end of binary expression:" + exp->name;
}

void CodeGenerator::visitUnaryExpression(UnaryExpression* exp)
{
	/*POS = Token::ADD,
		NAG = Token::SUB,
		NOT = Token::NOT,
		INT = Token::INT,
		FLOAT = Token::FLOAT,
		BOOL = Token::BOOL*/
	output << "# unary expression:" + exp->name;
	output << "# calculate opnd";
	exp->m_opnd->accept(this);
	output << "# calculate result";
	switch (exp->m_optr)
	{
	case UnaryExpression::NAG:
	{
		if (exp->getType()->toInt() != nullptr)
			output << is.Nagi();
		else if (exp->getType()->toFloat() != nullptr)
			output << is.Nagf();
		else
			assert(0);
		break;
	}
	case UnaryExpression::POS:
		break;// do nothing
	case UnaryExpression::NOT:
		output << is.Not();
		break;
	case UnaryExpression::INT:
		output << is.Toi();
		break;
	case UnaryExpression::FLOAT:
		output << is.Tof();
		break;
	case UnaryExpression::BOOL:
		output << is.Tob();
		break;
	default:
		assert(0);
		break;
	}
	output << "# end of unary expression:" + exp->name;
}

void CodeGenerator::visitFunctionCall(FunctionCall* exp)
{
	output << "# function call :" + exp->name;
	// 1、将各个参数入栈
	output << "# push params from right to left";
	for (auto it=exp->m_params.rbegin();it!=exp->m_params.rend();it++)
	{
		// 从右到左入栈
		output << "# pushing param "+(*it)->name;
		(*it)->accept(this);
	}
	// 2、上调FP
	output << "# adjust FP for the function";
	int offset = m_localVariables->getCurScopeVarNum();
	output << is.Pushi(offset);
	output << is.IncFP();
	// 3、将调用地址入栈
	//output << s.Pushi(g(exp->m_name));
	output << is.Jal(getFunctionAddr(exp->m_name));
	// 4、函数调用完成，下调FP
	output << "# function returned,restore the FP";
	output << is.Pushi(offset);
	output << is.DecFP();
	output << "# end of function call";
}

void CodeGenerator::visitIConstance(IConstance* exp)
{
	output << is.Pushi(exp->m_value);
}

void CodeGenerator::visitFConstance(FConstance* exp)
{
	output << is.Pushf(exp->m_value);
}

void CodeGenerator::visitBConstance(BConstance* exp)
{
	output << is.Pushb(exp->m_value);
}

void CodeGenerator::visitObject(Object* obj)
{
	// 将obj对应的内存地址或值放在操作数栈上
	m_lastVisitedVar = obj->m_id;
	if (m_localVariables->hasVariable(m_lastVisitedVar))
	{
		output << "# local object access: " + obj->name;
		int offset = m_localVariables->getOffsetFromCurFP(m_lastVisitedVar);
		output << is.Pushi(offset);
		if (!obj->inLeft())
		{
			output << is.Rfsm();
		}
		output << "# end of local object access: " + obj->name;
	}
	else
	{
		output << "# global object access: " + obj->name;
		int offset = m_gloablVariables->getOffsetFromCurFP(m_lastVisitedVar);
		output << is.Pushi(offset);
		if (!obj->inLeft())
			output << is.Rfgm();
		output << "# end of global object access: " + obj->name;
	}
}

void CodeGenerator::visitIfStatement(IfStatement* stmt)
{
	output << "# if statement:";
	output << "# calculate condition value:";
	stmt->m_condition->accept(this);
	auto elseLable = newLable();
	auto endif = newLable();
	output << "# test condition";
	output << is.Jiff(elseLable);
	output << "# then statement";
	stmt->m_thenStatement->accept(this);
	output << "# jump to endif";
	output << is.Pushi(endif);
	output << is.Jl();
	output << "# else statement";
	output << is.Lable(elseLable);
	if (stmt->m_elseStatement != nullptr)
		stmt->m_elseStatement->accept(this);
	output << is.Lable(endif);
	output << "# end of if statement";
}

void CodeGenerator::visitPrintStatement(PrintStatement* stmt)
{
	output << "# print statement:"+stmt->m_expression->name;
	output << "# calculate value to be printed";
	stmt->m_expression->accept(this);
	auto exp = stmt->m_expression;
	if (exp->getType()->toInt() != nullptr)
		output << is.Printi();
	else if (exp->getType()->toFloat() != nullptr)
		output << is.Printf();
	else if (exp->getType()->toBool() != nullptr)
		output << is.Printb();
	else
		assert(0);
	output << "# end of print statement: " + stmt->m_expression->name;
}

void CodeGenerator::visitReturnStatement(ReturnStatement* stmt)
{
	output << "# return statement: ";
	output << "# calculate the return value";
	stmt->m_expression->accept(this);
	output << "# adjust the FP for potential variables declared within the function";
	output << is.Pushi(m_localVariables->getScopeChainVarNum());// 1 for return address
	output << is.DecFP();
	output << "# fetch return address from stack memory";
	output << is.Pushi(1);
	output << is.DecFP();
	output << is.Pushi(0);
	output << is.Rfsm();
	output << "# jump to return address";
	output << is.Jump();
	output << "# end of return statement";
}

void CodeGenerator::visitCompoundStatement(CompoundStatement* stmt)
{
	output << "# CompoundStatement";
	output << "# adjust FP";
	int offset = m_localVariables->getCurScopeVarNum();
	output << is.Pushi(offset);
	output << is.IncFP();
	auto outer = m_localVariables;
	m_localVariables = new VariableCnter(outer);
	output << "# CompoundStatement body";
	for (auto s : stmt->m_stmts)
	{
		s->accept(this);
	}
	output << "# end of CompoundStatement body";
	delete m_localVariables;
	m_localVariables = outer;
	output << "# adjust FP";
	output << is.Pushi(offset);
	output << is.DecFP();
	output << "# end of CompoundStatement";
}

void CodeGenerator::visitExpressionStatement(ExpressionStatement* stmt)
{
	if (stmt->m_expression != nullptr)
	{
		output << "# expression statement";
		stmt->m_expression->accept(this);
		output << is.Pop();
		output << "# end of expression statement";
	}
}

void CodeGenerator::visitVariableDeclaration(VariableDeclaration* decl)
{
	// only for local variable
	output << "# local variable declaration: "+decl->name;
	output << "# codes for initializer";
	decl->m_value->accept(this);
	m_localVariables->addVariable(decl->m_id);
	output << "# move variable value from opnd stack to stack memory";
	output << is.Pushi(m_localVariables->getOffsetFromCurFP(decl->m_id));
	output << is.Wtsm();
	output << is.Pop();
	output << "# end of local variable declaration " + decl->name;
}

void CodeGenerator::visitFunctionDeclaration(FunctionDeclaration* decl)
{
	if (getFunctionAddr(decl->m_id) == -1)
	{
		addFunction(decl->m_id);
	}
	if (decl->m_body != nullptr)
	{
		output << "# global function declaration:";
		output << "# "+decl->name;
		m_funAddr = getFunctionAddr(decl->m_id);
		output << "# function address";
		output << is.Lable(m_funAddr);
		// 1、将操作数栈上的返回地址保存到栈内存
		output << "# save return address";
		output << is.Pushi(0);
		output << is.Wtsm();
		// 2、弹出操作数栈上的返回地址
		output << "# pop up return address on the opnd stack";
		output << is.Pop();
		// 3、将FP上调1
		output << "# shadow the fp for return address";
		output << is.Pushi(1);
		output << is.IncFP();
		// 4、保存各个参数
		auto old = m_localVariables;
		m_localVariables = new VariableCnter(m_localVariables);
		for (auto arg : decl->m_args)
		{
			output << "# move arg "+arg->name+" to stack memory";
			m_localVariables->addVariable(arg);
			output << is.Pushi(m_localVariables->getOffsetFromCurFP(arg));
			output << is.Wtsm();
			output << is.Pop();
		}
		// 5、正式执行函数体
		output << "# codes for function body";
		decl->m_body->accept(this);
		output << "# end of function: " + decl->name;
		delete m_localVariables;
		m_localVariables = old;
	}
	
}

void CodeGenerator::addFunction(Identifier* funname)
{
	if (funname->name == "main")
		m_funAddrs.insert(std::make_pair(funname, 0));
	else
		m_funAddrs.insert(std::make_pair(funname, newLable()));
}

void CodeGenerator::visitTranslationUnit(TranslationUnit* unit)
{
	for (auto s : unit->m_stmts)
	{
		if (s->toVariableDeclaration() != nullptr)
		{
			// 全局变量声明
			output << "# global variable declaration: "+s->name;
			s->toVariableDeclaration()->m_value->accept(this);
			m_gloablVariables->addVariable(s->toVariableDeclaration()->m_id);
			output << is.Pushi(m_gloablVariables->getOffsetFromCurFP(s->toVariableDeclaration()->m_id));
			output << is.Wtgm();
			output << is.Pop();
			output << "# end of global variable declaration: " + s->name;
		}
		else
		{
			s->accept(this);
		}
	}
}