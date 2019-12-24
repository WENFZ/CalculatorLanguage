#include "Lexer.h"
#include "Text.h"
#include "Token.h"
#include <map>
#include <cassert>
static map<string, int>keywords =
{
	{"if",Token::IF},
	{"then",Token::THEN},
	{"else",Token::ELSE},

	{"int",Token::INT},
	{"float",Token::FLOAT},
	{"bool",Token::BOOL},
	{"true",Token::TRUE},
	{"false",Token::FALSE},

	{"var",Token::VAR},
	{"function",Token::FUNCTION},

	{"return",Token::RETURN},
	{"print",Token::PRINT},

	{"or",Token::OR},
	{"and",Token::AND},
	{"not",Token::NOT}
};


Lexer::~Lexer()
{
	for (auto t : m_ts)
		delete t;
}

void Lexer::Tokenize()
{
	while (true)
	{
		auto token = readToken();
		
		m_ts.push_back(token);
		if (token->m_type == Token::END)
			break;
	}
}

int Lexer::peek()
{
	return m_text->peek();
}

int Lexer::next()
{
	return m_text->next();
}

bool Lexer::attempt(int ch)
{
	if (m_text->peek() == ch)
	{
		m_text->next();
		return true;
	}
	return false;
}

bool Lexer::skipWhitespace()
{
	int ch = m_text->peek();
	bool res = (ch == '\t' || ch == ' ' || ch == '\n');
	while (true)
	{
		int ch = m_text->peek();
		if (ch == '\t' || ch == ' ' || ch == '\n')
			m_text->next();
		else
			break;
	}
	return res;
}

bool Lexer::skipComment()
{
	if (attempt('#'))
	{
		while (!attempt('#'))
		{
			next();
		}
		return true;
	}
	return false;
}

Token* Lexer::readToken()
{
	while (skipWhitespace() || skipComment())
	{

	}
	int c = peek();
	
	switch (c)
	{
	case 0:
		return new Token(Token::END, m_text->m_row, m_text->m_col);
	case '(':
	case ')':
	case '{':
	case '}':
	case ';':
	case '+':
	case '-':
	case '*':
	case '/':
	case ':':
	case ',':
		m_text->next();
		return new Token(c, m_text->m_row, m_text->m_col, string(1, (char)c));
	case '!':
		m_text->next();
		if (attempt('='))
			return new Token(Token::NE, m_text->m_row, m_text->m_col, "!=");
		else
			return new Token(Token::NOT, m_text->m_row, m_text->m_col, "=");
	case '=':
		m_text->next();
		if(attempt('='))
			return new Token(Token::EQ, m_text->m_row, m_text->m_col,"==");
		else
			return new Token('=', m_text->m_row, m_text->m_col, "=");
	case '>':
		m_text->next();
		if (attempt('='))
			return new Token(Token::GE, m_text->m_row, m_text->m_col, ">=");
		else
			return new Token('>', m_text->m_row, m_text->m_col, ">");
	case '<':
		m_text->next();
		if (attempt('='))
			return new Token(Token::LE, m_text->m_row, m_text->m_col, "<=");
		else
			return new Token('<', m_text->m_row, m_text->m_col, "<");
	default:
		if(isdigit(c))
			return readNumber();
		if(isalpha(c)||c=='_')
			return readIdentifierOrKeyword();
		// real default:
		assert(0);

	}
	assert(0);
	return nullptr;
}

Token* Lexer::readNumber()
{
	std::string value = "";
	auto type = Token::ICONSTANCE;
	for (int ch = peek(); isdigit(ch); ch = next())
	{
		value.push_back(ch);
	}
	if (attempt('.'))
	{
		value.push_back('.');
		type = Token::FCONSTANCE;
		for (int ch = peek(); isdigit(ch); ch = next())
		{
			value.push_back(ch);
		}
	}
	return new Token(type,m_text->m_row, m_text->m_col,value);
}

Token* Lexer::readIdentifierOrKeyword()
{
	std::string value = "";
	for (int ch = peek(); isalnum(ch) || ch == '_'; ch = next())
	{
		value.push_back(ch);
	}
	int type = Token::IDENTIFIER;
	if (keywords.find(value) != keywords.end())
		type = keywords[value];
	return new Token(type, m_text->m_row, m_text->m_col, value);
}
