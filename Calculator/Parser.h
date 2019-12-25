#pragma once
#include <list>
#include <string>

class Token;

class Statement;
class Expression;
class TranslationUnit;
class Type;
class Scope;
class Declaration;
class Function;
class FunctionDeclaration;
class Parser
{
public:
	Parser(std::list<Token*> tokens);
	~Parser();
	void error(std::string);

	int peekTokenType();
	std::string peekTokenValue();
	bool attempt(int type)
	{
		if (peekTokenType() == type)
		{
			next();
			return true;
		}
		return false;
	}
	bool test(int type);
	int next();
	void expect(int type);
	std::list<Token*> m_ts;
	std::list<Token*>::iterator m_pos;
	std::list<std::list<Token*>::iterator> m_postions;

	Scope* m_scope;
	void savePosition();
	void restorePosition();
	void discardPosition();
	void enterNewScope(int type);
	void leaveScope();

	Statement* m_whileStatement;
	FunctionDeclaration* m_funDef;
	Function* m_funSignature;
	

	TranslationUnit* parseTranslationUnit();
	
	Statement* parseStatement();
	Statement* parseCompoundStatement();
	Statement* parseIfStatement();
	Statement* parseReturnStatement();
	Statement* parsePrintStatement();
	Statement* parseExpressionStatement();
	
	Expression* parseExpression();
	Expression* parseAssignmentExpression();
	Expression* parseBinaryExpression(int level=0);
	Expression* parseUnaryExpression();
	Expression* parsePrimaryExpression();
	Expression* parseFunctionCall();
	Expression* parseObject();

	Declaration* parseVariableDeclaration();
	Declaration* parseFunctionDeclaration();
	void parseFunctionBody();
	
	Type* parseBaseType();
	Type* parseFunctionType();

};

