#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "Memory.h"
#ifndef __ALIGNEDMEMORY_INCLUDED__ 
#define __ALIGNEDMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stAlignedUnitMeta {
		size_t ref_count;
	} AlignedUnitMeta;

	struct stAlignedMemory;
	struct stAlignedMemoryChunk;
	struct stAlignedMemoryPage;



	typedef struct stAlignedMemoryPage {
		struct stAlignedMemoryPage* next;
	}AlignedMemoryPage;
	typedef struct stAlignedMemoryChunk {
		AlignedMemoryPage* page;
		struct stAlignedMemory* memory;
		size_t unitSize;
		size_t pageCapacity;
		size_t pageSize;
		struct stAlignedMemoryChunk* nextChunk;
	}AlignedMemoryChunk;

	typedef struct stAlignedMemoryOptions {
		size_t pageSize;
		size_t pageCount;
		size_t unitMetaSize;
	}AlignedMemoryOptions;

	typedef struct stAlignedMemory {
		struct stMemory;
		struct stAlignedMemoryOptions;
		AlignedMemoryChunk* large;
		AlignedMemoryChunk* chunks[32];

	}AlignedMemory;

	AlignedMemory* AlignedMemory___construct__(AlignedMemory* self, const AlignedMemoryOptions* opts);

	void AlignedMemory___destruct__(AlignedMemory* self, bool_t existed);

	void* AlignedMemory_require(AlignedMemory* self, size_t size, void* type);

	inline void* AlignedMemory_require1(AlignedMemory* self, size_t size, void* type) {
		void* p = AlignedMemory_require(self,size,type);
		((AlignedUnitMeta*)(((byte_t*)p) - self->unitMetaSize))->ref_count = 1;
		return p;
	}
	
	inline bool_t AlignedMemory_release(AlignedMemory* self, void* p) {
		((AlignedUnitMeta*)(((byte_t*)p) - self->unitMetaSize))->ref_count = 0;
		return 1;
	}
	 
	inline bool_t AlignedMemory_increase(AlignedMemory* self, void* p) {
		((AlignedUnitMeta*)(((byte_t*)p) - self->unitMetaSize))->ref_count++;
		return 1;
	}
	inline bool_t AlignedMemory_decrease(AlignedMemory* self, void* p) {
		((AlignedUnitMeta*)(((byte_t*)p) - self->unitMetaSize))->ref_count--;
		return 1;
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ALIGNEDMEMORY_INCLUDED__



