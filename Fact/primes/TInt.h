/*****************************************************
*
* author:yiy
*
* description: 整数结构(带类型)
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "../runtime.h"
#ifndef __INT_INCLUDED__ 
#define __INT_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTInt {
		struct stTObject;
		int_t value;
	} TInt;
	extern const struct stTType* const TInt__type__;
	TInt* TInt__construct__(TInt* self,int_t value, TMemory* mm, MemoryKinds mkind);
	inline static struct stTType* TInt__gettype__() { return TInt__type__; }
	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TARRAY_INCLUDED__



