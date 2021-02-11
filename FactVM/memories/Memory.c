#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>



Memory* Memory_defaultInstance = NULL;

inline void* Memory_internalRequire(Memory* self, size_t size,void* type) { return malloc(size); }
inline bool_t Memory_internalRelease(Memory* self, void* obj) { free(obj); return 1; }
inline bool_t Memory_mockWeakRelease(Memory* self, void* obj) { printf_s("mock weak release:%p",obj); return 1; }
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

#ifndef __DEVALOPMENT__
	self->release = self->weekRelease = Memory_internalRelease;
#endif // !__DEVALOPMENT__
#ifdef __DEVALOPMENT__
	self->release =  Memory_internalRelease;
	self->weekRelease = 
#endif // __DEVALOPMENT__


	
	self->destruct = Memory_internalDestruct;
	return self;
}

bool_t Memory_copy(void* dest, const void* src, size_t size) {
	
	if (dest && src && size) {
		if (size == sizeof(word_t)) {
			*((word_t*)dest) = *(word_t*)src;
			return 1;
		}
		size_t wordc = size / sizeof(word_t);
		size_t bytec = size % sizeof(word_t);
		
		if (wordc)for (size_t i = 0; i < wordc; i++) {
			*((word_t*)dest) = *((word_t*)src);
			((word_t*)dest)++; ((word_t*)src)++;
		}
		if (bytec) for (size_t i = 0; i < bytec; i++) {
			*((byte_t*)dest) = *((byte_t*)src);
			((byte_t*)dest)++; ((byte_t*)src)++;
		}
		return 1;
	}
	return 0;

}