#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "../layout.h"

#ifndef __GCMEMORY_INCLUDED__ 

#define AlignedMemoryLookupUnit(unit, chunk, unitSize) \
	AlignedMemoryPage* lookupPage = chunk->page;\
	while (lookupPage) {\
		ObjectLayout* p_unit = (ObjectLayout*)&lookupPage->free;\
		usize_t pageCapacity= chunk->pageCapacity; \
		for(usize_t freeUnitIndex=0;freeUnitIndex<pageCapacity ;freeUnitIndex++){\
			if (p_unit->ref==0) {unit = p_unit;break;}\
		} \
		if (!unit) lookupPage = lookupPage->next;\
	}\

#endif

#include "AlignedMemory.h"
#ifndef __GCMEMORY_INCLUDED__ 
#define __GCMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stGCMemoryOpts {
		usize_t sweepBytes;
	}GCMemoryOpts;

	typedef struct stGCMemoryOptions {
		struct stAlignedMemoryOptions;
		struct stGCMemoryOpts;
	} GCMemoryOptions;
	typedef struct stGCMemory {
		struct stAlignedMemory;
		struct stGCMemoryOpts;
		
	}GCMemory;
	typedef struct stGCMemoryMETA {
		struct stAlignedMemoryMETA;
	}GCMemoryMETA;
	extern GCMemoryMETA gcMemoryMETA;

	GCMemory* GCMemory__construct__(GCMemory* self, GCMemoryOptions* opts, Logger* logger);
	static inline void GCMemory__destruct__(GCMemory* self, bool_t existed) {
		AlignedMemory__destruct__((AlignedMemory*)self,existed);
	}

	static inline void* GCMemory_alloc(GCMemory* self, usize_t size) {
		return AlignedMemory_alloc((AlignedMemory*)self, size);
	}
	static inline void* GCMemory_alloc0(GCMemory* self, usize_t size,void* type) {
		void* p = AlignedMemory_alloc((AlignedMemory*)self, size);
		//(*((GCUnitLayout*)p-1)).type = type;
		return p;
	}
	static inline void* GCMemory_alloc1(GCMemory* self, usize_t size) {
		GCUnitLayout* p = (GCUnitLayout*)AlignedMemory_alloc((AlignedMemory*)self, size);
		(*((GCUnitLayout*)p - 1)).ref = 1;
		return p ;
	}
	static inline void* GCMemory_alloc01(GCMemory* self, usize_t size,void* type) {
		GCUnitLayout* p = (GCUnitLayout*)AlignedMemory_alloc((AlignedMemory*)self, size);
		//(--p)->ref = 1; p->type = type;
		return ++p;
	}
	static inline bool_t GCMemory_free(GCMemory* self, void* obj) {
		ObjectLayout* p = ((ObjectLayout*)obj -1);
		p->ref = 0;
		return 1;
	}

	static inline void* GCMemory__initPageUnits(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, size_t unitSize) {
		byte_t* unit = (byte_t*)&page->free;
		for (usize_t i = 0; i < chunk->pageCapacity;i++) {
			((ObjectLayout*)unit)->ref = 0;
			unit = unit + unitSize;
		}
		return (void*)&page->free;
	}

	AlignedMemoryReleaseInfo GCMemory_collectGarbages(GCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback);
	MemoryAllocatingDirectives GCMemory__allocating(GCMemory* self, usize_t t, AlignedMemoryChunk* param);
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



