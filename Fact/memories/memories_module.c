#include "memories_module.h"
#include "../loggers/loggers_module.h"
bool_t memories_initialized = 0;

void memories_module(void* p) {
	if (memories_initialized) return;
	memories_initialized = 1;
	loggers_module(p);
	memoryVTBL.offset = 0;
	memoryVTBL.alloc = &TMemory_alloc;
	memoryVTBL.alloc1 = &TMemory_alloc1;
	memoryVTBL.free = &TMemory_free;
	memoryVTBL.__destruct__ = &TMemory__destruct__;
	TMemory_default = (TMemory*)((byte_t*)&TMemory_defaultInstance + sizeof(TGCUnitLayout));
}


