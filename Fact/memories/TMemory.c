#include "TMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

TMemoryVTBL memoryVTBL;

TMemoryGCLayout TMemory_defaultInstance;

TMemory* TMemory_default = 0;

void* TMemory_alloc(TMemory* self, usize_t size) { 
	if (!size) {
		if (self && self->logger) TLogger_notice(self->logger,"TMemory.alloc","parameter size is required.");
		return 0;
	}
	if (!self || self->allocating == 0 || self->allocating(self, size,0)>0) {
		void* p = malloc(size);
		if (self && self->logger) {
			if (p)TLogger_trace(self->logger, "TMemory.alloc", "TMemory[%p] allocated:%ld", p, (long)size);
			else TLogger_warn(self->logger,"TMemory.alloc","Cannot alloc memory:%ld",(long)size);
		} 
		return p;
	}else return 0;
}
void* TMemory_alloc1(TMemory* self, usize_t size) { 
	if (!size) {
		if (self && self->logger) TLogger_notice(self->logger, "TMemory.alloc1", "parameter size is required.");
		return 0;
	}
	if (!self || self->allocating == 0 || self->allocating(self, size,0)>0) {
		void* p = malloc(size);
		if (self && self->logger) {
			if (p)TLogger_trace(self->logger, "TMemory.alloc", "TMemory[%p] allocated:%ld", p, (long)size);
			else TLogger_warn(self->logger, "TMemory.alloc", "Cannot alloc memory:%ld", (long)size);
		}
		return p;
	}
	else return 0;
}
bool_t TMemory_free(TMemory* self, void* p) { 
	if (!p) {
		if (self && self->logger) TLogger_notice(self->logger, "TMemory.free", "parameter p is required.");
		TLogger_notice(0, "TMemory.free", "parameter p is required.");
		return 0;
	}
	free(p); 
	if (self && self->logger) TLogger_trace(self->logger, "TMemory.free", "TMemory[%p] was free.", p);
	return 1;
}

void TMemory__destruct__(TMemory* self, bool_t existed) {
	if (!self) { TLogger_warn(0, "TMemory.__destruct__", "NULL should not invoke __dstruct__"); return; }
	if (!existed) {
		if (self != TMemory_default) {
			TLogger* logger = self->logger;
			free(self);
			if (logger) TLogger_trace(logger,"TMemory.__destruct__","Memory[%p] was destructed.", self);
		}
		else {
			TLogger_warn(0, "TMemory.__destruct__", "default Memory should not be destructed.");
			if (self->logger) TLogger_warn(self->logger, "TMemory.__destruct__", "default Memory should not be destructed.");
		}
	}
}

TMemory* TMemory__construct__(TMemory* self, const MemoryOptions* options,TLogger* logger) {
	if (!self) {
		self = (TMemory*)malloc(sizeof(TMemory));
		if (!self) {
			log_exit(1,"TMemory.__construct__","Cannot allocate memory.");
			return 0;
		}
		else {
			if (logger) {
				if (logger) TLogger_trace(logger, "TTMemory.__construct__", "Memory is allocated for <TMemory>[%p].",self);
			}
		}
	}
	self->vftptr = (vftptr_t)&memoryVTBL;
	if (options) {
		m_copy(&self->allocating,options,sizeof(MemoryOptions));
	}
	else {
		self->allocating = 0;
	}
	self->logger = logger;
	if (logger) TLogger_trace(logger,"TTMemory.__construct__","<TTMemory> constructed.");
	return self;
}
