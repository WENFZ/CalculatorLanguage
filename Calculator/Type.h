#pragma once
#include <list>
#include <set>
#include <unordered_set>
#include <string>
#include <vector>
class Function;
class Int;
class Float;
class Bool;
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
	static int instanceCnt;

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
};

