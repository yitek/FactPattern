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
#include <malloc.h>
#ifndef __ALIGNEDMEMORY_INCLUDED__ 
#define __ALIGNEDMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	
	struct stAlignedMemory;
	struct stAlignedMemoryChunk;
	struct stAlignedMemoryPage;

	

	//typedef MemoryAllocatingDirectives (*AlignedMemoryAllocating)(struct stAlignedMemoryChunk* chunk);

	

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
	

	struct stAlignedMemoryOpts {
		usize_t pageSize;
		usize_t gcBytes;
	};
	typedef struct stAlignedMemoryOptions {
		struct stMemoryOptions;
		struct stAlignedMemoryOpts;
	}AlignedMemoryOptions;

	struct stAlignedMemoryHeader {
		struct stMemory;
		struct stAlignedMemoryOpts opts;
	};

	typedef struct stAlignedMemory {
		struct stMemory;
		struct stAlignedMemoryOpts;
		usize_t allocatedBytes;
		AlignedMemoryChunk* large;
		AlignedMemoryChunk* chunks[32+1];

	}AlignedMemory;

	typedef struct stAlignedMemoryMETA {
		struct stMemoryMETA;
		
		void* (*initPageUnits)(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, usize_t size);
		AlignedMemoryReleaseInfo(*collectGarbages)(AlignedMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback);
	} AlignedMemoryMETA;

	extern AlignedMemoryMETA alignedMemoryMETA;

	

	

	AlignedMemory* AlignedMemory__construct__(AlignedMemory* self, AlignedMemoryOptions* opts,Logger* logger);
	void AlignedMemory__destruct__(AlignedMemory* self, bool_t existed);
	
	bool_t AlignedMemory_free(AlignedMemory* self, void* p);
	AlignedMemoryReleaseInfo AlignedMemory_collectGarbages(AlignedMemory* self, bool_t releasePage,AlignedMemoryGCCallback callback);
	
#define AlignedMemory_sfree(self, p) (AlignedMemory_free(self,p)?p=0,1:0);

	MemoryAllocatingDirectives AlignedMemory__allocating(AlignedMemory* memory, usize_t size, AlignedMemoryChunk* chunk);
	AlignedMemoryChunk* AlignedMemory__getLargeChunk(AlignedMemory* self, size_t unitSize);
	void* AlignedMemory__chunkResolveUnit(AlignedMemoryChunk* chunk, size_t unitSize);
	void* AlignedMemory__initPageUnits(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, size_t unitSize);

#ifndef AlignedMemoryLookupUnit
#define AlignedMemoryLookupUnit(unit, chunk, unitSize) \
	AlignedMemoryPage* lookupPage = chunk->page;\
	while (lookupPage) {\
		unit = (void*)(lookupPage->free);\
		if (unit) {lookupPage->free = ((AlignedMemoryFreeUnit*)unit)->next;break;}\
		lookupPage = lookupPage->next;\
	}\

