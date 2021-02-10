#pragma once
#include "def.h"
#ifndef __MEMORY_INCLUDED__ 
#define __MEMORY_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif
	struct stMemory;

	
	typedef struct stMemory {
		void* (*require)(struct stMemory* self, size_t size, void* args);
		int (*release)(struct stMemory* self, void* p);
		void (*destruct)(struct stMemory* allocator);
	} Memory;


	extern Memory* Memory_defaultInstance;
	Memory* Memory___construct__(Memory* self);
	inline void Memory___destruct__(Memory* self) { if (self->destruct) self->destruct(self); }

	inline void* Memory_require(Memory* self, size_t size, void* args) { return self->require(self, size, args); }
	inline int Memory_release(Memory* self, void* p) { return self->release(self, p); }

	inline Memory* Memory_default() {
		return Memory_defaultInstance ? Memory_defaultInstance : (Memory_defaultInstance = Memory___construct__(Memory_defaultInstance));

	}
#ifdef __cplusplus 
}//end extern c
#endif
#endif



