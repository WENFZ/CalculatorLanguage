#include "Text.h"
#include <cassert>
std::string s = "F:\\VS2017ÏîÄ¿\\Toy\\Toy\\Text.cpp";
Text::Text(std::string filename)
{
	FILE* f = fopen(filename.c_str(), "r");
	assert(f != NULL);
	int c;
	while ((c = fgetc(f)) != EOF)
	{
		m_text.push_back(c);
	}
	m_text.push_back(0);
	fclose(f);
	m_pos = 0;
	m_row = 0;
	m_col = 0;
}

int Text::peek()
{
	return m_text[m_pos];
}

int Text::next()
{
	int c = m_text[++m_pos];
	if (c == '\n')
	{
		m_row++;
		m_col = 0;
	}
	m_col++;
	return c;
}

