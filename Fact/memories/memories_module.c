#include "memories_module.h"
#include "../loggers/loggers_module.h"
bool_t memories_initialized = 0;

void memories_module(void* p) {
	if (memories_initialized) return;
	memories_initialized = 1;
	loggers_module(p);
	memoryVTBL.offset = 0;
	memoryVTBL.alloc = &Memory_alloc;
	memoryVTBL.alloc1 = &Memory_alloc1;
	memoryVTBL.free = &Memory_free;
	memoryVTBL.__destruct__ = &Memory__destruct__;
	Memory_default = (Memory*)((byte_t*)&Memory_defaultInstance + sizeof(ObjectLayout));

	
	alignedMemoryVTBL.offset = 0;
	alignedMemoryVTBL.alloc =(void* (*)(Memory*, usize_t)) &AlignedMemory_alloc;
	alignedMemoryVTBL.alloc1 = (void* (*)(Memory*, usize_t))&AlignedMemory_alloc1;
	alignedMemoryVTBL.free = (bool_t(*)(Memory *, void*))&AlignedMemory_free;
	alignedMemoryVTBL.collectGarbages = AlignedMemory_collectGarbages;
	alignedMemoryVTBL.__destruct__ = (void(*)(Memory*, bool_t))&AlignedMemory__destruct__;
}


