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

	GCMemory* GCMemory__construct__(GCMemory* self, GCMemoryOptions* opts, TLogger* logger);
	static inline void GCMemory__destruct__(GCMemory* self, bool_t existed) {
		AlignedMemory__destruct__((AlignedMemory*)self,existed);
	}

	static inline void* GCMemory_alloc(GCMemory* self, usize_t size,uword_t masks) {
		void* p = AlignedMemory_allocRef((AlignedMemory*)self, size + sizeof(ObjectLayout),masks);
		//(*((GCUnitLayout*)p-1)).type = type;
		return p;
	}
	
	static inline bool_t GCMemory_free(GCMemory* self, void* obj) {
		ObjectLayout* p = ((ObjectLayout*)obj -1);
		p->ref = 0;
		return 1;
	}

	

	AlignedMemoryReleaseInfo GCMemory_collectGarbages(GCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback);
	MemoryAllocatingDirectives GCMemory__allocating(GCMemory* self, usize_t size,uword_t masks, AlignedMemoryChunk* param);
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



