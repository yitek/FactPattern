#include "memories_module.h"
#include "../loggers/loggers_module.h"
bool_t memories_initialized = 0;

void memories_module(void* memory, void* vm, void* param) {
	if (memories_initialized) return;
	memories_initialized = 1;
	loggers_module(memory,vm,param);
	TMemory__meta__.offset = 0;
	TMemory__meta__.allocating = 0;
	TMemory__meta__.alloc = &TMemory_alloc;
	TMemory__meta__.free = &TMemory_free;
	TMemory__meta__.allocating = 0;
	TMemory__meta__.__destruct__ = &TMemory__destruct__;
	TMemory_default = (TMemory*)((byte_t*)&TMemory_defaultInstance + sizeof(ObjectLayout));

	
	TAlignedMemory_Meta.offset = 0;
	TAlignedMemory_Meta.alloc =(void* (*)(TMemory*, usize_t,uword_t)) &TAlignedMemory_alloc;
	TAlignedMemory_Meta.free = (bool_t(*)(TMemory *, void*))&TAlignedMemory_free;
	TAlignedMemory_Meta.collectGarbages = &TAlignedMemory_collectGarbages;
	TAlignedMemory_Meta.allocating =(MemoryAllocatingDirectives (*)(TMemory *, usize_t, uword_t, void * )) TAlignedMemory__allocating;
	TAlignedMemory_Meta.__destruct__ = (void(*)(TMemory*, bool_t))&TAlignedMemory__destruct__;

	gcMemoryMETA.offset = 0;
	gcMemoryMETA.alloc = (void* (*)(TMemory*, usize_t,uword_t)) & GCMemory_alloc;
	gcMemoryMETA.free = (bool_t(*)(TMemory*, void*)) & GCMemory_free;
	gcMemoryMETA.collectGarbages = (AlignedMemoryReleaseInfo(*)(TAlignedMemory *, bool_t, AlignedMemoryGCCallback))&GCMemory_collectGarbages;
	gcMemoryMETA.allocating = (MemoryAllocatingDirectives(*)(TMemory*, usize_t, uword_t, void*))GCMemory__allocating;
	gcMemoryMETA.__destruct__ = (void(*)(TMemory*, bool_t)) & GCMemory__destruct__;
	
}


