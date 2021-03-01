// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include "tests/TestRuntime.h"
#include "tests/memories/TestMemory.h"
#include "tests/primes/TestArray.h" 
#include "tests/primes/TestLink.h" 
#include "tests/primes/TestList.h"
#include "tests/primes/TestEncoding.h"
#include "tests/primes/TestString.h"
//#include "tests/TestLink.h"

using namespace std;
void printSysInfo() {
#if defined(__WIN__)

    // change code page，use utf-8
    system("chcp 65001");
    printf_s("system: WINDOWS\n");
#endif
#if defined(__LINUX__)
    printf_s("system: LINUX\n");
#endif

#if defined(__16BITS__)
    printf_s("compile bits: 16\n");
#elif defined(__64BITS__)
    printf_s("compile bits: 64\n");
#else
    printf_s("compile bits: 32\n");
#endif
    printf_s("sizeof(char): %d\n",sizeof(char));
    printf_s("sizeof(short): %d\n", sizeof(short));
    printf_s("sizeof(int): %d\n", sizeof(int));
    printf_s("sizeof(long): %d\n", sizeof(long));
    printf_s("sizeof(long long): %d\n", sizeof(long long));
    printf_s("sizeof(void*): %d\n", sizeof(void*));
    printf_s("sizeof(wchar_t): %d\n", sizeof(void*));
}

class ClassA {
    
public:
    int classA_value;
    ClassA(int value) {
        this->classA_value = value;
    }
    void aMethod() { printf_s("ClassA.aMethod() is invoked.\n"); }
    virtual void get_type() { cout << "ClassA.get_type(virtual) is invoked, self="<< this << endl; }
};
void testCppClassLayout() {
    ClassA a(22);
    typedef union {
        void(*Fun)(ClassA*);
        void(ClassA::* CFun)();
    }Fn;

    

    Fn pFun;
    cout << "a:" << (int*)(&a) << endl;
    cout << "a.__vftptr__ value:" << (int*)(&a) << endl;

    cout << "a.__vftptr__[0]:" << (int*)*(int*)(&a) << endl;
    //cout << "ClassA::get_type addr:" << (Fun)&ClassA::get_type << endl;
    printf_s(" ClassA::get_type addr:%p\n",(&ClassA::get_type));
    pFun.Fun = (void(*)(ClassA*)) * ((int*)*(int*)(&a));
    a.get_type();
    ClassA* pA = &a;
    __asm 
    {
        mov ecx,pA;
    }
    pFun.Fun(&a);
    //pFun.CFun = (&ClassA::get_type);
    //pFun.Fun();
    //printf_s("*vftptr[%p]\n", (void*)*(long**)vftptr);
    //printf_s("objA._ClassA_value[%p]\n",&objA->classA_value);
    
}
int main()
{
    testCppClassLayout();
    printSysInfo();
    testRuntime();
    testMemory();
    testAlignedMemory();
    testArray();
    testLink();
    testList();
    testEncoding();
    testString();
    //testLink();
    //
    std::cout << "程序员xxx.press any key to continue...\n";
    char key = std::getchar();
}
