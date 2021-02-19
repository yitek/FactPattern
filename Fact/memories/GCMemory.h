#pragma once
/*****************************************************
*
* author:yiy
*
* description: 对齐的内存管理类
*
******************************************************/

#pragma once
#include "AlignedMemory.h"
#ifndef __GCMEMORY_INCLUDED__ 
#define __GCMEMORY_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef union stGCUnitLayout {
		struct stObjectLayout __GC__;
		struct {
			usize_t ref;
			void* type;
		};
	}GCUnitLayout;
	typedef struct stGCMemory {
		union {
			vftptr_t vfptr;
			struct stAlignedMemory;
		};
		
	}GCMemory;
	typedef struct stGCMemoryVTBL {
		struct stAlignedMemoryVTBL;
	}GCMemoryVTBL;
	extern GCMemoryVTBL gcMemoryVTBL;
	static inline GCMemory* GCMemory__construct__(GCMemory* self, AlignedMemoryOptions* opts, Logger* logger) {
		GCMemory* p = (GCMemory*)AlignedMemory__construct__((AlignedMemory*)self,opts,logger);
		p->vfptr = (vftptr_t)&gcMemoryVTBL;
		return p;
	}
	static inline void* GCMemory_alloc(GCMemory* self, usize_t size) {
		return ((ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size + sizeof(GCUnitLayout))+1);
	}
	static inline void* GCMemory_alloc0(GCMemory* self, usize_t size,void* type) {
		ObjectLayout* p = (ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size + sizeof(GCUnitLayout));
		p->type = type;
		return p + 1;
	}
	static inline void* GCMemory_alloc1(GCMemory* self, usize_t size) {
		ObjectLayout* p = (ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size + sizeof(GCUnitLayout));
		p->ref = 1;
		return p + 1;
	}
	static inline void* GCMemory_alloc01(GCMemory* self, usize_t size,void* type) {
		ObjectLayout* p = (ObjectLayout*)AlignedMemory_alloc((AlignedMemory*)self, size + sizeof(GCUnitLayout));
		p->ref = 1; p->type = type;
		return p + 1;
	}
	static inline bool_t GCMemory_free(GCMemory* self, void* obj) {
		ObjectLayout* p = ((ObjectLayout*)obj -1);
		p->ref = 0;
		return 1;
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __GCMEMORY_INCLUDED__



