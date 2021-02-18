#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "TMemory.h"
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
	}TAlignedMemoryPage;

	typedef struct stAlignedMemoryChunk {
		TAlignedMemoryPage* page;
		struct stAlignedMemory* memory;
		usize_t unitSize;
		usize_t pageCapacity;
		usize_t pageSize;
		struct stAlignedMemoryChunk* nextChunk;
	}TAlignedMemoryChunk;

	typedef struct stAlignedMemoryReleaseInfo {
		usize_t unitSize;
		usize_t chunkCount;
		usize_t pageSize;
		usize_t pages;
		usize_t bytes;
		usize_t units;
	}AlignedMemoryReleaseInfo;

	typedef void (*AlignedMemoryGCCallback)(AlignedMemoryReleaseInfo info);
	typedef void* (*AlignedMemoryInitPage)(TAlignedMemoryChunk* chunk, TAlignedMemoryPage* page, usize_t size);
	typedef void* (*AlignedMemoryLookupUnit)(TAlignedMemoryChunk* chunk,usize_t size);

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
		TAlignedMemoryChunk* large;
		TAlignedMemoryChunk* chunks[32];

	}TAlignedMemory;

	typedef struct stAlignedMemoryVTBL {
		struct stMemoryVTBL;
		
	} TAlignedMemoryVTBL;

	extern TAlignedMemoryVTBL alignedMemoryVTBL;

	TAlignedMemory* TAlignedMemory__construct__(TAlignedMemory* self, AlignedMemoryOptions* opts,TLogger* logger);
	void TAlignedMemory__destruct__(TAlignedMemory* self, bool_t existed);
	void* TAlignedMemory_alloc(TAlignedMemory* self, usize_t size);
	static inline void* TAlignedMemory_alloc1(TAlignedMemory* self, usize_t size) {return TAlignedMemory_alloc(self,size);}

	bool_t TAlignedMemory_free(TAlignedMemory* self, void* p);
	AlignedMemoryReleaseInfo TAlignedMemory_collectGarbages(TAlignedMemory* self, AlignedMemoryGCCallback callback);
	

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TALIGNEDMEMORY_INCLUDED__



