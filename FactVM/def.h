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

//是否是开发过程
#define __DEVALOPMENT__

typedef char* addr_t;
typedef char byte_t;
typedef char utf8char_t;
typedef unsigned short utf16char_t;
typedef unsigned int utf32char_t;
typedef unsigned short char_t;
typedef unsigned int lchar_t;
typedef unsigned int bool_t;


#ifdef _WIN64
typedef long word_t;
typedef long long dword_t;
typedef long long  lword_t[2];
typedef unsigned long size_t;
typedef unsigned long pointer_t;
#else // else _WIN64
typedef int word_t;
typedef long dword_t;
typedef long long lword_t;
typedef unsigned int size_t;
typedef unsigned int pointer_t;
#endif // endif _WIN64

inline word_t invalidWordValue() { return !((word_t)0); }

typedef union stByte {
	byte_t value;
	byte_t bytes[1];
}Byte;

typedef union stShort {
	short value;
	byte_t bytes[2];
}Short;

typedef union stInt {
	int value;
	byte_t bytes[4];
}Int;

typedef union stLong {
	long value;
	byte_t bytes[8];
}Long;

typedef union stLarge {
	long long value;
	byte_t bytes[16];
}Large;

typedef union stChar {
	char_t value;
	byte_t bytes[2];
}Char;

typedef union stFloat {
	char_t value;
	byte_t bytes[2];
}Float;

#endif // end ifndef __DEF_INCLUDED__
