/*****************************************************
*
* author:yiy
*
* description: 数组类(带类型)
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "Array.h"
#ifndef __TARRAY_INCLUDED__ 
#define __TARRAY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	
	typedef struct stTArray TArray;

	extern const struct stTObjectMeta TArray__metaInstance;

	extern const struct stTType* const TArray__type__;

	const TArray* TArray__construct__(TArray* self, const void* buffer, const usize_t count, TMemory* mm, struct stTType* type, MemoryKinds mkind);

	void TArray__destruct__(TArray* self, bool_t existed, TMemory* mm);

	inline static usize_t TArray_length(const TArray* self) { return self ? ((struct stTArray*)self)->length : 0; }

	const TArray* TArray_concat(const TArray* left, const TArray* right, TMemory* mm, MemoryKinds mkind);

	const TArray* TArray_clip(const TArray* arr, const usize_t start, const usize_t length, TMemory* mm, MemoryKinds mkind);

	inline static void* TArray_buffer(const TArray* self) { return (char*)self + sizeof(TArray); }

	inline static void* TArray__get__(const TArray* self, usize_t index) {
		return  (self &&index < ((struct stTArray*)self)->length) ?  (char*)self + sizeof(TArray) + index * get_genericArgument((struct stTObject*)self,0)->size :0;
	}
	usize_t TArray_index(const TArray* self, void* item, usize_t unitSize,usize_t start);
	

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TARRAY_INCLUDED__



