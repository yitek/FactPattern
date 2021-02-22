// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#include "tests/loggers/TestLogger.h"
#include "tests/memories/TestMemory.h"
#include "tests/primes/TestArray.h" 
#include "tests/primes/TestLink.h" 
//#include "tests/TestString.h"
//#include "tests/TestLink.h"
//#include "tests/TestList.h"
 
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
}
int main()
{
    printSysInfo();
    //CTLogger* loggeer = new CTLogger();
    
    //ObjectLayout objInst;
    //objInst = 0; objInst.type = 0;
   // void* obj = &objInst+1;
    //Object_param(obj);
    //void* obj2 = 0;



    
    //setlocale(LC_ALL, ""); 
    //setlocale(LC_ALL, "chs");
    testTLogger();
    testMemory();
    testAlignedMemory();
    testArray();
    testLink();
    //testString();
    //testLink();
    //testList();
    std::cout << "程序员xxx.press any key to continue...\n";
    char key = std::getchar();
}
