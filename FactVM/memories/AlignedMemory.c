#include "AlignedMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>





AlignedMemoryChunk* AlignedMemory_getNormalChunk(AlignedMemory* self, size_t chunkIndex) {
	AlignedMemoryChunk* chunk = self->chunks[chunkIndex];
	if (chunk) return chunk;
	size_t unitSize;
	if (chunkIndex < 16) {
		unitSize = (chunkIndex + 1) * sizeof(word_t);
	}
	else if (chunkIndex < 24) {
		unitSize = 16 * sizeof(word_t) + (chunkIndex - 16 + 8) * sizeof(dword_t);
	}
	else { //chunkIndex<32
		unitSize = 16 * sizeof(dword_t) + (chunkIndex - 24 + 8) * sizeof(lword_t);
	}
	chunk = malloc(sizeof(AlignedMemoryChunk));
	if (!chunk) {
		printf_s("[ERR00001]:AlignedMemory_getNormalChunk:无法分配内存");
		exit(1);
		return NULL;
	}
	chunk->page = 0;
	chunk->memory = self;
	chunk->nextChunk = 0;
	chunk->pageSize = self->pageSize;
	chunk->unitSize = unitSize;
	chunk->pageCapacity = (self->pageSize - sizeof(AlignedMemoryPage)) / unitSize;
	return chunk;
}

AlignedMemoryChunk* AlignedMemory_getLargeChunk(AlignedMemory* self, size_t size) {
	//假设大内存不常用，用链表处理
	if (size % sizeof(lword_t)) size = (size / sizeof(lword_t)) + 1;
	AlignedMemoryChunk* existed = self->large;
	AlignedMemoryChunk* prev = 0;
	while (existed) {
		if (existed->unitSize == size) return existed;
		else if (existed->unitSize < size) {
			prev = existed;
			existed = existed->nextChunk;
		}
		else break;
	}
	size_t pageSize = self->pageSize;
	size_t pageUsableSize = (pageSize - sizeof(AlignedMemoryPage));
	size_t pageCapacity = pageUsableSize / size;
	// 一页都无法装下一个
	if (pageCapacity == 0) {
		pageSize = size + sizeof(AlignedMemoryPage);
		pageCapacity = 1;
	}
	// 一页只能装一个
	else if (pageCapacity < 2) {
		//剩余的太多
		if (pageUsableSize % size > pageUsableSize / 4) {
			pageSize = size + sizeof(AlignedMemoryPage);
			pageCapacity = 1;
		}
	}
	AlignedMemoryChunk* chunk = malloc(sizeof(AlignedMemoryChunk));
	if (!chunk) {
		printf_s("[ERR00001]:AlignedMemory_getHugeChunk:无法分配内存");
		exit(1);
		return NULL;
	}
	chunk->page = 0;
	chunk->memory = self;
	chunk->nextChunk = existed;
	chunk->pageSize = pageSize;
	chunk->unitSize = size;
	chunk->pageCapacity = pageCapacity;
	if (prev) prev->nextChunk = chunk;
	else self->large = chunk;
	return chunk;
}

void* AlignedMemoryChunk_resolveIdleUnit(AlignedMemoryChunk* chunk,size_t unitSize) {
	AlignedMemoryPage* existed = chunk->page;
	AlignedMemoryPage* prev = 0;
	while (existed) {
		byte_t* unit = ((byte_t*)existed) + sizeof(AlignedMemoryPage);
		for (size_t i = 0; i < chunk->pageCapacity; i++) {
			if (((AlignedUnit*)unit)->ref == 0) return unit;
			else unit += unitSize;
		}
		existed = (prev= existed)->next;
	}
	if (chunk->memory->beforeAllocatePage ) {
		AllocatePageDirectives directive = chunk->memory->beforeAllocatePage(chunk);
		if (directive == AllocatePageDirective_Fail) return 0;
		if (directive == AllocatePageDirective_Recheck || directive == AllocatePageDirective_RecheckOrNewPage) {
			existed = chunk->page;
			prev = 0;
			while (existed) {
				byte_t* unit = ((byte_t*)existed) + sizeof(AlignedMemoryPage);
				for (size_t i = 0; i < chunk->pageCapacity; i++) {
					if (((AlignedUnit*)unit)->ref == 0) return unit;
					else unit += unitSize;
				}
				existed = (prev = existed)->next;
			}
		}
		if (directive == AllocatePageDirective_Recheck) return 0;
	}

	AlignedMemoryPage* page= (AlignedMemoryPage*)malloc(chunk->pageSize);
	if (!page) {
		printf_s("[ERR00001]:AlignedMemoryChunk_resolveIdleUnit:无法分配内存");
		exit(1);
		return NULL;
	}
	byte_t* unit = ((byte_t*)page) + sizeof(AlignedMemoryPage);
	for (size_t i = 0; i < chunk->pageCapacity; i++) {
		((AlignedUnit*)unit)->ref =0;
		unit += unitSize;
	}
	if (prev) prev->next = page;
	else chunk->page = page;
	return ((byte_t*)page) + sizeof(AlignedMemoryPage);
}

