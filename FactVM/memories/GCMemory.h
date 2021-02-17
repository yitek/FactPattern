
/*****************************************************
*
* author:yiy
*
* description: 可做垃圾回收的内存管理类
*
******************************************************/

#pragma once
#include "AlignedMemory.h"
#ifndef __GCMEMORY_INCLUDED__ 
#define __GCMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct stGCUnit {
		size_t ref;
		void* type;
	} GCUnit;

	typedef void* (*FindReferenceObject)(void* obj, size_t memberIndex);

	typedef struct stGCMemory {
		struct stAlignedMemory;
		FindReferenceObject findReferenceObject;
	}GCMemory;

	typedef void (*LogCollectGarbageResults)(AlignedMemoryChunk* chunk,size_t count);

	void GCMemory_collectGarbages(GCMemory* self, LogCollectGarbageResults log);

	inline AllocatePageDirectives GCMemory_beforeAllocatePage(struct stAlignedMemoryChunk* chunk) {
		GCMemory_collectGarbages((GCMemory*)chunk->memory,0);
		return AllocatePageDirective_RecheckOrNewPage;
	}

	inline GCUnit* getGCUnit(void* p) { return (GCUnit*)(((byte_t*)p) - sizeof(GCUnit)); }
	inline void* getGCObject(void* p) { return (void*)(((byte_t*)p) + sizeof(GCUnit)); }

	GCMemory* GCMemory___construct__(GCMemory* self, FindReferenceObject findReferenceObject, AlignedMemoryOptions* opts,MemoryTLogger* logger);

	inline void GCMemory___destruct__(GCMemory* self, bool_t existed) {
		AlignedMemory___destruct__((AlignedMemory*)self,existed);
	}

	inline void* GCMemory_require(AlignedMemory* self, size_t size, void* type) {
		byte_t* p = (byte_t*)AlignedMemory_require(self, size, type);
		((GCUnit*)(p - sizeof(GCUnit)))->type = type;
		return p;
	}

	inline void* GCMemory_require1(AlignedMemory* self, size_t size, void* type) {
		void* p = AlignedMemory_require(self, size, type);
		GCUnit* obj =(GCUnit*)(((byte_t*)p) - sizeof(GCUnit));
		obj->type = type; obj->ref = 1;
		return (byte_t*)p + sizeof(GCUnit);
	}

	inline bool_t GCMemory_release(AlignedMemory* self, void* p) {
		((AlignedUnit*)(((byte_t*)p) - sizeof(GCUnit)))->ref = 0;
		return 1;
	}

	inline bool_t GCMemory_increase(AlignedMemory* self, void* p) {
		((GCUnit*)(((byte_t*)p) - sizeof(GCUnit)))->ref++;
		return 1;
	}
	inline bool_t GCMemory_decrease(AlignedMemory* self, void* p) {
		((GCUnit*)(((byte_t*)p) - sizeof(GCUnit)))->ref--;
		return 1;
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



