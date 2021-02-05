#pragma once
#include <stddef.h>
#include <malloc.h>
typedef struct TMAllocator {
	void* (*require)(struct TMAllocator* self,size_t size);
	int (*release)(struct TMAllocator* self,void* p);
	void (*destruct)(struct TMAllocator* allocator);
} MAllocator;


MAllocator* MAllocator_construct(MAllocator* self);
inline void MAllocator_destruct(MAllocator* self) {if (self->destruct) self->destruct(self);}

inline void* MAllocator_require(MAllocator* self, size_t size) { return self->require(self,size); }
inline int MAllocator_release(MAllocator* self, void* p) { return self->release(self,p); }

extern MAllocator* MAllocator_defaultInstance;
inline MAllocator* MAllocator_default() {
	return MAllocator_defaultInstance ? MAllocator_defaultInstance : (MAllocator_defaultInstance = MAllocator_construct(MAllocator_defaultInstance));

}
