/*****************************************************
* 
* author:yiy
* 
* description: 公用定义文件
* 定义一些常用数据类型
* 32位或64位编译能够用相同的源代码
* 
******************************************************/

#pragma once

// 用预处理标签来处理头文件的交叉引用
// 实现多次引用该头文件时，头文件的内容在经过预处理后的送编译器的源代码中只出现一次
// 其结构为 #ifndef __Some_Mark
//          #define __some_Mark
//         #endif
#ifndef __DEF_INCLUDED__ 
#define __DEF_INCLUDED__


typedef char* addr_t;
typedef char byte_t;
typedef unsigned short char_t;
typedef unsigned int bool_t;


#ifdef _WIN64
typedef long word_t;
typedef unsigned long size_t;
typedef long pointer_t;
#else // else _WIN64
typedef int word_t;
typedef unsigned int size_t;
typedef int pointer_t;
#endif // endif _WIN64

#endif // end ifndef __DEF_INCLUDED__