#endif

	static inline void* AlignedMemory_alloc(AlignedMemory* self, usize_t unitSize) {
#if defined(AlignedMemoryMetaSize)
		unitSize += AlignedMemoryMetaSize;
#endif
		size_t chunkIndex;
		AlignedMemoryChunk* chunk = 0;
		if (unitSize > 16 * sizeof(addr_t) * 4) {
			if (unitSize % (sizeof(addr_t) * 4)) unitSize = (unitSize / sizeof(addr_t) * 4) + 1;
			AlignedMemoryChunk* existed = self->large;
			AlignedMemoryChunk* prev = 0;
			while (existed) {
				if (existed->unitSize == unitSize) return existed;
				else if (existed->unitSize < unitSize) {
					prev = existed;
					existed = existed->nextChunk;
				}
				else break;
			}
			size_t pageSize = ((struct stAlignedMemoryHeader*)self)->opts.pageSize;
			size_t pageUsableSize = (pageSize - sizeof(AlignedMemoryPage));
			size_t pageCapacity = pageUsableSize / unitSize;
			// 一页都无法装下一个
			if (pageCapacity == 0) {
				pageSize = unitSize + sizeof(AlignedMemoryPage);
				pageCapacity = 1;
			}
			// 一页只能装一个
			else if (pageCapacity < 2) {
				//剩余的太多
				if (pageUsableSize % unitSize > pageUsableSize / 4) {
					pageSize = unitSize + sizeof(AlignedMemoryPage);
					pageCapacity = 1;
				}
			}
			AlignedMemoryChunk* chunk = (AlignedMemoryChunk*)malloc(sizeof(AlignedMemoryChunk));
			if (!chunk) {
				log_exit(1, "AlignedMemory._getLargeChunk", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
				return 0;
			}
			else {
				chunk->memory->allocatedBytes += sizeof(AlignedMemoryChunk);
			}
			chunk->page = 0;
			chunk->memory = self;
			chunk->nextChunk = existed;
			chunk->pageSize = pageSize;
			chunk->unitSize = unitSize;
			chunk->pageCapacity = pageCapacity;
			if (prev) prev->nextChunk = chunk;
			else self->large = chunk;

			if (((Memory*)self)->logger) {
				Logger_trace(((Memory*)self)->logger, "AlignedMemory._getLargeChunk", "<AlignedMemoryChunk>[%p] for LARGE is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld }"
					, chunk, (unsigned long)chunk->unitSize, (unsigned long)chunk->pageSize, (unsigned long)chunk->pageCapacity, chunk->memory->allocatedBytes
				);
			}
		}
		else {
			if (unitSize <= 16 * sizeof(addr_t)) {// 16 个 1word 增加的 最大的是 16word, 0-15
				chunkIndex = unitSize / sizeof(addr_t);
				if (unitSize % sizeof(addr_t)) { unitSize = (++chunkIndex) * sizeof(addr_t); }
			}
			else if (unitSize <= 16 * sizeof(addr_t) * 2) {// 最大的 32word 15 - 31 128 
				chunkIndex = 16 - 8 + unitSize / (sizeof(addr_t) * 2);
				if (unitSize % (sizeof(addr_t) * 2)) {
					unitSize = 16 * sizeof(word_t) + (++chunkIndex - 16 + 8) * sizeof(addr_t) * 2;
				}
			}
			else if (unitSize <= 16 * sizeof(addr_t) * 4) {// 最大 64 word 256bytes 24-28
				chunkIndex = 24 - 8 + unitSize / (sizeof(addr_t) * 4);
				if (unitSize % (sizeof(addr_t) * 4)) { unitSize = 16 * sizeof(dword_t) + (++chunkIndex - 24 + 8) * sizeof(addr_t) * 4; }
			}

			chunk = self->chunks[--chunkIndex];
			if (!chunk) {

				chunk = (AlignedMemoryChunk*)malloc(sizeof(AlignedMemoryChunk));
				if (!chunk) {
					log_exit(1, "AlignedMemory.alloc", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
					return 0;
				}
				else {
					self->allocatedBytes += sizeof(AlignedMemoryChunk);
					self->chunks[chunkIndex] = chunk;
				}
				chunk->page = 0;
				chunk->memory = self;
				chunk->nextChunk = 0;
				chunk->pageSize = ((struct stAlignedMemoryHeader*)self)->opts.pageSize;
				chunk->unitSize = unitSize;
				chunk->pageCapacity = (chunk->pageSize - sizeof(AlignedMemoryPage)) / unitSize;

				if (((Memory*)self)->logger) {
					Logger_trace(((Memory*)self)->logger, "AlignedMemory.alloc", "<AlignedMemoryChunk>[%p] for NORMAL is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld}"
						, chunk, chunk->unitSize, chunk->pageSize, chunk->pageCapacity, chunk->memory->allocatedBytes
					);
				}
			}

		}

		void* unit = 0;
		unitSize = chunk->unitSize;
#if defined(AlignedMemoryLookupUnit)
		AlignedMemoryLookupUnit(unit, chunk, unitSize)
#endif
			if (unit) return unit;
		return AlignedMemory__chunkResolveUnit(chunk, unitSize);
	}

	static inline void* AlignedMemory_alloc1(AlignedMemory* self, usize_t size) { return AlignedMemory_alloc(self, size); }

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ALIGNEDMEMORY_INCLUDED__



