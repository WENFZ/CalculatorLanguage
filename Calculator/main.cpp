
// CSubset.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Type.h"
#include "Text.h"
#include "Lexer.h"
#include "Parser.h"
#include "AST.h"
#include <list>
#include "Scope.h"
#include "CodeGenerator.h"
using namespace std;


int main()
{
	


	const char* file = "C:\\Users\\wen\\Desktop\\tcc\\main.txt";
	//const char* file = "C:\\Users\\wen\\Desktop\\DeclarationTestCases.txt";



	Text text(file);
	Lexer lexer(&text);
	lexer.Tokenize();
	Parser* parser= new Parser(lexer.m_ts);
	auto u=parser->parseTranslationUnit();
	CodeGenerator g;
	cout << "code:\n";
	u->accept(&g);
	
	delete u;
	delete parser;
	Identifier::deleteAllInstances();
	Function::deleteAllInstances();
	int i = 5;
}

