// ToyVirtualMachine.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <functional>
#include "VirualMachine.h"
#include "HeapManager.h"
using namespace std;


int main()
{
    vector<int> v;
    v.push_back(1);
    v.push_back(1);
    *(&(v[1])) = 2;
    VirtualMachine vm;
    auto mm = new GCMemoryManager();
    vm.verbose = false;
    vm.m_mm = mm;
    


    std::function<std::set<Object*>()> callable = [&vm]()->set<Object*> {
        return vm.getRootset();
    };

    mm->m_getRootset = callable;

    
    
    const char* asmfile = "../testcases/object.asm";




    vm.executeFile(asmfile,0);



    std::cout << "Hello World!\n";
}



