#pragma once
#include <ostream>
#include <string>
#include <iostream>
#include <cassert>
#include <fstream>
class CodeOutput
{
	std::ofstream out;
	bool m_hasCout;
public:
	CodeOutput(const char* p,bool hascout=true)
	{
		//const char* p = "C:\\Users\\wen\\Desktop\\tcc\\asm.txt";
		m_hasCout = hascout;
		out.open(p);
		assert(out.is_open());

	}
	~CodeOutput()
	{
		out.close();
	}
	std::ostream& operator<<(std::string codeseg)
	{
		if(m_hasCout)
			std::cout << codeseg << std::endl;
		out << codeseg<<std::endl;
		return out;
	}
};

