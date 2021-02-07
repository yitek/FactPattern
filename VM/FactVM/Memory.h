#pragma once
#ifndef __MEMORY_INCLUDED__ 
#define __MEMORY_INCLUDED__
#include "def.h"



typedef struct TMemory {
	void* (*require)(struct TMemory* self, size_t size,void* args);
	int (*release)(struct TMemory* self, void* p);
	void (*destruct)(struct TMemory* allocator);
} Memory;

extern Memory* Memory_defaultInstance;
Memory* Memory_construct(Memory* self);
inline void Memory_destruct(Memory* self) { if (self->destruct) self->destruct(self); }

inline void* Memory_require(Memory* self, size_t size, void* args) { return self->require(self, size, args); }
inline int Memory_release(Memory* self, void* p) { return self->release(self, p); }

inline Memory* Memory_default() {
	return Memory_defaultInstance ? Memory_defaultInstance : (Memory_defaultInstance = Memory_construct(Memory_defaultInstance));

}

#endif



