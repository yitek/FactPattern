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
// 排除 utf8警告
// #pragma warning(disable:4819)
// 排除enum class警告
// #pragma warning(disable:26812)
//是否是开发过程
#define __DEVALOPMENT__



#if defined(_WIN64) || __SIZEOF_POINTER__ == 8
#define __64BITS__
#elif defined(_WIN16) || defined(__MSDOS__)
#define __16BITS__
#else 
#define __32BITS__
#endif

#if defined(_WIN32)
#define __WIN__
#endif

#if defined(__linux__)
#define __LINUX__
#endif

#if defined(__64BITS__)
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef unsigned long long addr_t;

typedef long long word_t;
typedef unsigned long long uword_t;
typedef long long dword_t[2];
typedef unsigned long long udword_t[2];
typedef unsigned long long usize_t;

#elif defined(__32BITS__)
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef unsigned int addr_t;

typedef int word_t;
typedef unsigned int uword_t;
typedef long long dword_t;
typedef unsigned long long udword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#else // else16
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef long long large_t[2];
typedef unsigned long long ularge_t[2];
typedef unsigned int addr_t;

typedef short word_t;
typedef unsigned short uword_t;
typedef int dword_t;
typedef unsigned int udword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#endif // endif _WIN64

#if defined(__WIN__)
typedef unsigned short schar_t;
#elif defined(__LINUX__)
typedef unsigned int schar_t;
#endif

typedef char byte_t;
typedef unsigned char ubyte_t;
typedef short short_t;
typedef unsigned short ushort_t;
typedef int int_t;
typedef unsigned int uint_t;
typedef unsigned int unichar_t;
typedef word_t bool_t;




#endif // end ifndef __DEF_INCLUDED__
