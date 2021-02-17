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
#include "../memories/Memory.h"
#ifndef __ARRAY_INCLUDED__ 
#define __ARRAY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	
	typedef struct stArray {
		size_t length;
	} Array;

	extern const size_t Array_EndlMask;


	const Array* Array_construct(Array* self, const void* buffer, const size_t count, size_t unitSize, void* mmArgs, Memory* memory);
	void Array_destruct(Array* self, bool_t existed);

	inline size_t Array_length(const Array* self) { return self ? self->length : 0; }

	const Array* Array_concat(const Array* left, const Array* right, size_t unitSize,const Array* empty, void* mmArgs, Memory* memory);

	const Array* Array_clip(const Array* arr, const size_t start, const size_t length, const size_t unitSize,const Array* empty, void* mmArgs, Memory* memory);

	inline void* Array_buffer(const Array* self) { return (char*)self + sizeof(Array); }

	inline void* Array_get(const Array* self, size_t index, size_t unitSize) {
		return  (self &&index < self->length) ?  ((char*)self + sizeof(Array) + index * unitSize):0;
	}
	size_t Array_index(const Array* self, void* item, size_t unitSize,size_t start);
	

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ARRAY_INCLUDED__



