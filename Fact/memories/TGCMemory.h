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

#include "TAlignedMemory.h"
#ifndef __TGCMEMORY_INCLUDED__ 
#define __TGCMEMORY_INCLUDED__
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
		
	}TGCMemory;
	typedef struct stGCMemoryMeta {
		struct stAlignedMemoryMeta;
	}TGCMemoryMeta;

	extern TGCMemoryMeta TGCMemory__meta__;


	TGCMemory* TGCMemory__construct__(TGCMemory* self, GCMemoryOptions* opts, TLogger* logger);
	static inline void TGCMemory__destruct__(TGCMemory* self, bool_t existed) {
		TAlignedMemory__destruct__((TAlignedMemory*)self,existed);
	}

	static inline void* TGCMemory_alloc(TGCMemory* self, usize_t size,uword_t masks) {
		void* p = TAlignedMemory_allocRef((TAlignedMemory*)self, size + sizeof(ObjectLayout),masks);
		//(*((GCUnitLayout*)p-1)).type = type;
		return p;
	}
	
	static inline bool_t TGCMemory_free(TGCMemory* self, void* obj) {
		ObjectLayout* p = ((ObjectLayout*)obj -1);
		p->ref = 0;
		return 1;
	}

	

	AlignedMemoryReleaseInfo TGCMemory_collectGarbages(TGCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback);
	MemoryAllocatingDirectives TGCMemory__allocating(TGCMemory* self, usize_t size,uword_t masks, AlignedMemoryChunk* param);
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



