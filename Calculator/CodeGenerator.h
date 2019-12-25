#pragma once
#include <list>
#include <map>
#include <cassert>
#include <stack>
#include "Visitor.h"
#include "CodeOutput.h"
#include "InstructionSet.h"

class Identifier;
class VariableCnter
{
public:
	VariableCnter(VariableCnter* outer = nullptr)
	{
		m_outer = outer;
	}
	
	
	VariableCnter* getOuter()
	{
		return m_outer;
	}
	void addVariable(Identifier* var)
	{
		m_offsets.insert(std::make_pair(var, cnter++));
	}
	bool hasVariable(Identifier* var)
	{
		if (m_offsets.find(var) != m_offsets.end())
			return true;
		return m_outer != nullptr && m_outer->hasVariable(var);
	}
	// 正数如果var在FP指针之后，否则负数
	int getOffsetFromCurFP(Identifier* var)
	{
		if (m_offsets.find(var) != m_offsets.end())
			return m_offsets[var];
		else
			return m_outer->getOffsetFromNextFP(var);
	}
	int getCurScopeVarNum()
	{
		return cnter;
	}
	int getScopeChainVarNum(VariableCnter* originPoint=nullptr)
	{
		assert(this != originPoint);
		return m_outer->getScopeChainVarNumHelper(originPoint);
	}
private:
	int cnter = 0;
	std::map<Identifier*,int> m_offsets;
	VariableCnter* m_outer;
	// 总是负数，因为var总在下一FP之前
	int getOffsetFromNextFP(Identifier* var)
	{
		if (m_offsets.find(var) != m_offsets.end())
			return m_offsets[var]-cnter;
		else
			return -cnter + m_outer->getOffsetFromNextFP(var);
	}
	int getScopeChainVarNumHelper(VariableCnter* originPoint)
	{
		if (this == originPoint)
			return cnter;
		else
			return m_outer==nullptr ?0:(m_outer->getScopeChainVarNumHelper(originPoint) + cnter);

	}
};

class CodeGenerator:public Visitor
{
public:
	CodeGenerator(const char* p,bool out):output(p,out)
	{
		m_localVariables = new VariableCnter;
		m_gloablVariables = new VariableCnter();
	}
	~CodeGenerator()
	{
		delete m_localVariables;
		delete m_gloablVariables;
		
	}
	virtual void visitTranslationUnit(TranslationUnit* unit);

private:
	// lable manager
	int newLable()
	{
		static int i = 1;
		return i++;
	}
private:
	// identifier manager
	VariableCnter* m_localVariables;
	VariableCnter* m_gloablVariables;
	Identifier* m_lastVisitedVar;
	
	VariableCnter* m_whileScope;


private:
	// function manager
	void addFunction(Identifier* funname);
	int getFunctionAddr(Identifier* funname)
	{
		if (m_funAddrs.find(funname) != m_funAddrs.end())
			return m_funAddrs[funname];
		return -1;
	}
	std::map<Identifier*, int> m_funAddrs;
	int m_funAddr;
private:
	int m_testLable;
	int m_endLable;

private:

	CodeOutput output;

	InstructionSet is;


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
	virtual void visitWhileStatement(WhileStatement* stmt);
	virtual void visitBreakStatement(BreakStatement* stmt);
	virtual void visitContinueStatement(ContinueStatement* stmt);

	virtual void visitVariableDeclaration(VariableDeclaration* decl);
	virtual void visitFunctionDeclaration(FunctionDeclaration* decl);

	
};

