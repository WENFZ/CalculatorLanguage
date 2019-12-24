#pragma once
#include <list>
#include <string>
#include <cassert>
#include "Token.h"
#include "Type.h"
#include "Visitor.h"
class Visitor;
class Object;
class Function;
class Statement;
class Expression;
class Declaration;
class FunctionDeclaration;
class VariableDeclaration;
using namespace std;
class ASTNode
{
	static int instanceCnt;
public:
	ASTNode()
	{
		instanceCnt++;
	}
	virtual void accept(Visitor* visitor)=0;
	virtual ~ASTNode() { instanceCnt--; }
	std::string name;// for debugging

};
class Identifier 
{
	Identifier(Type* type, string value)
	{
		m_type = type;
		name = value;
	}
	~Identifier()
	{

	}
public:
	std::string name;

	virtual Type* getType() { return m_type; }
	virtual bool isLval() { return true; }
	static Identifier* newInstance(Type* type, string value)
	{
		auto i = new Identifier(type, value);
		instances.push_back(i);
		return i;
	}
	static void deleteAllInstances()
	{
		for (auto i : instances)
			delete i;
		instances.clear();
	}
	static list<Identifier*> instances;
	
	Type* m_type;

};
class Expression :public ASTNode
{
public:
	virtual Type* getType() { return nullptr; }
	virtual Object* toObject()
	{
		return nullptr;
	}
};
class IConstance :public Expression
{
public:
	virtual Type* getType()
	{
		return Int::newInstance();
	}
	IConstance(int value)
	{
		m_value = value;
		name = to_string(value);
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitIConstance(this);
	}

	int m_value;
};
class BConstance :public Expression
{
public:
	virtual Type* getType()
	{
		return Bool::newInstance();
	}
	BConstance(bool value)
	{
		m_value = value;
		if (value)
			name = "true";
		else
			name = "false";

	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitBConstance(this);
	}
	bool m_value;
};
class FConstance :public Expression
{
public:
	virtual Type* getType()
	{
		return Float::newInstance();
	}
	FConstance(double value)
	{
		m_value = value;
		name = to_string(value);
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitFConstance(this);
	}
	double m_value;
};
class Object :public Expression
{
public:
	Object(Identifier* id,bool inLeft=false)
	{
		m_id = id;
		name = id->name;
		m_inLeft = inLeft;
	}
	virtual Type* getType()
	{
		return m_id->getType();
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitObject(this);
	}
	bool inLeft() 
	{ 
		return m_inLeft; 
	}
	virtual Object* toObject()
	{
		return this;
	}
	bool m_inLeft;
	Identifier* m_id;
	
};
class FunctionCall :public Expression
{
public:
	FunctionCall(Identifier* name,std::vector<Expression*> params)
	{
		m_name = name;
		m_type = name->getType()->toFunction();
		m_params = params;
	}
	~FunctionCall()
	{
		for (auto p : m_params)
			delete p;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitFunctionCall(this);
	}
	virtual Type* getType()
	{
		return m_type->toFunction()->getReturnType();
	}
	Identifier* m_name;
	Function* m_type;
	std::vector<Expression*> m_params;
};
class UnaryExpression :public Expression
{
public:
	enum
	{
		POS = Token::ADD,
		NAG = Token::SUB,
		NOT = Token::NOT,
		INT = Token::INT,
		FLOAT = Token::FLOAT,
		BOOL = Token::BOOL
	};
	UnaryExpression(Expression* opnd, int optr)
	{
		m_optr = optr;
		m_opnd = opnd;
		if (optr == NAG)
			name = "-" + opnd->name;
		if (optr == NOT)
			name = "!" + opnd->name;
		if (optr == INT)
			name = "int" + opnd->name;
		if (optr == FLOAT)
			name = "float" + opnd->name;
		if (optr == BOOL)
			name = "bool" + opnd->name;
	}
	~UnaryExpression()
	{
		delete m_opnd;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitUnaryExpression(this);
	}
	virtual Type* getType()
	{
		switch (m_optr)
		{
		case NAG:
			return m_opnd->getType();
		case NOT:
			return Bool::newInstance();
		case POS:
			return m_opnd->getType();
		case BOOL:
			return Bool::newInstance();
		case INT:
			return Int::newInstance();
		case FLOAT:
			return Float::newInstance();
		default:
			assert(0);
			return nullptr;
			break;
		}
	}
	int m_optr;
	Expression* m_opnd;
};
class BinaryExpression :public Expression
{
public:
	enum
	{
		OR = Token::OR,
		AND = Token::AND,

		EQ = Token::EQ,
		NE = Token::NE,
		GE = Token::GE,				// >=
		LE = Token::LE,				// <=
		GT = Token::GT,				// >
		LT = Token::LT,				// <

		ADD = Token::ADD,			// +
		SUB = Token::SUB,			// -
		MUL = Token::MUL,			// *
		DIV = Token::DIV,			// /


