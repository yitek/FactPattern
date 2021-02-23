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
//#include "tests/TestString.h"
//#include "tests/TestLink.h"

 
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
int main()
{
    printSysInfo();
    testRuntime();
    testMemory();
    testAlignedMemory();
    testArray();
    testLink();
    testList();
    testEncoding();/**/
    //testString();
    //testLink();
    //
    std::cout << "程序员xxx.press any key to continue...\n";
    char key = std::getchar();
}
