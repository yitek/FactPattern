#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "../def.h"

#ifndef __GCMEMORY_INCLUDED__ 

#define AlignedMemoryMetaSize sizeof(ObjectLayout)
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
	typedef struct stGCUnitLayout {
		struct stObjectLayout;
	}GCUnitLayout;

	typedef void* (*FindReferenceObject)(void* obj, size_t memberIndex);

	typedef struct stGCMemory {
		struct stAlignedMemory;
		
	}GCMemory;
	typedef struct stGCMemoryVTBL {
		struct stAlignedMemoryVTBL;
		FindReferenceObject findReferenceObject;
	}GCMemoryVTBL;
	extern GCMemoryVTBL gcMemoryVTBL;

	static inline GCMemory* GCMemory__construct__(GCMemory* self, AlignedMemoryOptions* opts, Logger* logger) {
		GCMemory* p = (GCMemory*)AlignedMemory__construct__((AlignedMemory*)self,opts,logger);
		((VirtStructLayout*)p)->vftptr = (vftptr_t)&gcMemoryVTBL;
		return p;
	}
	static inline void* GCMemory_alloc(GCMemory* self, usize_t size) {
		return AlignedMemory_alloc((AlignedMemory*)self, size);
	}
	static inline void* GCMemory_alloc0(GCMemory* self, usize_t size,void* type) {
		void* p = AlignedMemory_alloc((AlignedMemory*)self, size);
		(*((ObjectLayout*)p-1)).type = type;
		return p;
	}
	static inline void* GCMemory_alloc1(GCMemory* self, usize_t size) {
		ObjectLayout* p = (ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size);
		(*((ObjectLayout*)p - 1)).ref = 1;
		return p ;
	}
	static inline void* GCMemory_alloc01(GCMemory* self, usize_t size,void* type) {
		ObjectLayout* p = (ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size);
		(--p)->ref = 1; p->type = type;
		return ++p;
	}
	static inline bool_t GCMemory_free(GCMemory* self, void* obj) {
		ObjectLayout* p = ((ObjectLayout*)obj -1);
		p->ref = 0;
		return 1;
	}

	static inline void* AlignedMemory__initPageUnits(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, size_t unitSize) {
		byte_t* unit = (byte_t*)&page->free;
		for (usize_t i = 0; i < chunk->pageCapacity;i++) {
			((ObjectLayout*)unit)->ref = 0;
			unit = unit + unitSize;
		}
		return (void*)&page->free;
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__


