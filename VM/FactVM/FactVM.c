#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "FactVM.h"
#include "Memory.h"






Machine* Machine_construct(Machine* self,const MachineCreationOptions* opts) {
	if (!self) self = malloc(sizeof(Machine));
	if (!self) {
		printf_s("无法给Machine分配内存");
		exit(1);
	}
	//构建核心
	List_construct(&self->cores, sizeof(struct TCore), 0, 0);
	int coreCount = opts->coreCount > 0 ? opts->coreCount : 1;
	for (int i = 0; i < coreCount; i++) {
		struct TCore* core = List_add(&self->cores);
		core->machine = self;
	}
	//构建内存
	self->mmu = Memory_construct(0, (HOST_ADDR)opts->memoryPageSize, opts->memoryMaxPageCount);
	return self;
}