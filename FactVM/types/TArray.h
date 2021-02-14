/*****************************************************
*
* author:yiy
*
* description: 数组类

*
******************************************************/

#pragma once
#include "TObject.h"
#include "../ads/Array.h"
#ifndef __TARRAY_INCLUDED__ 
#define __TARRAY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif


	inline const Array* TArray___construct__(Array* self, const void* buffer, const size_t count) {
		return Array___construct__(self, buffer, count, TObject_getIndexType(self,0)->size, 0, 0);
	}


	inline size_t TArray_length(const Array* self) { return self ? self->length : 0; }

	const Array* TArray_concat(const Array* left, const Array* right) {
		if (!left)return right; if (!right)return left;
		Type* type = TObject_getType((void*)left);
		Type* itemType = Type_getGenericType(type,0);
		return Array_concat(left, right, itemType->size, (void*)type, type->memory);
	}

	const Array* TArray_clip(const Array* arr, const size_t start, const size_t length) {
		if (!arr) return 0;
		const Type* type = TObject_getType((void*)(arr));
		Type* itemType = (Type*)Array___INDEXGETER__(type->generics, 0, sizeof(Type*));
		return Array_clip(arr, start, length, itemType->size, (void*)type, type->memory);
	}

	inline void* TArray_buffer(const Array* self) { return self?(char*)self + sizeof(Array):0; }

	inline void* TArray___INDEXGETER__(const Array* self, size_t index, size_t unitSize) {
		return (self && index < self->length) ? ((char*)self + sizeof(Array) + index * unitSize):0;
	}


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ARRAY_INCLUDED__



