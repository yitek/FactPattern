/*****************************************************
*
* author:yiy
*
* description: 字符串类(带类型)
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "String.h"
#ifndef __TSTRING_INCLUDED__ 
#define __TSTRING_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTString TString;
	const TString* TString__construct__(TString* self, const byte_t* buffer, usize_t byteCount, TMemory* memory,MemoryKinds mkind);

	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TSTRING_INCLUDED__



