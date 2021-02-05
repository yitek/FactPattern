#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AlignAllocator.h"


AlignChunk* AlignAllocator_resolveGeneralChunk(AlignAllocator* self, size_t size);
AlignLargeChunk* AlignAllocator_resolveLargeChunk(AlignAllocator* self, size_t size);
AlignLargeChunk* AlignAllocator_createLargChunk(AlignLargeChunk* nextChunk, size_t pageSize, size_t unitSize);
void AlignAllocator_collectGarbages(AlignAllocator* self);

AlignObject* AlignAllocator_chunkCreatePage(AlignChunk* chunk);
AlignObject* AlignAllocator_chunkRequire(AlignAllocator* self,AlignChunk* chunk);
int AlignAllocator_chunkRelease(AlignChunk* chunk, AlignObject* p, size_t pageSize);
void AllignAllocator_chunkSweep(AlignChunk* chunk);
void AllignAllocator_chunkMark(AlignChunk* chunk);

AlignObject* AlignAllocator_pageFindIdle(struct TAlignPage* page, size_t capacity, size_t unitSize);

void AllignAllocator_markGCObject(AlignObject* gcObj);


int AlignAllocator_printTrace = 1;

AlignAllocator* AlignAllocator_construct(AlignAllocator* self, const AlignAllocatorOptions* params) {
	//自己的结构+32个chunk指针
	if (!self) self = (AlignAllocator*)malloc(sizeof(AlignAllocator));
	if (!self) {
		printf_s("AlignAllocator_construct:无法分配内存");
		exit(1);
		return 0;
	}
	self->require = (void* (*)(struct TMAllocator* self, size_t size))AlignAllocator_require;
	self->release =(int (*)(struct TMAllocator* self, void* p)) AlignAllocator_release;
	if (params) {
		memcpy((char*)(&self->pageSize),params,sizeof(AlignAllocatorOptions));
	}
	else {
		self->pageSize = 1024 * 2;
		self->pageCount = 1024 * 2;
	}
	AlignChunk** chunks = (AlignChunk**)(&self->large);
	memset(chunks,0,(size_t)((char*)&self->chunk128-(char*)&self->large));
	self->chunk128 = 0;
	return self;
}


void* AlignAllocator_require(AlignAllocator* self, size_t size) {
	AlignChunk* chunk;
	if (size > 128) chunk = (AlignChunk*)AlignAllocator_resolveLargeChunk(self, size);
	else chunk = AlignAllocator_resolveGeneralChunk(self,size);

	AlignObject* alignObject = AlignAllocator_chunkRequire(self,chunk);
	alignObject->ref = 1;
	return (void*)(alignObject +1);
}
AlignChunk* AlignAllocator_resolveGeneralChunk(AlignAllocator* self, size_t size) {
	size_t chunkIndex;
	if (size <= 4) chunkIndex = 1;
	else if (size <= 20) chunkIndex = size / 2;
	else if (size <= 40) chunkIndex = size / 4;
	else if (size <= 80) chunkIndex = size / 8;
	else if (size <= 128) chunkIndex = size / 16;
	else chunkIndex = 0;
	
	AlignChunk** chunks = (AlignChunk**)(&self->large);
	AlignChunk* chunk = chunks[chunkIndex];
	size_t unitSize;
	if (!chunk) {
		chunk = (AlignChunk*)malloc(sizeof(AlignChunk));
		if (!chunk) {
			printf_s("AlignAllocator_AllocateBox:无法分配内存");
			exit(1);
			return 0;
		}
		if (size <= 4) unitSize = 4;
		else if (size <= 20) unitSize = chunkIndex * 2;
		else if (size <= 40) unitSize = chunkIndex * 4;
		else if (size <= 80) unitSize = chunkIndex * 8;
		else if (size <= 128) unitSize = chunkIndex * 16;
		else unitSize = 0;

		chunk->unitSize = unitSize;
		chunk->allocator = self;
		chunk->pageSize = self->pageSize;
		chunk->pageCapacity = (self->pageSize - sizeof(AlignPage)) / unitSize;
		chunk->headPage = chunk->tailPage = 0;
		chunks[chunkIndex] = chunk;
	}
	return chunk;
}
AlignLargeChunk* AlignAllocator_resolveLargeChunk(AlignAllocator* self, size_t size) {
	//计算Unit大小,按照64Bytes对齐
	size_t c = size / 64;
	if (size % 64) c++;
	size_t unitSize = c * 64;
	//计算页大小
	size_t pageSize;
	if (size <= self->pageSize) {
		c = (self->pageSize +sizeof(AlignPage)) / unitSize;
		if (c < 2) pageSize = self->pageSize * 2;
		else pageSize = self->pageSize;
	}
	else {
		c = (unitSize + sizeof(AlignPage))/ self->pageSize;
		if (unitSize + sizeof(AlignPage) % self->pageSize) c++;
		pageSize = c * self->pageSize;
		unitSize = pageSize;
	}
	//查找chunk
	AlignLargeChunk* chunk = self->large;
	AlignLargeChunk* prev = 0;
	while (chunk) {
		if (chunk->unitSize == unitSize) break;
		if (chunk->unitSize > unitSize) {
			chunk = AlignAllocator_createLargChunk(chunk, pageSize, unitSize);
			chunk->allocator = self;
			break;
		}
		prev = chunk;
		chunk = chunk->nextChunk;
	}
	if (!chunk) {
		chunk = AlignAllocator_createLargChunk(0, pageSize, unitSize);
		chunk->allocator = self;
		if (prev) {
			prev->nextChunk = chunk;
			chunk->prevChunk = prev;
		}
		else {
			self->large = chunk;
		}
	}
	return chunk;
	
}

