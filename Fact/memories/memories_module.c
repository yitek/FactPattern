#include "memories_module.h"
#include "../primes/primes_module.h"
bool_t memories_initialized = 0;

void memories_module(void* memory, void* vm, void* param) {
	if (memories_initialized) return;
	memories_initialized = 1;
	primes_module(memory,vm,param);
	TMemory__meta__.offset = 0;
	TMemory__meta__.allocating = 0;
	TMemory__meta__.alloc = &TMemory_alloc;
	TMemory__meta__.free = &TMemory_free;
	TMemory__meta__.allocating = 0;
	TMemory__meta__.__destruct__ = &TMemory__destruct__;
	TMemory_default = (TMemory*)((byte_t*)&TMemory_defaultInstance + sizeof(MTObjUnit));

	
	TAlignedMemory_Meta.offset = 0;
	TAlignedMemory_Meta.alloc =(void* (*)(TMemory*, usize_t,uword_t)) &TAlignedMemory_alloc;
	TAlignedMemory_Meta.free = (bool_t(*)(TMemory *, void*))&TAlignedMemory_free;
	TAlignedMemory_Meta.collectGarbages = &TAlignedMemory_collectGarbages;
	TAlignedMemory_Meta.allocating =(MemoryAllocatingDirectives (*)(TMemory *, usize_t, uword_t, void * )) TAlignedMemory__allocating;
	TAlignedMemory_Meta.__destruct__ = (void(*)(TMemory*, bool_t))&TAlignedMemory__destruct__;

	TGCMemory__meta__.offset = 0;
	TGCMemory__meta__.alloc = (void* (*)(TMemory*, usize_t,uword_t)) & TGCMemory_alloc;
	TGCMemory__meta__.free = (bool_t(*)(TMemory*, void*)) & TGCMemory_free;
	TGCMemory__meta__.collectGarbages = (AlignedMemoryReleaseInfo(*)(TAlignedMemory *, bool_t, AlignedMemoryGCCallback))&TGCMemory_collectGarbages;
	TGCMemory__meta__.allocating = (MemoryAllocatingDirectives(*)(TMemory*, usize_t, uword_t, void*))TGCMemory__allocating;
	TGCMemory__meta__.__destruct__ = (void(*)(TMemory*, bool_t)) & TGCMemory__destruct__;
	
}


