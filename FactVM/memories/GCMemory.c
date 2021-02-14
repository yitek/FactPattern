#include "GCMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


GCMemory* GCMemory___construct__(GCMemory* self, FindReferenceObject findReferenceObject, AlignedMemoryOptions* opts,MemoryLogger* logger) {
	if (!self) {
		self = malloc(sizeof(GCMemory));
		if (!self) {
			printf_s("[ERR00001]:GCMemory___construct__:无法分配内存");
			exit(1);
			return NULL;
		}
	}
	AlignedMemory___construct__((AlignedMemory*)self,opts,logger);   
	self->beforeAllocatePage = GCMemory_beforeAllocatePage;
	self->findReferenceObject = findReferenceObject;
	self->require = (void* (*)(Memory*, size_t, void*)) GCMemory_require;
	self->require1 = (void*(*)(Memory*, size_t, void*)) GCMemory_require1;
	self->increase = (bool_t(*)(Memory*, void*)) GCMemory_increase;
	self->decrease = (bool_t (*)(Memory*, void*))GCMemory_decrease;
	return self;

}





const markNumber = 1 << (sizeof(size_t) - 1);
const unmarkNumber = !(1 << (sizeof(size_t) - 1));


void GCMemory_markObject(GCUnit* gcObj,FindReferenceObject findReferenceObject) {
	// idle或已标记

	// 标记可达性
	gcObj->ref |= markNumber;
	size_t memberIndex = 0;
	GCUnit* member = findReferenceObject(gcObj,memberIndex);
	while (member) {
		GCMemory_markObject(member, findReferenceObject);
		member = findReferenceObject(gcObj, ++memberIndex);
	}
	
}

void GCMemory_markChunk(AlignedMemoryChunk* chunk) {
	//mark阶段
	AlignedMemoryPage* page = chunk->page;
	FindReferenceObject findReferenceObject = ((GCMemory*)chunk->memory)->findReferenceObject;
	while (page) {
		GCUnit* gcObj = (GCUnit*)((byte_t*)page + sizeof(AlignedMemoryPage));
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcObj->ref == 0 || gcObj->ref & markNumber) return;
			GCMemory_markObject(gcObj, findReferenceObject);
		}
		page = page->next;
	}
}


size_t GCMemory_sweepChunk(AlignedMemoryChunk* chunk) {
	AlignedMemoryPage* page = chunk->page;
	size_t count = 0;
	while (page) {
		GCUnit* gcObj = (GCUnit*)((byte_t*)page + sizeof(AlignedMemoryPage));
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcObj->ref | markNumber) {
				//标记过，有引用,还原引用计数
				gcObj->ref &= unmarkNumber;
			}
			//否则引用计数设0
			else {
				gcObj->ref = 0;
				count++;
			}

		}
		page = page->next;
	}
	return count;
}

void GCMemory_collectGarbages(GCMemory* self, LogCollectGarbageResults log) {
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		GCMemory_markChunk(chunk);
		chunk = chunk->nextChunk;
	}
	for (size_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			GCMemory_markChunk(chunk);
		}
	}

	while (chunk) {
		size_t c = GCMemory_sweepChunk(chunk);
		if (log) log(chunk,c);
		chunk = chunk->nextChunk;
	}
	for (size_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			size_t c = GCMemory_sweepChunk(chunk);
			if (log) log(chunk, c);
		}
	}

}