AlignLargeChunk* AlignAllocator_createLargChunk(AlignLargeChunk* nextChunk, size_t pageSize, size_t unitSize) {
	AlignLargeChunk* chk = (AlignLargeChunk*)malloc(sizeof(AlignLargeChunk));
	if (!chk) {
		printf_s("AlignAllocator_requireLarge:无法分配内存");
		exit(1);
		return 0;
	}
	chk->unitSize = unitSize;
	chk->pageSize = pageSize;
	chk->pageCapacity = pageSize / unitSize;
	if (nextChunk) {
		chk->prevChunk = nextChunk->prevChunk;
		chk->nextChunk = nextChunk;
		nextChunk->prevChunk = chk;
	}
	else {
		chk->nextChunk = chk->prevChunk = 0;
	}
	
	return chk;
}
AlignObject* AlignAllocator_chunkRequire(AlignAllocator* allocator,AlignChunk* chunk) {
	size_t unitSize = chunk->unitSize, pageSize = chunk->pageSize;
	struct TAlignPage* page = chunk->headPage;
	AlignObject* unit = AlignAllocator_pageFindIdle(page, chunk->pageCapacity, unitSize);
	
	if (!unit) {
		// 没有在已经存在的页中找到空闲单元
		// 如果已经有页分配了
		if (chunk->headPage) {
			if (allocator->enableGC) {
				AlignAllocator_collectGarbages(allocator);
				unit = AlignAllocator_pageFindIdle(page, chunk->pageCapacity, unitSize);
			}			
		}

		if (!unit) {
			unit = AlignAllocator_chunkCreatePage(chunk);
		}
	}
	return unit;
}

AlignObject* AlignAllocator_pageFindIdle(struct TAlignPage* page, size_t capacity, size_t unitSize) {
	while (page) {
		AlignObject* unit = (AlignObject*)&page->first;
		size_t c = 0;
		while (c < capacity) {
			if (unit->ref == 0) {
				return unit;
			}
			c++;
			unit = (AlignObject*)((char*)unit + unitSize);
		}
		//没找到空闲单元，找next 页
		page = page->next;
	}
	return 0;
}

AlignObject* AlignAllocator_chunkCreatePage(AlignChunk* chunk) {
	struct TAlignPage* page = (struct TAlignPage*)malloc(chunk->pageSize);
	if (!page) {
		printf_s("GCAllocator_AllocateChunkPage:无法分配内存");
		exit(1);
		return 0;
	}
	AlignObject* unit = (AlignObject*)&page->first;
	//全部设置为0
	for (size_t i = 0; i < chunk->pageCapacity; i++) {
		unit->ref = 0;
		unit = (AlignObject*)((char*)unit + chunk->unitSize);
	}
	if (chunk->tailPage) {
		chunk->tailPage = chunk->tailPage->next = page;
	}
	else chunk->tailPage = chunk->headPage = page;
	page->next = 0;

	if (AlignAllocator_printTrace) {
		printf_s("内存页已请求:unitsize=%ld,pagesize=%ld\n", chunk->pageSize, chunk->unitSize);
	}

	return (AlignObject*)&page->first;

}
int AlignAllocator_release(AlignAllocator* self,void* obj) {
	size_t pageSize = self->pageSize;
	AlignObject* p = ((AlignObject*)obj) - 1;
	if(AlignAllocator_chunkRelease(self->chunk4,p,pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk6, p, pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk8, p, pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk10, p, pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk12, p, pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk16, p, pageSize)) return 1;
	if (AlignAllocator_chunkRelease(self->chunk18, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk20, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk24, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk28, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk32, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk36, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk40, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk48, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk56, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk64, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk72, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk80, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk96, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk112, p, pageSize)) return 1;
	if(AlignAllocator_chunkRelease(self->chunk128, p, pageSize)) return 1;
	AlignLargeChunk* largChunk = self->large;
	while (largChunk) {
		if (AlignAllocator_chunkRelease((AlignChunk*)largChunk, p, largChunk->pageSize)) return 1;
		largChunk = largChunk->nextChunk;
	}
	return 0;
}
int AlignAllocator_chunkRelease(AlignChunk* chunk,AlignObject* p ,size_t pageSize) {
	size_t pageSize1 = pageSize - sizeof(AlignPage);
	AlignPage* page = chunk->headPage;
	while (page) {
		void* startAddr = (void*)&page->first;
		void* endAddr = (char*)page + pageSize1;
		if ((void*)p >= startAddr && (void*)p <= endAddr) {
			p->ref = 0;
			return 1;
		}
		page = page->next;
	}
	return 0;
}



