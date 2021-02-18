// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#include "loggers/CLogger.h"
#include "tests/loggers/TestLogger.h"
//#include "tests/TestMemory.h"
//#include "tests/TestArray.h" 
//#include "tests/TestString.h"
//#include "tests/TestLink.h"
//#include "tests/TestList.h"
 
int main()
{
    CLogger* loggeer = new CLogger();
    //setlocale(LC_ALL, ""); 
    //setlocale(LC_ALL, "chs");
    #if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // change code page，use utf-8
    //system("chcp 65001");
    #endif
    testTLogger();
    //testMemory();
    //testArray();
    //testString();
    //testLink();
    //testList();
    std::cout << "程序员xxx.press any key to continue...\n";
    std::getchar();
}
