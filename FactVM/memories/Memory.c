#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>



Memory* Memory_defaultInstance = NULL;

inline void* Memory_internalRequire(Memory* self, size_t size,void* type) { return malloc(size); }
inline bool_t Memory_internalRelease(Memory* self, void* obj) { free(obj); return 1; }
inline bool_t Memory_internalEmptyFn(void* obj) { return 1; }
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
	self->require = self->require1 = Memory_internalRequire;	
	self->release = Memory_internalRelease;
	self->increase = self->decrease = Memory_internalEmptyFn;
	self->destruct = Memory_internalDestruct;
	return self;
}
