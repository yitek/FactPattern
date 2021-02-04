#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "MAllocator.h"

MAllocator* MAllocator_defaultInstance = 0;

MAllocator* MAllocator_default() {
	return MAllocator_defaultInstance ? MAllocator_defaultInstance : (MAllocator_defaultInstance = MAllocator_construct(MAllocator_defaultInstance));

}


MAllocator* MAllocator_construct(MAllocator* self) {
	if (!self) self =(MAllocator*) malloc(sizeof(MAllocator));
	if (!self) {
		printf_s("MAllocator_construct:无法分配内存");
		exit(1);
		return 0;
	}
	self->allocate = malloc;
	self->release = free;
	return self;
}
void MAllocator_destruct(MAllocator* self) {
	if (self->destruct) self->destruct(self);
}




