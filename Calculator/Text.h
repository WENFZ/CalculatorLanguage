#pragma once
#include <string>
class Text
{
public:
	Text(std::string filename);
	int peek();
	int next();
	int m_row;
	int m_col;
private:
	std::string m_text;
	int m_pos;
	
};