const markNumber = 1 << (sizeof(size_t) - 1);
const unmarkNumber = !(1 << (sizeof(size_t) - 1));

void AlignAllocator_collectGarbages(AlignAllocator* self) {
	AllignAllocator_chunkMark(self->chunk4);
	AllignAllocator_chunkMark(self->chunk6);
	AllignAllocator_chunkMark(self->chunk8);
	AllignAllocator_chunkMark(self->chunk10);
	AllignAllocator_chunkMark(self->chunk12);
	AllignAllocator_chunkMark(self->chunk16);
	AllignAllocator_chunkMark(self->chunk18);
	AllignAllocator_chunkMark(self->chunk20);
	AllignAllocator_chunkMark(self->chunk24);
	AllignAllocator_chunkMark(self->chunk28);
	AllignAllocator_chunkMark(self->chunk32);
	AllignAllocator_chunkMark(self->chunk36);
	AllignAllocator_chunkMark(self->chunk40);
	AllignAllocator_chunkMark(self->chunk48);
	AllignAllocator_chunkMark(self->chunk56);
	AllignAllocator_chunkMark(self->chunk64);
	AllignAllocator_chunkMark(self->chunk72);
	AllignAllocator_chunkMark(self->chunk80);
	AllignAllocator_chunkMark(self->chunk96);
	AllignAllocator_chunkMark(self->chunk112);
	AllignAllocator_chunkMark(self->chunk128);
	AlignLargeChunk* large = self->large;
	while (large) {
		AllignAllocator_chunkMark((AlignChunk*)large);
		large = large->nextChunk;
	}
	AllignAllocator_chunkSweep(self->chunk4);
	AllignAllocator_chunkSweep(self->chunk6);
	AllignAllocator_chunkSweep(self->chunk8);
	AllignAllocator_chunkSweep(self->chunk10);
	AllignAllocator_chunkSweep(self->chunk12);
	AllignAllocator_chunkSweep(self->chunk16);
	AllignAllocator_chunkSweep(self->chunk18);
	AllignAllocator_chunkSweep(self->chunk20);
	AllignAllocator_chunkSweep(self->chunk24);
	AllignAllocator_chunkSweep(self->chunk28);
	AllignAllocator_chunkSweep(self->chunk32);
	AllignAllocator_chunkSweep(self->chunk36);
	AllignAllocator_chunkSweep(self->chunk40);
	AllignAllocator_chunkSweep(self->chunk48);
	AllignAllocator_chunkSweep(self->chunk56);
	AllignAllocator_chunkSweep(self->chunk64);
	AllignAllocator_chunkSweep(self->chunk72);
	AllignAllocator_chunkSweep(self->chunk80);
	AllignAllocator_chunkSweep(self->chunk96);
	AllignAllocator_chunkSweep(self->chunk112);
	AllignAllocator_chunkSweep(self->chunk128);
	large = self->large;
	while (large) {
		AllignAllocator_chunkMark((AlignChunk*)large);
		large = large->nextChunk;
	}
}


void AllignAllocator_chunkMark(AlignChunk* chunk) {
	//mark阶段
	AlignPage* page = chunk->headPage;
	while (page) {
		AlignObject* gcObj = (AlignObject*)&page->first;
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcObj->ref == 0 || gcObj->ref & markNumber) return;
			AllignAllocator_markGCObject(gcObj);
		}
		page = page->next;

	}
}


void AllignAllocator_chunkSweep(AlignChunk* chunk) {
	AlignPage* page = chunk->headPage;
	while (page) {
		AlignObject* gcObj = (AlignObject*)&page->first;
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcObj->ref | markNumber) {
				//标记过，有引用,还原引用计数
				gcObj->ref &= unmarkNumber;
			}
			//否则引用计数设0
			else gcObj->ref = 0;

		}
		page = page->next;

	}
}
void AllignAllocator_markGCObject(AlignObject* gcObj) {
	// idle或已标记

	// 标记可达性
	gcObj->ref |= markNumber;
	// 获取到类型
	struct TType* type = gcObj->type;
	struct TTypeMember* member = (struct TTypeMember*)((char*)type + sizeof(struct TType));
	void* obj = GCObject_toObject(gcObj);
	for (int i = 0, j = type->memberCount; i < j; i++) {
		if (member->type->assignType == AssignType_GCRef) {
			void* memberObj = (void*)((char*)obj + member->offset);
			AlignObject* memberGCObj = GCObject_fromObject(memberObj);
			if (memberGCObj->ref == 0 || memberGCObj->ref & markNumber) continue;
			AllignAllocator_markGCObject(memberGCObj);
		}
	}
}
