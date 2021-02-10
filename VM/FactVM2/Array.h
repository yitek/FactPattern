#pragma once
#ifndef __ARRAY_INCLUDED__ 
#define __ARRAY_INCLUDED__
#include "Memory.h"

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct stArray {
		size_t length;
	} Array;


	const Array* Array___construct__(Array* self, const void* buffer, const size_t count, const size_t unitSize, void* mmArgs, Memory* memory);


	inline size_t Array_length(const Array* self) {return self ? self->length : 0;}

	const Array* Array_concat(const Array* left, const Array* right, size_t unitSize,void* mmArgs, Memory* memory);

	const Array* Array_clip(const Array* arr,  const size_t start, const size_t length, const size_t unitSize, void* mmArgs,Memory* memory);

	inline const void* Array___INDEXGETER__(const Array* self, size_t index, size_t unitSize) {
		return (index >= self->length) ? 0 : ((char*)self + sizeof(Array) + index * unitSize);
	}
	inline const void* Array_buffer(const Array* self) { return (char*)self + sizeof(Array); }

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif
