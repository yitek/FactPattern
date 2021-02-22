#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "../runtime.h"
#ifndef __ALIGNEDMEMORY_INCLUDED__ 
#define __ALIGNEDMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	
	struct stTAlignedMemory;
	struct stAlignedMemoryChunk;
	struct stAlignedMemoryPage;

	typedef enum {
		MemoryAllocatingDirective_fail = 0,
		MemoryAllocatingDirective_lookup = -1,
		MemoryAllocatingDirective_new = 1,
		MemoryAllocatingDirective_lookupOrNew = 2,
	} MemoryAllocatingDirectives;

	

	



	typedef struct stAlignedMemoryPage {
		struct stAlignedMemoryPage* next;
		MemoryKinds kind;
		MLnkUnit* free;
	}AlignedMemoryPage;

	typedef struct stAlignedMemoryChunk {
		AlignedMemoryPage* page;
		struct stTAlignedMemory* memory;
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
		MemoryUnitKinds unitKind;
		usize_t pageSize;
		usize_t gcBytes;
	};
	typedef struct stAlignedMemoryOptions {
		MemoryUnitKinds unitKind;
		usize_t pageSize;
		usize_t totalBytes;
		usize_t gcBytes;
	}AlignedMemoryOptions;

	struct stAlignedMemoryHeader {
		struct stTMemory;
		struct stAlignedMemoryOptions opts;
	};

	typedef struct stTAlignedMemory {
		struct stTMemory;
		struct stAlignedMemoryOptions;
		TLogger* logger;
		usize_t allocatedBytes;
		AlignedMemoryChunk* large;
		AlignedMemoryChunk* chunks[32+1];

	}TAlignedMemory;

	typedef struct stTAlignedMemoryMeta {
		struct stTMemoryMeta;
		MemoryAllocatingDirectives(*allocating)(TAlignedMemory* memory, usize_t size, MemoryKinds masks, AlignedMemoryChunk* param);
		AlignedMemoryReleaseInfo(*collectGarbages)(TAlignedMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback);
	} TAlignedMemoryMeta;

	extern TAlignedMemoryMeta TAlignedMemory__meta__;

	

	

	TAlignedMemory* TAlignedMemory__construct__(TAlignedMemory* self, AlignedMemoryOptions* opts,TLogger* logger);
	void TAlignedMemory__destruct__(TAlignedMemory* self, bool_t existed);
	
	AlignedMemoryReleaseInfo TAlignedMemory_collectGarbages(TAlignedMemory* self, bool_t releasePage,AlignedMemoryGCCallback callback);
	
	
#define AlignedMemory_sfree(self, p) (TAlignedMemory_free(self,p)?p=0,1:0);

	MemoryAllocatingDirectives TAlignedMemory__allocating(TAlignedMemory* memory, usize_t size,MemoryKinds masks ,AlignedMemoryChunk* chunk);
	void* TAlignedMemory__chunkResolveUnit(AlignedMemoryChunk* chunk, usize_t unitSize,uword_t masks);

	bool_t TAlignedMemory_freeLink(TAlignedMemory* self, void* obj);
	static inline bool_t AlignedMemory_freeRef(TAlignedMemory* self, void* obj) {
		return ((MRefUnit*)obj)->__ref__=0,1;
	}
	static inline bool_t TAlignedMemory_free(TAlignedMemory* self, void* obj) {
		if (((struct stAlignedMemoryHeader*)self)->opts.unitKind == MemoryUnitKind_link) return TAlignedMemory_freeLink(self,obj);
		else if(((struct stAlignedMemoryHeader*)self)->opts.unitKind == MemoryUnitKind_ref) return AlignedMemory_freeRef(self, obj);
		return 0;
	}

#define MemoryLookupLinkUnit(unit, chunk, unitSize,masks) \
	AlignedMemoryPage* lookupPage = chunk->page;\
	while (lookupPage) {\
		if(lookupPage->kind == masks){\
			unit = (void*)(lookupPage->free);\
			if (unit) {lookupPage->free = ((MLnkUnit*)unit)->next;break;}\
		}\
		lookupPage = lookupPage->next;\
	}\


	static inline void* TAlignedMemory_allocLink(TAlignedMemory* self, usize_t unitSize,MemoryKinds masks) {

		usize_t chunkIndex;
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
			usize_t pageSize = ((struct stAlignedMemoryHeader*)self)->opts.pageSize;
			usize_t pageUsableSize = (pageSize - sizeof(AlignedMemoryPage));
			usize_t pageCapacity = pageUsableSize / unitSize;
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
			AlignedMemoryChunk* chunk = m_allocate(AlignedMemoryChunk,0,0, MemoryKind_system);
			if (!chunk) {
				log_exit(ExitCode_memory, "AlignedMemory._getLargeChunk", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
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

			if (self->logger) {
				TLogger_trace(self->logger, "AlignedMemory._getLargeChunk", "<AlignedMemoryChunk>[%p] for LARGE is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld }"
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

				chunk = m_allocate(AlignedMemoryChunk,0,0, MemoryKind_system);
				if (!chunk) {
					log_exit(ExitCode_memory, "AlignedMemory.alloc", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
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

				if (self->logger) {
					TLogger_trace(self->logger, "AlignedMemory.alloc", "<AlignedMemoryChunk>[%p] for NORMAL is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld}"
						, chunk, chunk->unitSize, chunk->pageSize, chunk->pageCapacity, chunk->memory->allocatedBytes
					);
				}
			}

		}

		void* unit = 0;
		unitSize = chunk->unitSize;

		MemoryLookupLinkUnit(unit, chunk, unitSize, masks)
		if (unit) return unit;
		return TAlignedMemory__chunkResolveUnit(chunk, unitSize,masks);
	}

#define MemoryLookupRefUnit(unit, chunk, unitSize,masks) \
	AlignedMemoryPage* lookupPage = chunk->page;\
	while (lookupPage) {\
		if(lookupPage->kind == masks){\
			MRefUnit* p_unit = (MRefUnit*)&lookupPage->free;\
			usize_t pageCapacity= chunk->pageCapacity; \
			for(usize_t freeUnitIndex=0;freeUnitIndex<pageCapacity ;freeUnitIndex++){\
				if (p_unit->__ref__==0) {unit = p_unit;break;}\
				p_unit = (MRefUnit*)((byte_t*)p_unit + unitSize);\
			} \
		}\
		if (unit) break;\
		lookupPage = lookupPage->next;\
	}\

	static inline void* TAlignedMemory_allocRef(TAlignedMemory* self, usize_t unitSize, MemoryKinds masks) {

		usize_t chunkIndex;
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
			usize_t pageSize = ((struct stAlignedMemoryHeader*)self)->opts.pageSize;
			usize_t pageUsableSize = (pageSize - sizeof(AlignedMemoryPage));
			usize_t pageCapacity = pageUsableSize / unitSize;
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
			AlignedMemoryChunk* chunk = m_allocate(AlignedMemoryChunk,0,0,MemoryKind_system);
			if (!chunk) {
				log_exit(ExitCode_memory, "AlignedMemory._getLargeChunk", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
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

			if (self->logger) {
				TLogger_trace(self->logger, "AlignedMemory._getLargeChunk", "<AlignedMemoryChunk>[%p] for LARGE is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld }"
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

				chunk = m_allocate(AlignedMemoryChunk,0,0, MemoryKind_system);
				if (!chunk) {
					log_exit(ExitCode_memory, "AlignedMemory.alloc", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemoryChunk));
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

				if (self->logger) {
					TLogger_trace(self->logger, "AlignedMemory.alloc", "<AlignedMemoryChunk>[%p] for NORMAL is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld}"
						, chunk, chunk->unitSize, chunk->pageSize, chunk->pageCapacity, chunk->memory->allocatedBytes
					);
				}
			}

		}

		void* unit = 0;
		unitSize = chunk->unitSize;

		AlignedMemoryPage* lookupPage = chunk->page;
		while (lookupPage) {
			if (lookupPage->kind == masks) {
				MRefUnit* p_unit = (MRefUnit*)&lookupPage->free;
				usize_t pageCapacity = chunk->pageCapacity;
				for (usize_t freeUnitIndex = 0; freeUnitIndex < pageCapacity; freeUnitIndex++) {
					if (p_unit->__ref__ == 0) { unit = p_unit; break; }
					p_unit = (MRefUnit*)((byte_t*)p_unit + unitSize);
				}
			}
			if (unit) break;
			lookupPage = lookupPage->next;

		}
		if (unit) return unit;
		return TAlignedMemory__chunkResolveUnit(chunk, unitSize, masks);
	}
	static inline void* TAlignedMemory_alloc(TAlignedMemory* self, usize_t unitSize,void* mInitArgs, MemoryKinds masks) {
		if (((struct stAlignedMemoryHeader*)self)->opts.unitKind == MemoryUnitKind_link) return TAlignedMemory_allocLink(self, unitSize, masks);
		else if (((struct stAlignedMemoryHeader*)self)->opts.unitKind == MemoryUnitKind_ref) return TAlignedMemory_allocRef(self,unitSize,masks);
		return 0;
	}
	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ALIGNEDMEMORY_INCLUDED__