void* AlignedMemory_require(AlignedMemory* self, size_t size, void* type) {
	size_t chunkIndex;
	size += self->unitMetaSize;
	AlignedMemoryChunk* chunk;
	if (size <= 16 * sizeof(word_t)) {// 16 个 1word 增加的 最大的是 16word, 0-15
		chunkIndex = size / sizeof(word_t);
		if (size % sizeof(word_t)) chunkIndex++;
		chunk = AlignedMemory_getNormalChunk(self, chunkIndex);
	}
	else if (size <= 16 * sizeof(dword_t)) {// 最大的 32word 15 - 31 128 
		chunkIndex =16-8+ size / (sizeof(dword_t));
		if (size % sizeof(dword_t)) chunkIndex++;
		chunk = AlignedMemory_getNormalChunk(self, chunkIndex);
	}
	else if (size <= 16 * sizeof(lword_t)) {// 最大 64 word 256bytes 24-28
		chunkIndex = 24 -8 + size / (sizeof(lword_t));
		if (size %  sizeof(lword_t)) chunkIndex++;
		chunk = AlignedMemory_getNormalChunk(self, chunkIndex);
	}else {
		chunk = AlignedMemory_getLargeChunk(self, size);
	}
	return (byte_t*)AlignedMemoryChunk_resolveIdleUnit(chunk,size) + self->unitMetaSize;
}
void AlignedMemoryChunk___destruct__(AlignedMemoryChunk* chunk) {
	AlignedMemoryPage* page = chunk->page;
	while (page) {
		AlignedMemoryPage* next = page->next;
		free(page);
		page = next;
	}
	free(chunk);
}
void AlignedMemory___destruct__(AlignedMemory* self,bool_t existed) {
	for (size_t i = 0; i < 32; i++) {
		if (self->chunks[i]) {
			AlignedMemoryChunk___destruct__(self->chunks[i]);
			self->chunks[i] = 0;
		}
	}
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		AlignedMemoryChunk* next = chunk->nextChunk;
		AlignedMemoryChunk___destruct__(chunk);
		chunk = next;
	}
	self->large = 0;
	if (!existed) free(self);
}


AlignedMemory* AlignedMemory___construct__(AlignedMemory* self, AlignedMemoryOptions* opts,MemoryLogger* logger) {
	if (!self) {
		self = (AlignedMemory*)malloc(sizeof(Memory));
		if (!self) {
			printf_s("[ERR00001]:Memory___construct__:无法分配内存");
			exit(1);
			return NULL;
		}
	}
	self->vptr = (struct stMemoryVTBL*)((char*)self + sizeof(struct stMemoryVTBL*));
	self->require = (void*(*)(Memory*,size_t ,void*))AlignedMemory_require;
	self->require1 = (void* (*)(Memory*, size_t, void*))AlignedMemory_require1;
	self->release = (bool_t(*)(Memory*, void*)) AlignedMemory_release;
	self->increase = (bool_t(*)(Memory*, void*))AlignedMemory_increase;
	self->decrease = (bool_t(*)(Memory*, void*))AlignedMemory_decrease;
	self->destruct =(void(*)(Memory*,bool_t)) AlignedMemory___destruct__;
	self->logger = logger;

	
	void* cfg = ((&self->destruct) + 1);
	//拷贝配置
	if (!opts) {
		AlignedMemoryOptions dftOpts;
		dftOpts.pageSize = 1024*2;
		dftOpts.pageCount = 0;
		Memory_copy(cfg, &dftOpts, sizeof(AlignedMemoryOptions));
	}else Memory_copy(cfg, opts, sizeof(AlignedMemoryOptions));

	for (size_t i = 0; i < 32; i++) self->chunks[i] = 0;
	self->large = 0;
	return self;
}