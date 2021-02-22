/*****************************************************
*
* author:yiy
*
* description: 数组基类
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "../runtime.h"
#ifndef __ARRAY_INCLUDED__ 
#define __ARRAY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	
	typedef struct stArray {
		usize_t length;
	} Array;



	const Array* Array__construct__(Array* self, const void* buffer, const usize_t count, usize_t unitSize, TMemory* mm, void* mInitArgs, MemoryKinds mkind);
	void Array__destruct__(Array* self, bool_t existed);

	inline usize_t Array_length(const Array* self) { return self ? self->length : 0; }

	const Array* Array_concat(const Array* left, const Array* right, usize_t unitSize,const Array* empty,  TMemory* mm, void* mInitArgs, MemoryKinds mkind);

	const Array* Array_clip(const Array* arr, const usize_t start, const usize_t length, const usize_t unitSize,const Array* empty, TMemory* mm, void* mInitArgs, MemoryKinds mkind);

	inline void* Array_buffer(const Array* self) { return (char*)self + sizeof(Array); }

	inline void* Array__get__(const Array* self, usize_t index, usize_t unitSize) {
		return  (self &&index < self->length) ?  ((char*)self + sizeof(Array) + index * unitSize):0;
	}
	usize_t Array_index(const Array* self, void* item, usize_t unitSize,usize_t start);
	

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ARRAY_INCLUDED__



