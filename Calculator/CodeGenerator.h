#pragma once
#include <list>
#include <map>
#include <cassert>
#include "Visitor.h"
#include "CodeOutput.h"
#include "InstructionSet.h"

class Identifier;
class VariableCnter
{
public:
	void addVariable(Identifier* var)
	{

	}
	int getOffset(Identifier* var)
	{
		if (m_offsets.find(var) != m_offsets.end())
			return m_offsets[var];
		// 不在当前作用域内
		assert(m_outter != nullptr);
		//return m_outter->getOffset()
		return 0;
		
	}
	int getSize()
	{
		return cnter;
	}
	int cnter = 0;
	std::map<Identifier*,int> m_offsets;
	VariableCnter* m_outter;

};
class CodeGenerator:public Visitor
{
public:
	int newLable()
	{
		static int i = 0;
		return i++;
	}
	void addFunction(Identifier* funname)
	{
		
	}
	int getFunction(Identifier* funname)
	{
		return 0;
	}
	void addVariable(Identifier* id)
	{

	}
	int getVariable(Identifier* id)
	{
		return 0;
	}
	int getTop()
	{
		return 0;
	}
	int varCnter;
	
	VariableCnter* m_gloablVariables;

	int funAddr;
	CodeOutput output;
	InstructionSet s;
	virtual void visitTranslationUnit(TranslationUnit* unit);

	virtual void visitBinaryExpression(BinaryExpression* exp);
	virtual void visitUnaryExpression(UnaryExpression* exp);
	virtual void visitFunctionCall(FunctionCall* exp);
	virtual void visitIConstance(IConstance* exp);
	virtual void visitFConstance(FConstance* exp);
	virtual void visitBConstance(BConstance* exp);
	virtual void visitObject(Object* obj);

	virtual void visitIfStatement(IfStatement* stmt);
	virtual void visitPrintStatement(PrintStatement* stmt);
	virtual void visitReturnStatement(ReturnStatement* stmt);
	virtual void visitCompoundStatement(CompoundStatement* stmt);
	virtual void visitExpressionStatement(ExpressionStatement* stmt);

	virtual void visitVariableDeclaration(VariableDeclaration* decl);
	virtual void visitFunctionDeclaration(FunctionDeclaration* decl);

	
};

