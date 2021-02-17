#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>



Memory* Memory_defaultInstance = 0;

void* Memory_require(Memory* self, size_t size,void* type) { return malloc(size); }
bool_t Memory_release(Memory* self, void* obj) { free(obj); return 1; }
void Memory_destruct(Memory* self, bool_t existed) {
	if (!existed) {
		if (self == Memory_defaultInstance)
			Memory_defaultInstance = 0;
		free(self);
	}
}

Memory* Memory_construct(Memory* self, TLogger* logger) {
	if (!self) {
		self = (Memory*)malloc(sizeof(Memory));
		if (!self) {
			printf_s("[ERR00001]:Memory___construct__:无法分配内存");
			exit(1);
			return NULL;
		}
	}
	self->vptr = (struct stMemoryVTBL*)((char*)self + sizeof(struct stMemoryVTBL*));
	self->require = self->require1 = Memory_require;
	self->release = Memory_release;
	self->increase = Memory_increase;
	self->decrease = Memory_decrease;
	self->destruct = Memory_destruct;
	self->logger = logger;
	return self;
}
