#pragma once
#include <string>
#include <list>
using namespace std;
class Text;
class Token;



class Lexer
{
public:
	Lexer(Text* text)
	{
		m_text = text;
	}
	~Lexer();
	void Tokenize();

	int peek();
	int next();
	bool attempt(int ch);



	bool skipWhitespace();
	bool skipComment();


	Token* readToken();
	Token* readNumber();
	Token* readIdentifierOrKeyword();

	Text* m_text;
	list<Token*> m_ts;
};

