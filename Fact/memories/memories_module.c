#include "memories_module.h"
#include "../loggers/loggers_module.h"
bool_t memories_initialized = 0;

void memories_module(void* memory, void* vm, void* param) {
	if (memories_initialized) return;
	memories_initialized = 1;
	loggers_module(memory,vm,param);
	memoryMETA.offset = 0;
	memoryMETA.allocating = 0;
	memoryMETA.alloc = &TMemory_alloc;
	memoryMETA.free = &TMemory_free;
	memoryMETA.allocating = 0;
	memoryMETA.__destruct__ = &TMemory__destruct__;
	TMemory_default = (TMemory*)((byte_t*)&TMemory_defaultInstance + sizeof(ObjectLayout));

	
	alignedMemoryMETA.offset = 0;
	alignedMemoryMETA.alloc =(void* (*)(TMemory*, usize_t,uword_t)) &AlignedMemory_alloc;
	alignedMemoryMETA.free = (bool_t(*)(TMemory *, void*))&AlignedMemory_free;
	alignedMemoryMETA.collectGarbages = &AlignedMemory_collectGarbages;
	alignedMemoryMETA.allocating =(MemoryAllocatingDirectives (*)(TMemory *, usize_t, uword_t, void * )) AlignedMemory__allocating;
	alignedMemoryMETA.__destruct__ = (void(*)(TMemory*, bool_t))&AlignedMemory__destruct__;

	gcMemoryMETA.offset = 0;
	gcMemoryMETA.alloc = (void* (*)(TMemory*, usize_t,uword_t)) & GCMemory_alloc;
	gcMemoryMETA.free = (bool_t(*)(TMemory*, void*)) & GCMemory_free;
	gcMemoryMETA.collectGarbages = (AlignedMemoryReleaseInfo(*)(AlignedMemory *, bool_t, AlignedMemoryGCCallback))&GCMemory_collectGarbages;
	gcMemoryMETA.allocating = (MemoryAllocatingDirectives(*)(TMemory*, usize_t, uword_t, void*))GCMemory__allocating;
	gcMemoryMETA.__destruct__ = (void(*)(TMemory*, bool_t)) & GCMemory__destruct__;
	
}


