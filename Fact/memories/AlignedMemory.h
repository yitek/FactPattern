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
#ifndef __TALIGNEDMEMORY_INCLUDED__ 
#define __TALIGNEDMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	
	struct stAlignedMemory;
	struct stAlignedMemoryChunk;
	struct stAlignedMemoryPage;

	typedef enum {
		AllocatePageDirective_Fail=0,
		AllocatePageDirective_Recheck = -1,
		AllocatePageDirective_NewPage =1,
		AllocatePageDirective_RecheckOrNewPage=2,
	} AllocatePageDirectives;

	typedef AllocatePageDirectives (*AlignedMemoryAllocating)(struct stAlignedMemoryChunk* chunk);

	

	typedef struct stAlignedMemoryFreeUnit {
		struct stAlignedMemoryFreeUnit* next;
	}AlignedMemoryFreeUnit;

	typedef struct stAlignedMemoryPage {
		struct stAlignedMemoryPage* next;
		AlignedMemoryFreeUnit* free;
	}AlignedMemoryPage;

	typedef struct stAlignedMemoryChunk {
		AlignedMemoryPage* page;
		struct stAlignedMemory* memory;
		usize_t unitSize;
		usize_t pageCapacity;
		usize_t pageSize;
		struct stAlignedMemoryChunk* nextChunk;
	}AlignedMemoryChunk;

	typedef struct stAlignedMemoryReleaseInfo {
		usize_t unitSize;
		usize_t chunkCount;
		usize_t pageSize;
		usize_t pages;
		usize_t bytes;
		usize_t units;
	}AlignedMemoryReleaseInfo;

	typedef void (*AlignedMemoryGCCallback)(AlignedMemoryReleaseInfo info);
	typedef void* (*AlignedMemoryInitPage)(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, usize_t size);
	typedef void* (*AlignedMemoryLookupUnit)(AlignedMemoryChunk* chunk,usize_t size);

	struct stAlignedMemoryOpts {
		AlignedMemoryInitPage initPage;
		AlignedMemoryLookupUnit lookupUnit;
		usize_t pageSize;
		usize_t totalBytes;
	};
	typedef struct stAlignedMemoryOptions {
		struct stMemoryOptions;
		struct stAlignedMemoryOpts;
	}AlignedMemoryOptions;

	typedef struct stAlignedMemory {
		struct stMemory;
		struct stAlignedMemoryOpts;
		usize_t allocatedBytes;
		AlignedMemoryChunk* large;
		AlignedMemoryChunk* chunks[32];

	}AlignedMemory;

	typedef struct stAlignedMemoryVTBL {
		struct stMemoryVTBL;
		
	} AlignedMemoryVTBL;

	extern AlignedMemoryVTBL alignedMemoryVTBL;

	AlignedMemory* AlignedMemory__construct__(AlignedMemory* self, AlignedMemoryOptions* opts,Logger* logger);
	void AlignedMemory__destruct__(AlignedMemory* self, bool_t existed);
	void* AlignedMemory_alloc(AlignedMemory* self, usize_t size);
	static inline void* AlignedMemory_alloc1(AlignedMemory* self, usize_t size) {return AlignedMemory_alloc(self,size);}

	bool_t AlignedMemory_free(AlignedMemory* self, void* p);
	AlignedMemoryReleaseInfo AlignedMemory_collectGarbages(AlignedMemory* self, AlignedMemoryGCCallback callback);
	

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TALIGNEDMEMORY_INCLUDED__



