#include "Parser.h"
#include "Token.h"
#include "AST.h"
#include "Type.h"
#include "Scope.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define checkType1(exp,type) \
do\
{\
	if(exp->getType()->to##type()==nullptr)\
	{\
		\
		error("type error\n");\
	}\
}while(0);
#define checkType2(exp,type1,type2) \
do\
{\
	if(exp->getType()->to##type1()==nullptr&&exp->getType()->to##type2()==nullptr)\
	{\
		error("type error\n");\
	}\
}while(0);
#define IsCompatible(exp1,exp2) \
do\
{\
	if (!exp1->getType()->isCompatible(exp2->getType()))\
	{\
		error("incompatible types\n");\
	}\
}while(0);





Parser::Parser(std::list<Token*> tokens)
{
	m_ts = tokens;
	m_pos = m_ts.begin();
	m_whileStatement = nullptr;
	m_funDef = nullptr;
	m_scope = nullptr;
}

Parser::~Parser()
{
}

void Parser:: error(std::string msg)
{
	cout << "error: at row " << (*m_pos)->m_row << ",col " << (*m_pos)->m_col << "," << msg;

}

int Parser::peekTokenType()
{
	return (*m_pos)->m_type;
}
std::string Parser::peekTokenValue()
{
	return (*m_pos)->m_value;
}
bool Parser::test(int type)
{
	return peekTokenType() == type;
}
int Parser::next()
{
	return (*(++m_pos))->m_type;
}
void Parser::expect(int type)
{
	if (peekTokenType() != type)
	{
		if (type == ';')
			cout<<"error: at row "<< (*m_pos)->m_row <<",col "<< (*m_pos)->m_col <<",missing ;\n";
		if (type == ')')
			cout << "error: at row " << (*m_pos)->m_row << ",col " << (*m_pos)->m_col << ",missing )\n";
		if (type == Token::THEN)
			cout << "error: at row " << (*m_pos)->m_row << ",col " << (*m_pos)->m_col << ",missing keyword then\n";
		
	}
	else
	{
		next();
	}
}
void Parser::savePosition()
{
	m_postions.push_back(m_pos);
}
void Parser::restorePosition()
{
	m_pos = m_postions.back();
	m_postions.pop_back();
}
void Parser::discardPosition()
{
	m_postions.pop_back();
}
void Parser::enterNewScope(int type)
{
	m_scope = new Scope(type,m_scope);
}
void Parser::leaveScope()
{
	auto outer = m_scope->getOuterScope();
	delete m_scope;
	m_scope = outer;
}

TranslationUnit* Parser::parseTranslationUnit()
{
	enterNewScope(Scope::UNIT);
	auto unit = new TranslationUnit();
	while (peekTokenType() != Token::END)
	{
		auto s = parseStatement();
		cout << s->name + " was parsed\n";
		unit->addStmt(s);
	}
	leaveScope();
	return unit;
}

Statement* Parser::parseStatement()
{
	switch (peekTokenType())
	{
	case Token::LBRACE:
		return parseCompoundStatement();
	case Token::IF:
		return parseIfStatement();
	case Token::RETURN:
		return parseReturnStatement();
	case Token::PRINT:
		return parsePrintStatement();
	case Token::INT:
	case Token::BOOL:
	case Token::FLOAT:
		error("missing keyword var or function\n");
	case Token::VAR:
		return parseVariableDeclaration();
	case Token::FUNCTION:
		return parseFunctionDeclaration();
	default:
		return parseExpressionStatement();
	}
}

Statement* Parser::parseCompoundStatement()
{
	expect('{');
	enterNewScope(Scope::COUPOUNDSTATEMENT);
	auto cs = new CompoundStatement;
	while (!attempt('}'))
	{
		auto stmt = parseStatement();
		cs->addStatement(stmt);
	}
	leaveScope();
	return cs;
}

Statement* Parser::parseIfStatement()
{
	cout << "parsing if statement\n";
	expect(Token::IF);
	auto expression = parseExpression();
	if (!expression->getType()->toBool())
	{
		error(" bool expression is expected,but get " + expression->getType()->name + " within if statement\n");
	}
	expect(Token::THEN);
	auto statement = parseStatement();
	Statement* elseStatement = nullptr;
	if (attempt(Token::ELSE))
		elseStatement = parseStatement();
	return new IfStatement(expression, statement, elseStatement);
}


Statement* Parser::parseReturnStatement()
{
	cout << "parsing return statement\n";
	expect(Token::RETURN);
	Expression* exp = parseExpression();
	expect(';');
	assert(exp != nullptr);
	if (m_funDef == nullptr)
	{
		error("return statement is not in function body\n");
	}
	else
	{
		if(!m_funSignature->getReturnType()->isCompatible(exp->getType()))
		{
			error("types of return value and that of proto do not match\n");
		}
	}
	return new ReturnStatement(exp, m_funDef);
}

Statement* Parser::parsePrintStatement()
{
	auto exp=parseExpression();
	expect(';');
	return new PrintStatement(exp);
}


Statement* Parser::parseExpressionStatement()
{
	if (attempt(';'))
		return new ExpressionStatement();
	auto exp = parseExpression();
	expect(';');
	return new ExpressionStatement(exp);
}


Declaration* Parser::parseVariableDeclaration()
{
	expect(Token::VAR);
	static std::map<int, Type*> types =
	{
		{Token::INT,Int::newInstance()},
		{Token::FLOAT,Float::newInstance()},
		{Token::BOOL,Bool::newInstance()},
	};
	Type* type = Int::newInstance();
	if (types.find(peekTokenType()) != types.end())
	{
		type = types[peekTokenType()];
		next();
	}
	else
	{
		error("unsupported type in variable declaration");
	}
	Identifier* id = nullptr;
	if (test(Token::IDENTIFIER))
	{
		if (m_scope->inCurScope(peekTokenValue()))
		{
			error("redefine identifier " + peekTokenValue());
		}
		else
		{
			id = Identifier::newInstance(type, peekTokenValue());
			m_scope->addIdentifier(peekTokenValue(), id);
		}
		next();
	}
	else
	{
		error("no identifier was found in variable declaration");
	}
	expect('=');
	auto value = parseExpression();
	if (!type->isCompatible(value->getType()))
	{
		error("unmatched types " + type->name + " and " + value->getType()->name + " in variable declaration");
	}
	expect(';');
	return new VariableDeclaration(id, value);
}
Declaration* Parser::parseFunctionDeclaration()
{
	expect(Token::FUNCTION);
	if (m_scope->getScopeType() != Scope::UNIT)
	{
		error("can only declare functions in unit scope");
	}
	auto type = parseFunctionType();
	m_funSignature = type->toFunction();
	auto funname = peekTokenValue();
	expect(Token::IDENTIFIER);
	Identifier* id = nullptr;
	if (m_scope->hasIdentifier(funname))
	{
		id = m_scope->getIdentifier(funname);
		auto funtype = id->getType()->toFunction();
		if (funtype == nullptr)
		{
			error("identifier was not function but is declared as function now");
			// 将旧的类型替换成新的类型
			//m_scope->modifiedIdentifier(val,)
			// todo
			assert(0);
		}
		else
		{
			if (!funtype->isCompatible(type))
			{
				error("unmatched types " + funtype->name + " and " + type->name);
			}
		}
	}
	else
	{
		// 首次出现
		id = Identifier::newInstance(type, funname);
		m_scope->addIdentifier(funname, id);
	}
	auto fundef=m_funDef = new FunctionDeclaration(id);
	if (attempt('='))
	{
		// function body
		parseFunctionBody();
	}
	m_funDef = nullptr;
	return fundef;
}
void Parser::parseFunctionBody()
{
	enterNewScope(Scope::FUNCTION);
	vector<string> args;
	vector<Identifier*> arglist;
	while (!attempt(':'))
	{
		auto name = peekTokenValue();
		args.push_back(name);
		next();
		if (!test(':'))
			expect(',');
	}
	auto argtypes = m_funSignature->getArgTypes();
	if (args.size() != argtypes.size())
	{
		error("in function decalaration,numbers of types and of args do not match\n");
		int len = std::min(argtypes.size(), args.size());
		for (int i = 0; i < len; i++)
		{
			auto id = Identifier::newInstance(argtypes[i], args[i]);
			m_scope->addIdentifier(args[i], id);
			arglist.push_back(id);
		}
		for (int i = len; i < args.size();i++)
		{
			auto id = Identifier::newInstance(Int::newInstance(), args[i]);
			m_scope->addIdentifier(args[i], id);
			arglist.push_back(id);
		}
	}
	else
	{
		int len = args.size();
		for (int i = 0; i < len; i++)
		{
			m_scope->addIdentifier(args[i], Identifier::newInstance(argtypes[i], args[i]));
		}
	}
	auto body=parseStatement();
	m_funDef->setArglist(arglist);
	m_funDef->setBody(body);
	leaveScope();
}
Expression* Parser::parseExpression()
{
	auto exp=parseAssignmentExpression();
	cout << "expression "+exp->name+" parsed\n";
	return exp;
}

Expression* Parser::parseAssignmentExpression()
{
	auto lhs = parseBinaryExpression();
	assert(lhs != nullptr);
	if (attempt('='))
	{
		auto rhs = parseAssignmentExpression();
		if (!lhs->isLval())
		{
			error(lhs->name + " is not a left value\n");
		}
		assert(rhs != nullptr);
		if (lhs->getType() != rhs->getType())
		{
			error("invalid operation at " + lhs->getType()->name + " and " + rhs->getType()->name + "\n");
		}
		lhs = new BinaryExpression(lhs, rhs, BinaryExpression::ASSIGN);
	}
	return lhs;
}

// precosition: lhs and rhs exist
// return value: never nullptr,throw assertion error if lhs or rhs is nullptr 
Expression* Parser::parseBinaryExpression(int level)
{
	// or
	// and
	// == !=
	// > < >= <= 
	// + -
	// * /

	static vector <set<int>> optrTree =
	{
		{Token::OR},// 0
		{Token::AND},// 1
		{Token::EQ,Token::NE},// 2
		{Token::GE,Token::GT,Token::LE,Token::LT},// 3
		{Token::ADD,Token::SUB},// 4
		{Token::MUL,Token::DIV},// 5
	};
	static map<int, set<Type*>> typeChecks =
	{
		{Token::OR,{Bool::newInstance()}},// or
		{Token::AND,{Bool::newInstance()}},// and

		{Token::EQ,{Bool::newInstance(),Int::newInstance(),Float::newInstance()}},// ==
		{Token::NE,{Bool::newInstance(),Int::newInstance(),Float::newInstance()}},// !=

		{Token::GE,{Int::newInstance(),Float::newInstance()}},// >=
		{Token::GT,{Int::newInstance(),Float::newInstance()}},// >
		{Token::LE,{Int::newInstance(),Float::newInstance()}},// <=
		{Token::LT,{Int::newInstance(),Float::newInstance()}},// <

		{Token::ADD,{Int::newInstance(),Float::newInstance()}},// +
		{Token::SUB,{Int::newInstance(),Float::newInstance()}},// -

		{Token::MUL,{Int::newInstance(),Float::newInstance()}},// *
		{Token::DIV,{Int::newInstance(),Float::newInstance()}},// /
	};
	
	
	Expression* lhs = nullptr;
	if (level == optrTree.size() - 1)
		lhs = parseUnaryExpression();
	else
		lhs = parseBinaryExpression(level + 1);
	assert(lhs != nullptr);

	Expression* last = nullptr;
	while (last != lhs)
	{
		last = lhs;
		for (auto& optr:optrTree[level])
		{
			if (attempt(optr))
			{
				Expression* rhs = nullptr;
				if (level == optrTree.size() - 1)
					rhs = parseUnaryExpression();
				else
					rhs = parseBinaryExpression(level + 1);
				assert(rhs != nullptr);
				if (!(typeChecks[optr].find(lhs->getType()) != typeChecks[optr].end()&&lhs->getType()==rhs->getType()))
				{
					error("invalid operation at " + lhs->getType()->name + " and "+rhs->getType()->name+"\n");
				}

				lhs = new BinaryExpression(lhs, rhs, optr);
				cout << "binary expression " + lhs->name + " parsed\n";

			}
		}
	}
	return lhs;
}

// precosition: opnd exists
// return value: never nullptr,throw assertion error if opnd is nullptr
Expression* Parser::parseUnaryExpression()
{
	/*unary - expression:
		primary expression
		unaryoperator castexpression

		unaryoperator:+ - ! int float bool
	*/
	std::set<int> optrs =
	{
		Token::ADD,// +
		Token::SUB,// -
		Token::NOT,// !
		Token::INT,// int
		Token::FLOAT,// float
		Token::BOOL,// bool
	};
	std::map<int, set<Type*>> typeChecks =
	{
		{Token::ADD,{Int::newInstance(),Float::newInstance()}},// +
		{Token::SUB,{Int::newInstance(),Float::newInstance()}},// -
		{Token::NOT,{Bool::newInstance()}},// !
		{Token::INT,{Int::newInstance(),Float::newInstance(),Bool::newInstance()}},// int
		{Token::FLOAT,{Int::newInstance(),Float::newInstance(),Bool::newInstance()}},// float
		{Token::BOOL,{Int::newInstance(),Float::newInstance(),Bool::newInstance()}},// bool
	};
	for (auto optr : optrs)
	{
		if (attempt(optr))
		{
			auto exp = parseUnaryExpression();
			assert(exp != nullptr);
			if (typeChecks[optr].find(exp->getType()) != typeChecks[optr].end())
			{
				auto opnd=new UnaryExpression(exp, optr);
				cout << "unary expression " + opnd->name + " parsed\n";
				return opnd;
			}
			else
			{
				error("invalid operation at " + exp->getType()->name + " \n");
				return exp;
			}
		}
	}
	auto exp=parsePrimaryExpression();
	cout << "primary expression " + exp->name << " parsed\n";
	return exp;
}

Expression* Parser::parsePrimaryExpression()
{
	if (attempt('('))
	{
		auto exp = parseExpression();
		assert(exp != nullptr);
		expect(')');
		return exp;
	}
	if (attempt(Token::TRUE))
	{
		return new BConstance(true);
	}
	if (attempt(Token::FALSE))
	{
		return new BConstance(false);
	}
	if (test(Token::FCONSTANCE))
	{
		auto val = peekTokenValue();
		next();
		return new FConstance(atof(val.c_str()));
	}
	if (test(Token::ICONSTANCE))
	{
		auto val = peekTokenValue();
		next();
		return new IConstance(atoi(val.c_str()));
	}
	if (test(Token::IDENTIFIER))
	{
		auto id = m_scope->getIdentifier(peekTokenValue());
		
		if (id == nullptr)
		{
			error("identifier is used before declaration\n");
			next();
			return new IConstance(0);
			//return new NConstance();
		}

		if (id->getType()->toFunction() != nullptr)
		{
			return parseFunctionCall();
		}
		// a object
		return parseObject();
	}
	error("null expression\n");
	assert(0);
	return nullptr;
}

Expression* Parser::parseFunctionCall()
{
	auto funname = m_scope->getIdentifier(peekTokenValue());
	auto types = funname->getType()->toFunction();
	assert(types != nullptr);
	next();

	// 1、parse params
	expect('(');
	vector<Expression*> params;
	while (!attempt(')'))
	{
		auto p = parseExpression();
		params.push_back(p);
		if (!test(')'))
			expect(',');
	}

	// 2、check types
	auto argtypes = types->getArgTypes();
	if (params.size() != argtypes.size())
	{
		error("number of params and number of args do not match when calling function\n");
	}
	else
	{
		for (int i = 0; i < params.size(); i++)
		{
			if (!argtypes[i]->isCompatible(params[i]->getType()))
			{
				error("incompatible types of " + argtypes[i]->name + " and " + params[i]->name + " \n");
			}
		}
	}

	return new FunctionCall(funname, params);
}

Expression* Parser::parseObject()
{
	auto name = peekTokenValue();
	cout << "object " + name + " parsed\n";

	expect(Token::IDENTIFIER);
	auto id = m_scope->getIdentifier(name);
	return new Object(id);
}

Type* Parser::parseBaseType()
{
	static std::map<int, Type*> types =
	{
		{Token::INT,Int::newInstance()},
		{Token::FLOAT,Float::newInstance()},
		{Token::BOOL,Bool::newInstance()},
	};
	Type* type = Int::newInstance();
	if (types.find(peekTokenType()) != types.end())
	{
		type = types[peekTokenType()];
		next();
	}
	else
	{
		error("unsupported base type");
	}
	return type;
}

Type* Parser::parseFunctionType()
{
	auto returnType = parseBaseType();

	expect('(');
	std::vector<Type*> argTypes;
	while (!attempt(')'))
	{
		auto argType = parseBaseType();
		argTypes.push_back(argType);
		if(!test(')'))
			expect(',');
	}
	auto type = Function::newInstance(returnType, argTypes);
	return type;
}

