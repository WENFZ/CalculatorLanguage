#include "CodeGenerator.h"
#include "AST.h"
void CodeGenerator::visitTranslationUnit(TranslationUnit* unit)
{
	for (auto s : unit->m_stmts)
	{
		if (s->toVariableDeclaration() != nullptr)
		{
			// 全局变量声明
			m_gloablVariables->addVariable(s->toVariableDeclaration()->m_id);
		}
		else
		{
			s->accept(this);
		}
	}
}
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
	exp->m_opnd1->accept(this);
	exp->m_opnd2->accept(this);

	int optr = exp->m_optr;
	switch (optr)
	{
	case BinaryExpression::OR:
		output << s.Or();
		break;
	case BinaryExpression::AND:
		output << s.And();
		break;

	case BinaryExpression::EQ:
	case BinaryExpression::NE:
		if (exp->getType()->toInt() != nullptr)
			output << s.Eqi();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Eqf();
		else if (exp->getType()->toBool() != nullptr)
			output << s.Eqb();
		else
			assert(0);

		if(optr==BinaryExpression::NE)
			output << s.Not();
		break;
		
	case BinaryExpression::GT:
	case BinaryExpression::LE:
		if (exp->getType()->toInt() != nullptr)
			output << s.Gti();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Gtf();
		else
			assert(0);

		if (optr == BinaryExpression::LE)
			output << s.Not();
		break;

	case BinaryExpression::LT:
	case BinaryExpression::GE:
		if (exp->getType()->toInt() != nullptr)
			output << s.Lti();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Ltf();
		else
			assert(0);

		if (optr == BinaryExpression::GE)
			output << s.Not();
		break;

	case BinaryExpression::ADD:
		if (exp->getType()->toInt() != nullptr)
			output << s.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Addf();
		else
			assert(0);
		break;
	case BinaryExpression::SUB:
		if (exp->getType()->toInt() != nullptr)
			output << s.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Addf();
		else
			assert(0);
		break;
	case BinaryExpression::MUL:
		if (exp->getType()->toInt() != nullptr)
			output << s.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Addf();
		else
			assert(0);
		break;
	case BinaryExpression::DIV:
		if (exp->getType()->toInt() != nullptr)
			output << s.Addi();
		else if (exp->getType()->toFloat() != nullptr)
			output << s.Addf();
		else
			assert(0);
		break;
	case BinaryExpression::ASSIGN:
		exp->m_opnd1->accept(this);// addr
		exp->m_opnd2->accept(this);// rval
		
	default:
		break;
	}
}

void CodeGenerator::visitUnaryExpression(UnaryExpression* exp)
{
	/*POS = Token::ADD,
		NAG = Token::SUB,
		NOT = Token::NOT,
		INT = Token::INT,
		FLOAT = Token::FLOAT,
		BOOL = Token::BOOL*/
	exp->m_opnd->accept(this);
	switch (exp->m_optr)
	{
	case UnaryExpression::NAG:
		if (exp->getType()->toInt() != nullptr)
		{
			output << s.Nagi();
			
		}
		else if (exp->getType()->toFloat() != nullptr)
		{
			output << s.Nagf();
			
		}
		else
		{
			assert(0);
		}
		break;
	case UnaryExpression::POS:
		break;// do nothing
	case UnaryExpression::NOT:
		output << s.Not();
		break;
	case UnaryExpression::INT:
		output << s.Toi();
		break;
	case UnaryExpression::FLOAT:
		output << s.Tof();
		break;
	case UnaryExpression::BOOL:
		output << s.Tob();
		break;
	default:
		assert(0);
		break;
	}
}

void CodeGenerator::visitFunctionCall(FunctionCall* exp)
{
	output << s.PushReg();// 保存fp
	
	for (auto param : exp->m_params)
	{
		param->accept(this);
	}
	output << s.Jal(getFunction(exp->m_name));
	output << s.PopReg();// 恢复fp
}

void CodeGenerator::visitIConstance(IConstance* exp)
{
	output << s.Pushi(exp->m_value);
}

void CodeGenerator::visitFConstance(FConstance* exp)
{
	output << s.Pushf(exp->m_value);
}

void CodeGenerator::visitBConstance(BConstance* exp)
{
	output << s.Pushb(exp->m_value);
}

void CodeGenerator::visitObject(Object* obj)
{
	//output << s.Loadi(getVariable(obj->m_id));
}

void CodeGenerator::visitIfStatement(IfStatement* stmt)
{
	stmt->m_condition->accept(this);
	auto elseLable = newLable();
	output << s.Jiff(elseLable);
	stmt->m_thenStatement->accept(this);
	output << s.Lable(elseLable);
	if (stmt->m_thenStatement != nullptr)
		stmt->m_elseStatement->accept(this);
}

void CodeGenerator::visitPrintStatement(PrintStatement* stmt)
{
	stmt->m_expression->accept(this);
	auto exp = stmt->m_expression;
	if (exp->getType()->toInt() != nullptr)
		output << s.Printi();
	else if (exp->getType()->toFloat() != nullptr)
		output << s.Printf();
	else if (exp->getType()->toBool() != nullptr)
		output << s.Printb();
	else
		assert(0);
}

void CodeGenerator::visitReturnStatement(ReturnStatement* stmt)
{
	stmt->m_expression->accept(this);
	output << s.Jr();
}

void CodeGenerator::visitCompoundStatement(CompoundStatement* stmt)
{
	for (auto s : stmt->m_stmts)
		s->accept(this);
}

void CodeGenerator::visitExpressionStatement(ExpressionStatement* stmt)
{
	stmt->m_expression->accept(this);
}

void CodeGenerator::visitVariableDeclaration(VariableDeclaration* decl)
{
	addVariable(decl->m_id);
}

void CodeGenerator::visitFunctionDeclaration(FunctionDeclaration* decl)
{
	if (getFunction(decl->m_id) == -1)
	{
		addFunction(decl->m_id);
	}
	if (decl->m_body != nullptr)
	{
		for (auto arg : decl->m_args)
		{
			addVariable(arg);

		}
		decl->m_body->accept(this);
	}
}
