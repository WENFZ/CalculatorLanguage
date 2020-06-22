#include "HeapManager.h"
#include <queue>
#include <iostream>
std::vector<int> StructureMetaBuilder::structureMeta = std::vector<int>();
bool StructureMetaBuilder::building = false;

//void MemoryManager::setRootSetSource(std::function<std::vector<Variable>> getter)
//{
//	getRootSet = getter;
//}

MemoryManager::~MemoryManager()
{
}

Object* NaiveMemoryManager::newObject(std::vector<int> meta)
{
	auto obj = new Object();
	Variable values[4];
	values[0].bval = false;
	values[0].type = Variable::BOOL;
	
	values[1].fval = 0;
	values[1].type = Variable::FLOAT;

	values[2].ival = 0;
	values[2].type = Variable::INT;

	values[3].oval = nullptr;
	values[3].type = Variable::REFERENCE;

	for (auto member : meta)
	{	
		obj->m_members.push_back(values[member]);
	}
	m_objs.push_back(obj);
	return obj;
}

Variable NaiveMemoryManager::memberAccess(Object* obj, int offset)
{
	Variable var;
	var.type = Variable::ADDR;
	var.addr = &(obj->m_members[offset]);
	return var;
}

NaiveMemoryManager::~NaiveMemoryManager()
{
	for (auto obj : m_objs)
	{
		delete obj;
	}
}

Object* GCMemoryManager::newObject(std::vector<int> meta)
{
	if (timeToGC())
	{
		GC();
	}
	return NaiveMemoryManager::newObject(meta);
}


GCMemoryManager::~GCMemoryManager()
{
}

bool GCMemoryManager::timeToGC()
{
	return true;// always true
}

void GCMemoryManager::GC()
{
	mark();
	sweep();
}

void GCMemoryManager::mark()
{
	auto rootSet = m_getRootset();
	// bfs
	std::queue<Object*> q;// for unmarked
	for (auto obj : rootSet) 
	{
		q.push(obj);
	}
	while (!q.empty())
	{
		auto node = q.front();
		q.pop();
		node->marked = true;
		auto children = node->m_members;
		for (auto child : children)
		{
			if (child.type != Variable::REFERENCE)
				continue;
			if (child.oval==nullptr||child.oval->marked)
				continue;
			q.push(child.oval);
		}
	}
}

void GCMemoryManager::sweep()
{
	int cnt = 0;
	for (auto it = m_objs.begin(); it != m_objs.end();)
	{
		if ((*it)->marked)
		{
			(*it)->marked = false;
			it++;
		}
		else
		{
			std::cout << "an object having " << (*it)->m_members.size() << " members is going to be collected\n";
			delete (*it);
			it = m_objs.erase(it);
			cnt++;
		}
	}
	std::cout << cnt << " objects were collected\n";
}
