#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

MemoryMETA memoryMETA;

MemoryGCLayout Memory_defaultInstance;

Memory* Memory_default = 0;



void* Memory_alloc(Memory* self, usize_t size,uword_t masks) { 
	if (!size) {
		if (self && self->logger) Logger_notice(self->logger,"Memory.alloc","parameter size is required.");
		return 0;
	}
	if (!self || ((MemoryMETA*)self->__meta__)->allocating == 0 || ((MemoryMETA*)self->__meta__)->allocating(self, size,masks,0)>0) {
		void* p = m_alloc(size,masks);
		if (self && self->logger) {
			if (p)Logger_trace(self->logger, "Memory.alloc", "Memory[%p] allocated:%ld", p, (long)size);
			else Logger_warn(self->logger,"Memory.alloc","Cannot alloc memory:%ld",(long)size);
		} 
		return p;
	}else return 0;
}

bool_t Memory_free(Memory* self, void* p) { 
	if (!p) {
		if (self && self->logger) Logger_notice(self->logger, "Memory.free", "parameter p is required.");
		Logger_notice(0, "Memory.free", "parameter p is required.");
		return 0;
	}
	m_free(p); 
	if (self && self->logger) Logger_trace(self->logger, "Memory.free", "Memory[%p] was free.", p);
	return 1;
}

void Memory__destruct__(Memory* self, bool_t existed) {
	if (!self) { Logger_warn(0, "Memory.__destruct__", "NULL should not invoke __dstruct__"); return; }
	if (!existed) {
		if (self != Memory_default) {
			Logger* logger = self->logger;
			free(self);
			if (logger) Logger_trace(logger,"Memory.__destruct__","Memory[%p] was destructed.", self);
		}
		else {
			Logger_warn(0, "Memory.__destruct__", "default Memory should not be destructed.");
			if (self->logger) Logger_warn(self->logger, "Memory.__destruct__", "default Memory should not be destructed.");
		}
	}
}

Memory* Memory__construct__(Memory* self, const MemoryOptions* options,Logger* logger) {
	if (!self) {
		self = (Memory*)malloc(sizeof(Memory));
		if (!self) {
			log_exit(ExitCode_memory,"Memory.__construct__","Cannot allocate memory.");
			return 0;
		}
		else {
			if (logger) {
				if (logger) Logger_trace(logger, "TMemory.__construct__", "Memory is allocated for <Memory>[%p].",self);
			}
		}
	}
	self->__meta__ = (ObjectMetaLayout*)&memoryMETA;
	if (options) {
		self->totalBytes = options->totalBytes;
	}
	else {
		self->totalBytes = 0;
	}
	self->logger = logger;
	if (logger) Logger_trace(logger,"TMemory.__construct__","<TMemory> constructed.");
	return self;
}


