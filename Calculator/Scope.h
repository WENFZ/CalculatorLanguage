#pragma once
#include <map>
#include <string>
#include <unordered_map>

class Identifier;
class Scope
{

public:
	enum
	{
		UNIT,
		FUNCTION,
		COUPOUNDSTATEMENT,
	};
	static int instanceCnt;
	Scope(int type,Scope* outer = nullptr);
	~Scope();
	void addIdentifier(std::string val, Identifier* identifier);
	bool inCurScope(std::string val);
	bool hasIdentifier(std::string val);
	void modifiedIdentifier(std::string val,Identifier* identifier);
	int getScopeType();
	Identifier* getIdentifier(std::string value);
	Scope* getOuterScope();
private:
	Scope* m_outerScope;
	std::unordered_map<std::string,Identifier*> m_identifiers;
	int m_type;
};

