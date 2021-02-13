
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
	typedef struct stGCObject {
		void* type;
		size_t ref_count;
	} GCObject;

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

	GCMemory* GCMemory___construct__(GCMemory* self, FindReferenceObject findReferenceObject, AlignedMemoryOptions* opts);

	inline void GCMemory___destruct__(GCMemory* self, bool_t existed) {
		AlignedMemory___destruct__((AlignedMemory*)self,existed);
	}

	inline void* GCMemory_require(AlignedMemory* self, size_t size, void* type) {
		void* p = AlignedMemory_require(self, size, type);
		((GCObject*)(((byte_t*)p) - sizeof(GCObject)))->type = type;
		return p;
	}

	inline void* GCMemory_require1(AlignedMemory* self, size_t size, void* type) {
		void* p = AlignedMemory_require(self, size, type);
		GCObject* obj =(GCObject*)(((byte_t*)p) - sizeof(GCObject));
		obj->type = type; obj->ref_count = 1;
		return p;
	}

	inline bool_t GCMemory_release(AlignedMemory* self, void* p) {
		((AlignedUnitMeta*)(((byte_t*)p) - sizeof(GCObject)))->ref_count = 0;
		return 1;
	}

	inline bool_t GCMemory_increase(AlignedMemory* self, void* p) {
		((GCObject*)(((byte_t*)p) - sizeof(GCObject)))->ref_count++;
		return 1;
	}
	inline bool_t GCMemory_decrease(AlignedMemory* self, void* p) {
		((GCObject*)(((byte_t*)p) - sizeof(GCObject)))->ref_count--;
		return 1;
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



