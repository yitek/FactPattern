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
	struct stAlignedMemory;
	struct stAlignedMemoryChunk;
	struct stAlignedMemoryPage;
	typedef void* (*SearchIdleUnit)(void* page,size_t unitSize);
	typedef struct stAlignedMemoryPage {
		size_t usedUnitCount;
		void* idle;

	}AlignedMemoryPage;
	typedef struct stAlignedMemoryChunk {
		void* firstPage;
		void* lastPage;
		struct stAlignedMemory* memory;
		size_t unitSize;
		size_t pageCapacity;
		size_t pageSize;
		struct stAlignedMemoryChunk* nextChunk;
	}AlignedMemoryChunk;

	typedef struct stAlignedMemoryOptions {
		size_t pageSize;
		size_t pageCount;
		size_t pagePaddingSize;
		SearchIdleUnit searchIdleUnit;
	}AlignedMemoryOptions;

	typedef struct stAlignedMemory {
		struct stMemory;
		struct stAlignedMemoryOptions;
		AlignedMemoryChunk* large;
		AlignedMemoryChunk* chunks[32];

	}AlignedMemory;

	


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ALIGNEDMEMORY_INCLUDED__



