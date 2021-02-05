#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "MAllocator.h"

MAllocator* MAllocator_defaultInstance = 0;

inline void* MAllocator_internalRequire(MAllocator* self, size_t size) { return malloc(size); }
inline int MAllocator_internalRelease(MAllocator* self, void* p) { if (p) { free(p); return 1; } return 0; }

MAllocator* MAllocator_construct(MAllocator* self) {

	if (!self) {
		if (MAllocator_defaultInstance) return self = MAllocator_defaultInstance;
		MAllocator_defaultInstance = self = (MAllocator*)malloc(sizeof(MAllocator));
	}
	if (!self) {
		printf_s("MAllocator_construct:无法分配内存");
		exit(1);
		return 0;
	}
	self->require = MAllocator_internalRequire;
	self->release = MAllocator_internalRelease;
	return self;
}





