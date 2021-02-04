#pragma once
#include <stddef.h>
typedef struct TMAllocator {
	void* (*allocate)(size_t size);
	void (*release)(void* p);
	void (*destruct)(struct TMAllocator* allocator);
} MAllocator;

extern MAllocator* MAllocator_defaultInstance;
MAllocator* MAllocator_default();
MAllocator* MAllocator_construct(MAllocator* self);
void MAllocator_destruct(MAllocator* self);

inline void* MAllocator_allocate(MAllocator* self, size_t size) { return self->allocate(size); }
inline void MAllocator_release(MAllocator* self, void* p) { self->release(p); }
