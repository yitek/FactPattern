#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

Memory* Memory_defaultInstance = 0;

inline void* Memory_internalRequire(Memory* self, size_t size, void* args) { return malloc(size); }
inline int Memory_internalRelease(Memory* self, void* p) { if (p) { free(p); return 1; } return 0; }

Memory* Memory___construct__(Memory* self) {

	if (!self) {
		if (Memory_defaultInstance) return self = Memory_defaultInstance;
		Memory_defaultInstance = self = (Memory*)malloc(sizeof(Memory));
	}
	if (!self) {
		printf_s("Memory_construct:无法分配内存");
		exit(1);
		return 0;
	}
	self->require = Memory_internalRequire;
	self->release = Memory_internalRelease;
	return self;
}





