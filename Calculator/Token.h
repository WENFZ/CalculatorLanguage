#pragma once
#include <string>
class Token
{
public:
	enum
	{
		END,
		// ����
		LPAR = '(',
		RPAR = ')',
		LBRACE = '{',
		RBRACE = '}',
		SEMICOLON = ';',
		COMMA=',',

		
		ADD = '+',
		SUB = '-',
		MUL = '*',
		DIV = '/',


		// ��ֵ�����
		ASSIGN = '=',

		// �Ƚ������
		GT = '>',
		LT = '<',
		EQ=128,			// ==
		NE,			// !=
		GE,			// >=
		LE,			// <=

		INT,
		FLOAT,
		BOOL,
		STRUCT,
		NEW,

		AND ,
		OR ,
		NOT ,
		DOT,

		IF,
		THEN,
		ELSE,

		WHILE,
		DO,
		BREAK,
		CONTINUE,

		RETURN,
		PRINT,
		IDENTIFIER,
		
		
		FUNCTION,


		ICONSTANCE,
		FCONSTANCE,
		TRUE,
		FALSE,

	};
	Token(int type, int row, int col, std::string value = "")
	{
		m_type = type;
		m_row = row;
		m_col = col;
		m_value = value;
	}
	
	int m_type;
	std::string m_value;
	int m_row;
	int m_col;
};
