#pragma once
class TranslationUnit;
class NewExpression;
class MemberAccess;
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
class StructureDeclaration;
class ExpressionStatement;
class ReturnStatement;
class WhileStatement;
class BreakStatement;
class ContinueStatement;
class Object;
class Visitor
{
public:
	virtual ~Visitor() {}
	virtual void visitTranslationUnit(TranslationUnit* unit) = 0;

	virtual void visitMemberAccessExpression(MemberAccess* exp) = 0;
	virtual void visitNewExpression(NewExpression* exp) = 0;
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
	virtual void visitWhileStatement(WhileStatement* stmt) = 0;
	virtual void visitBreakStatement(BreakStatement* stmt) = 0;
	virtual void visitContinueStatement(ContinueStatement* stmt) = 0;



	virtual void visitVariableDeclaration(VariableDeclaration* decl) = 0;
	virtual void visitFunctionDeclaration(FunctionDeclaration* decl) = 0;
	virtual void visitStructureDeclaration(StructureDeclaration* decl) = 0;

	

};

