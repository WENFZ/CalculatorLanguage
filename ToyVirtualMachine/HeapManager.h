#pragma once
#include <vector>
#include <functional>
#include <list>
#include <set>
struct Object;
struct Variable
{
	Variable()
	{
		type = NONE;
	}
	Variable(const Variable& var)
	{
		type = var.type;
		if (type == INT)
			ival = var.ival;
		if (type == BOOL)
			bval = var.bval;
		if (type == FLOAT)
			fval = var.fval;
		if (type == REFERENCE)
			oval = var.oval;
		if (type == ADDR)
			addr = var.addr;
		
	}
	enum
	{
		NONE,
		INT,
		BOOL,
		FLOAT,
		REFERENCE,
		ADDR,
	};
	int type;
	union
	{
		int ival;
		double fval;
		bool bval;
		Object* oval;
		
		Variable* addr;// only for opnd and mm
	};
};

struct GCAble
{
	bool marked;
};


struct Object:GCAble
{
	std::vector<Variable> m_members;
};

class StructureMetaBuilder
{
public:
	static std::vector<int> structureMeta;
	static bool building;
	static void beginStructureDeclaration()
	{
		building = true;
		StructureMetaBuilder::structureMeta.clear();
	}
	static void addMemeber(int type)
	{
		StructureMetaBuilder::structureMeta.push_back(type);
	}
	static std::vector<int> build()
	{
		return StructureMetaBuilder::structureMeta;
	}
};


class MemoryManager
{
public:
	//std::function<std::vector<Variable>> getRootSet;
	//void setRootSetSource(std::function<std::vector<Variable>> getter);
	virtual Object* newObject(std::vector<int> meta) = 0;
	virtual Variable memberAccess(Object* obj, int offset) = 0;
	virtual ~MemoryManager();
};

class NaiveMemoryManager :public MemoryManager
{
public:
	virtual Object* newObject(std::vector<int> meta);
	virtual Variable memberAccess(Object* obj, int offset);
	virtual ~NaiveMemoryManager();
	std::list<Object*> m_objs;
	
};

class GCMemoryManager :public NaiveMemoryManager
{
public:
	std::function<std::set<Object*>()> m_getRootset;

	virtual Object* newObject(std::vector<int> meta);
	virtual ~GCMemoryManager();

	bool timeToGC();
	void GC();
	void mark();
	void sweep();
};