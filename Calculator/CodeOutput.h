#pragma once
#include <ostream>
#include <string>
#include <iostream>
class CodeOutput
{
public:
	std::ostream& operator<<(std::string codeseg)
	{
		std::cout << codeseg << std::endl;
		return std::cout;
	}
};

