#include "TMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

MemoryMeta TMemory__meta__;

struct stMemoryGCLayout {
	struct stMRefUnit;
	struct stTMemory;
} TMemory_defaultInstance = {
	.ref = 0,
	.__meta__ = (ClazzMeta*)&TMemory__meta__,
	.totalBytes = 0,
	.logger = 0
};

TMemory* TMemory_default = (TMemory*)((byte_t*)&TMemory_defaultInstance + sizeof(MRefUnit));;



void* TMemory_alloc(TMemory* self, usize_t size,uword_t masks) { 
	if (!size) {
		if (self && self->logger) TLogger_notice(self->logger,"Memory.alloc","parameter size is required.");
		return 0;
	}
	if (!self || ((MemoryMeta*)self->__meta__)->allocating == 0 || ((MemoryMeta*)self->__meta__)->allocating(self, size,masks,0)>0) {
		void* p = m_alloc(size,masks);
		if (self && self->logger) {
			if (p)TLogger_trace(self->logger, "Memory.alloc", "Memory[%p] allocated:%ld", p, (long)size);
			else TLogger_warn(self->logger,"Memory.alloc","Cannot alloc memory:%ld",(long)size);
		} 
		return p;
	}else return 0;
}

bool_t TMemory_free(TMemory* self, void* p) { 
	if (!p) {
		if (self && self->logger) TLogger_notice(self->logger, "Memory.free", "parameter p is required.");
		TLogger_notice(0, "Memory.free", "parameter p is required.");
		return 0;
	}
	m_free(p); 
	if (self && self->logger) TLogger_trace(self->logger, "Memory.free", "Memory[%p] was free.", p);
	return 1;
}

void TMemory__destruct__(TMemory* self, bool_t existed) {
	if (!self) { TLogger_warn(0, "Memory.__destruct__", "NULL should not invoke __dstruct__"); return; }
	if (!existed) {
		if (self != TMemory_default) {
			TLogger* logger = self->logger;
			free(self);
			if (logger) TLogger_trace(logger,"Memory.__destruct__","Memory[%p] was destructed.", self);
		}
		else {
			TLogger_warn(0, "Memory.__destruct__", "default Memory should not be destructed.");
			if (self->logger) TLogger_warn(self->logger, "Memory.__destruct__", "default Memory should not be destructed.");
		}
	}
}

TMemory* TMemory__construct__(TMemory* self, const MemoryOptions* options,TLogger* logger) {
	if (!self) {
		self = (TMemory*)malloc(sizeof(TMemory));
		if (!self) {
			log_exit(ExitCode_memory,"Memory.__construct__","Cannot allocate memory.");
			return 0;
		}
		else {
			if (logger) {
				if (logger) TLogger_trace(logger, "TMemory.__construct__", "Memory is allocated for <Memory>[%p].",self);
			}
		}
	}
	self->__meta__ = (ClazzMeta*)&TMemory__meta__;
	if (options) {
		self->totalBytes = options->totalBytes;
	}
	else {
		self->totalBytes = 0;
	}
	self->logger = logger;
	if (logger) TLogger_trace(logger,"TMemory.__construct__","<TMemory> constructed.");
	return self;
}


