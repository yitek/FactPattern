#pragma once
#include <stddef.h>



typedef struct TMemory {
	void* (*require)(struct TMemory* self, size_t size,void* args);
	int (*release)(struct TMemory* self, void* p);
	void (*destruct)(struct TMemory* allocator);
} Memory;


Memory* Memory_construct(Memory* self);
inline void Memory_destruct(Memory* self) { if (self->destruct) self->destruct(self); }

inline void* Memory_require(Memory* self, size_t size, void* args) { return self->require(self, size, args); }
inline int Memory_release(Memory* self, void* p) { return self->release(self, p); }

extern Memory* Memory_defaultInstance;
inline Memory* Memory_default() {
	return Memory_defaultInstance ? Memory_defaultInstance : (Memory_defaultInstance = Memory_construct(Memory_defaultInstance));

}



