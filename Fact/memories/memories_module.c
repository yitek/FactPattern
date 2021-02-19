#include "memories_module.h"
#include "../loggers/loggers_module.h"
bool_t memories_initialized = 0;

void memories_module(void* p) {
	if (memories_initialized) return;
	memories_initialized = 1;
	loggers_module(p);
	memoryMETA.offset = 0;
	memoryMETA.allocating = 0;
	memoryMETA.alloc = &Memory_alloc;
	memoryMETA.alloc1 = &Memory_alloc1;
	memoryMETA.free = &Memory_free;
	memoryMETA.allocating = 0;
	memoryMETA.__destruct__ = &Memory__destruct__;
	Memory_default = (Memory*)((byte_t*)&Memory_defaultInstance + sizeof(ObjectLayout));

	
	alignedMemoryMETA.offset = 0;
	alignedMemoryMETA.alloc =(void* (*)(Memory*, usize_t)) &AlignedMemory_alloc;
	alignedMemoryMETA.alloc1 = (void* (*)(Memory*, usize_t))&AlignedMemory_alloc1;
	alignedMemoryMETA.free = (bool_t(*)(Memory *, void*))&AlignedMemory_free;
	alignedMemoryMETA.collectGarbages = &AlignedMemory_collectGarbages;
	alignedMemoryMETA.initPageUnits = AlignedMemory__initPageUnits;
	alignedMemoryMETA.allocating =(MemoryAllocatingDirectives (*)(Memory *, usize_t size, void * )) AlignedMemory__allocating;
	alignedMemoryMETA.__destruct__ = (void(*)(Memory*, bool_t))&AlignedMemory__destruct__;

	gcMemoryMETA.offset = 0;
	gcMemoryMETA.alloc = (void* (*)(Memory*, usize_t)) & GCMemory_alloc;
	gcMemoryMETA.alloc1 = (void* (*)(Memory*, usize_t)) & GCMemory_alloc1;
	gcMemoryMETA.free = (bool_t(*)(Memory*, void*)) & GCMemory_free;
	gcMemoryMETA.collectGarbages = (AlignedMemoryReleaseInfo(*)(AlignedMemory *, bool_t, AlignedMemoryGCCallback))&GCMemory_collectGarbages;
	gcMemoryMETA.initPageUnits = GCMemory__initPageUnits;
	gcMemoryMETA.allocating = (MemoryAllocatingDirectives(*)(Memory*, usize_t size, void*))GCMemory__allocating;
	gcMemoryMETA.__destruct__ = (void(*)(Memory*, bool_t)) & GCMemory__destruct__;
	
}


