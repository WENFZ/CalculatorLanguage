#include "Scope.h"
#include "AST.h"
#include <string>
int Scope::instanceCnt = 0;
Scope::Scope(int type, Scope* outer)
{
	instanceCnt++;
	m_outerScope = outer;
	m_identifiers = std::unordered_map<std::string, Identifier*>();
	m_type = type;
}
Scope::~Scope()
{
	instanceCnt--;
}
void Scope::addIdentifier(std::string val, Identifier* identifier)
{
	m_identifiers.insert(make_pair(val,identifier));
}
bool Scope::inCurScope(std::string val)
{
	return m_identifiers.find(val) != m_identifiers.end();
}
bool Scope::hasIdentifier(std::string identifier)
{
	bool inCur = inCurScope(identifier);
		return inCur||(m_outerScope != nullptr ? m_outerScope->hasIdentifier(identifier) : false);
}

void Scope::modifiedIdentifier(std::string val, Identifier* identifier)
{
	// todo
	assert(0);
}

int Scope::getScopeType()
{
	return m_type;
}

Identifier* Scope::getIdentifier(std::string value)
{

	for (auto& id : m_identifiers)
	{
		if (id.first == value)
			return id.second;
	}
	if (m_outerScope != nullptr)
		return m_outerScope->getIdentifier(value);
	else
		return nullptr;
}

Scope* Scope::getOuterScope()
{
	return m_outerScope;
}
