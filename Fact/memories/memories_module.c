#include "memories_module.h"
#include "../primes/primes_module.h"
bool_t memories_initialized = 0;

void memories_module(void* memory, void* vm, void* param) {
	if (memories_initialized) return;
	memories_initialized = 1;
	primes_module(memory,vm,param);
	

	//
	//TAlignedMemory__meta__.offset = 0;
	//TAlignedMemory__meta__.alloc =(void* (*)(TMemory*, usize_t,void*,MemoryKinds)) &TAlignedMemory_alloc;
	//TAlignedMemory__meta__.free = (bool_t(*)(TMemory *, void*))&TAlignedMemory_free;
	//TAlignedMemory__meta__.collectGarbages = &TAlignedMemory_collectGarbages;
	//TAlignedMemory__meta__.allocating = TAlignedMemory__allocating;
	//TAlignedMemory__meta__.__destruct__ = (void(*)(TMemory*, bool_t))&TAlignedMemory__destruct__;

	//TGCMemory__meta__.offset = 0;
	//TGCMemory__meta__.alloc = (void* (*)(TMemory*, usize_t,void*,MemoryKinds)) & TGCMemory_alloc;
	//TGCMemory__meta__.free = (bool_t(*)(TMemory*, void*)) & TGCMemory_free;
	//TGCMemory__meta__.collectGarbages = (AlignedMemoryReleaseInfo(*)(TAlignedMemory *, bool_t, AlignedMemoryGCCallback))&TGCMemory_collectGarbages;
	//TGCMemory__meta__.allocating = (MemoryAllocatingDirectives(*)(TAlignedMemory*, usize_t, MemoryKinds, AlignedMemoryChunk*))TGCMemory__allocating;
	//TGCMemory__meta__.__destruct__ = (void(*)(TMemory*, bool_t)) & TGCMemory__destruct__;
	//
}


