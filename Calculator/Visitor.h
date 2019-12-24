#pragma once
class TranslationUnit;
class BinaryExpression;
class UnaryExpression;
class IConstance;
class FConstance;
class BConstance;
class FunctionCall;
class IfStatement;
class PrintStatement;
class CompoundStatement;
class VariableDeclaration;
class FunctionDeclaration;
class ExpressionStatement;
class ReturnStatement;
class Object;
class Visitor
{
public:
	virtual ~Visitor() {}
	virtual void visitTranslationUnit(TranslationUnit* unit) = 0;


	virtual void visitBinaryExpression(BinaryExpression* exp) = 0;
	virtual void visitUnaryExpression(UnaryExpression* exp) = 0;
	virtual void visitFunctionCall(FunctionCall* exp) = 0;
	virtual void visitIConstance(IConstance* exp) = 0;
	virtual void visitFConstance(FConstance* exp) = 0;
	virtual void visitBConstance(BConstance* exp) = 0;

	virtual void visitObject(Object* obj) = 0;


	virtual void visitIfStatement(IfStatement* stmt) = 0;
	virtual void visitPrintStatement(PrintStatement* stmt) = 0;
	virtual void visitReturnStatement(ReturnStatement* stmt) = 0;
	virtual void visitCompoundStatement(CompoundStatement* stmt) = 0;
	virtual void visitExpressionStatement(ExpressionStatement* stmt) = 0;

	virtual void visitVariableDeclaration(VariableDeclaration* decl) = 0;
	virtual void visitFunctionDeclaration(FunctionDeclaration* decl) = 0;

	

};

