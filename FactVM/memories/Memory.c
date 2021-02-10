#include "memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>



Memory* Memory_defaultInstance = NULL;

inline void* Memory_internalRequire(Memory* self, size_t size) { return malloc(size); }
inline bool_t Memory_internalRelease(Memory* self, void* obj) { free(obj); return 1; }
inline void Memory_internalDestruct(Memory* self) {}

Memory* Memory___construct__(Memory* self) {
	if (!self) {
		self = (Memory*)malloc(sizeof(Memory));
		if (!self) {
			printf_s("[ERR00001]:Memory___construct__:无法分配内存");
			exit(1);
			return NULL;
		}
	}
	self->vptr = (struct stMemoryVTBL*)((char*)self + sizeof(struct stMemoryVTBL*));
	self->require = Memory_internalRequire;
	self->release = self->weekRelease = Memory_internalRelease;
	self->destruct = Memory_internalDestruct;
	return self;
}