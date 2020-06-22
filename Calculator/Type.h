#pragma once
#include <list>
#include <set>
#include <unordered_set>
#include <string>
#include <vector>
#include <cassert>
class Function;
class Int;
class Float;
class Bool;
class Structure;
class Type
{
protected:
	Type() {};
	virtual ~Type() {};
public:
	
	virtual Function* toFunction() { return nullptr; }
	virtual Int* toInt() { return nullptr; }
	virtual Float* toFloat() { return nullptr; }
	virtual Bool* toBool() { return nullptr; }
	virtual Structure* toStructure(){ return nullptr; }
	virtual bool isCompatible(Type* other) { return this == other; }
	std::string name;
};


class Int :public Type
{
	Int()
	{
		name = "int";
	}
public:
	static Int* newInstance()
	{
		static Int instance;
		return &instance;
	}
	virtual Int* toInt() { return this; }
};

class Float :public Type
{
	Float()
	{
		name = "float";
	}
public:
	static Float* newInstance()
	{
		static Float instance;
		return &instance;
	}
	virtual Float* toFloat() { return this; }
};


class Bool :public Type
{
	Bool()
	{
		name = "bool";
	}
public:
	virtual Bool* toBool() { return this; }
	static Bool* newInstance()
	{
		static Bool instance;
		return &instance;
	}
};


class Function :public Type
{
	Function(Type* returnType, std::vector<Type*> argTypes)
	{
		m_returnType = returnType;
		m_argTypes = argTypes;
		name = "function takes (";
		for (auto arg : argTypes)
			name += arg->name + ",";
		*name.rbegin() = ')';
		name += "returning (" + returnType->name+")";
	}
public:

	Type* getReturnType()
	{
		return m_returnType;
	}
	std::vector<Type*> getArgTypes()
	{
		return m_argTypes;
	}
	
	virtual Function* toFunction() { return this; }
	static Function* newInstance(Type* returnType, std::vector<Type*> argTypes)
	{
		Function* ins = new Function(returnType, argTypes);
		instances.insert(ins);
		instanceCnt++;
		return ins;
	}
	static void deleteAllInstances()
	{
		for (auto i : instances)
		{
			delete i;
			instanceCnt--;
		}
		instances.clear();
	}
	virtual bool isCompatible(Type* other)
	{
		if (other == nullptr)
			return false;
		if (other == this)
			return true;
		auto fun = other->toFunction();
		if (fun == nullptr)
			return false;
		if (!m_returnType->isCompatible(fun->m_returnType))
			return false;
		auto arglist = fun->m_argTypes;
		if (m_argTypes.size() != arglist.size())
			return false;
		for (int i = 0; i < m_argTypes.size(); i++)
		{
			if (!m_argTypes[i]->isCompatible(arglist[i]))
				return false;
		}
		return true;
	}
	
private:
	Type* m_returnType;
	std::vector<Type*> m_argTypes;
	static std::set<Function*> instances;
	static int instanceCnt;
};

class Structure :public Type 
{
public:
	static Structure* newInstance() 
	{
		auto inst = new Structure();
		instances.insert(inst);
		return inst;
	}
	int getMemberIndex(std::string name)
	{
		for (int i = 0; i < m_members.size(); i++)
		{
			if (m_members[i].first == name)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}
	bool addMember(Type* type, std::string name) 
	{
		for (auto it = m_members.begin(); it != m_members.end(); it++)
		{
			auto memberName = it->first;
			if (memberName == name)
			{
				return false;
			}
		}
		m_members.push_back(std::make_pair(name, type));
		instanceCnt++;
		return true;
	}
	bool hasMember(std::string name)
	{
		for (auto it = m_members.begin(); it != m_members.end(); it++)
		{
			auto memberName = it->first;
			if (memberName == name)
			{
				return true;
			}
		}
		return false;
	}
	Type* getMemberType(std::string name)
	{
		for (auto it = m_members.begin(); it != m_members.end(); it++)
		{
			auto memberName = it->first;
			if (memberName == name)
			{
				return it->second;
			}
		}
		return nullptr;
	}
	std::vector<std::pair<std::string, Type*>>& getMembers()
	{
		return m_members;
	}

	virtual Structure* toStructure() { return this; }
	bool m_finished;
private:
	std::vector<std::pair<std::string,Type*>> m_members;
	static std::set<Structure*> instances;
	static int instanceCnt;
};