		ASSIGN = Token::ASSIGN,			// =
	};
	BinaryExpression(Expression* opnd1, Expression* opnd2, int optr)
	{
		m_optr = optr;
		m_opnd1 = opnd1;
		m_opnd2 = opnd2;

		if (optr == OR)
			name = opnd1->name+ " or " + opnd2->name;
		if (optr == AND)
			name = opnd1->name + " and " + opnd2->name;
		if (optr == EQ)
			name = opnd1->name + " == " + opnd2->name;
		if (optr == NE)
			name = opnd1->name + " != " + opnd2->name;
		if (optr == GE)
			name = opnd1->name + " >= " + opnd2->name;
		if (optr == GT)
			name = opnd1->name + ">" + opnd2->name;
		if (optr == LT)
			name = opnd1->name + "<" + opnd2->name;
		if (optr == LE)
			name = opnd1->name + "<=" + opnd2->name;
		if (optr == ADD)
			name = opnd1->name + " + " + opnd2->name;
		if (optr == SUB)
			name = opnd1->name + " - " + opnd2->name;
		if (optr == MUL)
			name = opnd1->name + "*" + opnd2->name;
		if (optr == DIV)
			name = opnd1->name + "/" + opnd2->name;
	}
	~BinaryExpression()
	{
		delete m_opnd1;
		delete m_opnd2;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitBinaryExpression(this);
	}
	virtual Type* getType()
	{
		switch (m_optr)
		{
		case OR:
		case AND:
		case EQ:
		case NE:
		case GE:
		case GT:
		case LE:							
		case LT:		
			return Bool::newInstance();
		case ADD:			
		case SUB:			
		case MUL:			
		case DIV:
			return m_opnd1->getType();
		default:
			assert(0);
			return nullptr;
		}
	}
	int m_optr;
	Expression* m_opnd1;
	Expression* m_opnd2;

};

class Statement :public ASTNode
{
public:
	virtual VariableDeclaration* toVariableDeclaration() { return nullptr; }
};
class IfStatement :public Statement
{
public:
	IfStatement(Expression* condition, Statement* thenStatement,Statement* elseStatement=nullptr)
	{
		m_condition = condition;
		m_thenStatement = thenStatement;
		m_elseStatement = elseStatement;
		name = "if statement";
	}
	~IfStatement()
	{
		delete m_condition;
		delete m_thenStatement;
		delete m_elseStatement;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitIfStatement(this);
	}
	Expression* m_condition;
	Statement* m_thenStatement;
	Statement* m_elseStatement;
};

class ExpressionStatement :public Statement
{
public:
	ExpressionStatement(Expression* exp=nullptr)
	{
		m_expression = exp;
	}
	~ExpressionStatement()
	{
		delete m_expression;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitExpressionStatement(this);
	}
	Expression* m_expression;// null if it is a null statement;
};
class CompoundStatement :public Statement
{
public:
	CompoundStatement()
	{
		name = "compound statement";

	}
	void addStatement(Statement* stmt)
	{
		m_stmts.push_back(stmt);
	
	}
	~CompoundStatement()
	{
		for (auto stmt : m_stmts)
			delete stmt;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitCompoundStatement(this);
	}
	std::list<Statement*> m_stmts;
};


class ReturnStatement :public Statement
{
public:
	ReturnStatement(Expression* exp,FunctionDeclaration* functionDef)
	{
		m_expression = exp;
		m_funDef = functionDef;
	}
	~ReturnStatement()
	{
		delete m_expression;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitReturnStatement(this);
	}
	FunctionDeclaration* m_funDef;
	Expression* m_expression;
};
class PrintStatement :public Statement
{
public:
	PrintStatement(Expression* exp)
	{
		m_expression = exp;
	}
	~PrintStatement()
	{
		delete m_expression;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitPrintStatement(this);
	}
	Expression* m_expression;
};
class Declaration :public Statement
{

};
class DeclarationStatement :public Statement
{
public:
	DeclarationStatement(Type* type)
	{
		m_type = type;
	}
	void addDeclaration(Declaration* declaration)
	{
		m_declarations.push_back(declaration);
	}
	~DeclarationStatement()
	{
		for (auto d : m_declarations)
			delete d;
		
	}
	std::list<Declaration*> m_declarations;
	Type* m_type;
};


class VariableDeclaration :public Declaration
{
public:
	VariableDeclaration(Identifier* id, Expression* value)
	{
		m_value = value;
		m_id = id;
		name = "VariableDeclaration: " + id->getType()->name + " " + id->name +" "+ value->name;
	}
	~VariableDeclaration()
	{
		delete m_value;
	}
	virtual VariableDeclaration* toVariableDeclaration() { return this; }
	virtual void accept(Visitor* visitor)
	{
		visitor->visitVariableDeclaration(this);
	}
	Identifier* m_id;
	Type* m_type;
	Expression* m_value;
};
class FunctionDeclaration :public Declaration
{
public:
	FunctionDeclaration(Identifier* id)
	{
		m_type = id->getType();
		m_body = nullptr;
		m_id = id;
		name = "FunctionDeclaration: " + m_type->name + " " + id->name;
	}
	~FunctionDeclaration()
	{
		delete m_body;
		//delete m_id;
	} 
	virtual void accept(Visitor* visitor)
	{
		visitor->visitFunctionDeclaration(this);
	}
	void setArglist(vector<Identifier*> args)
	{
		m_args = args;
	}
	void setBody(Statement* body)
	{
		m_body = body;
	}
	vector<Identifier*> m_args;
	Type* m_type;
	Identifier* m_id;
	Statement* m_body;
};


class TranslationUnit :public ASTNode
{
public:
	void addStmt(Statement* stmt)
	{
		m_stmts.push_back(stmt);
	}
	~TranslationUnit()
	{
		for (auto s : m_stmts)
			delete s;
	}
	virtual void accept(Visitor* visitor)
	{
		visitor->visitTranslationUnit(this);
	}
	std::list<Statement*> m_stmts;
};