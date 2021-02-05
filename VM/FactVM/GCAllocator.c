#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GCAllocator.h"
#include "Type.h"

/// <summary>
/// 给Chunk分配新页
/// </summary>
/// <param name="chunk"></param>
/// <returns>返回第一个可用的内存单元</returns>
GCObject* GCAllocator_allocateChunkPage(GCChunk* chunk);

void GCAllocator_collectGarbages(GCAllocator* self);
/// <summary>
/// 回收垃圾
/// </summary>
/// <param name="self"></param>
void GCAllocator_markChunkGarbages(GCChunk* chunk);

void GCAllocator_markGCObject(GCObject* gcObj);

GCAllocator* GCAllocator_construct(GCAllocator* self,void* params) {
	//自己的结构+32个chunk指针
	if (!self) self = (GCAllocator*)malloc(sizeof(GCAllocator) + 32*sizeof(void*));
	if (!self) {
		printf_s("GCAllocator_construct:无法分配内存");
		exit(1);
		return 0;
	}
	self->pageSize = 1024 * 2;// 2k
	return self;
}



GCObject* GCAllocator_findIdleUnit(struct TGCPage* page, size_t capacity, size_t unitSize) {
	while (page) {
		GCObject* unit = (GCObject*)&page->first;
		size_t c = 0;
		while (c < capacity) {
			if (unit->ref == -1) {
				unit->ref = 0;
				return unit;
			}
			c++;
			unit = (GCObject*)((char*)unit + unitSize);
		}
		//没找到空闲单元，找next 页
		page = page->next;
	}
	return 0;
}
void* GCAllocator_allocateUnit(GCAllocator* self, size_t size) {
	size_t chunkIndex;
	if (size < 4) chunkIndex = 1;
	else if (size <= 20) chunkIndex = size / 2;
	else if (size <= 40) chunkIndex = size / 4;
	else if (size <= 80) chunkIndex = size / 8;
	else if (size <= 128) chunkIndex = size / 16;
	else chunkIndex = 0;
	if (chunkIndex == 0) {
		//TODO: 获取大对象
	}
	GCChunk** chunks = (GCChunk**)(&self->large);
	GCChunk* chunk = chunks[chunkIndex];
	size_t unitSize;
	if (!chunk) {
		chunk = (GCChunk*)malloc(self->pageSize);
		if (!chunk) {
			printf_s("GCAllocator_AllocateBox:无法分配内存");
			exit(1);
			return 0;
		}
		if (size < 4) unitSize = 4;
		else if (size <= 20) unitSize = chunkIndex * 2;
		else if (size <= 40) unitSize = chunkIndex * 4;
		else if (size <= 80) unitSize = chunkIndex * 8;
		else if (size <= 128) unitSize = chunkIndex * 16;
		else unitSize = 0;
		chunk->unitSize = unitSize;
		chunk->mallocator = self;
		chunk->pageCapacity = self->pageSize / unitSize;
		chunk->head = chunk->tail = 0;
	}
	else unitSize = chunk->unitSize;
	struct TGCPage* page = chunk->head;
	GCObject* unit = GCAllocator_findIdleUnit(page,chunk->pageCapacity,unitSize);
	if (!unit) {
		// 没有在已经存在的页中找到空闲单元
		// 如果已经有页分配了
		if (chunk->head) {
			// GC操作，回收内存
			GCAllocator_collectGarbages(self);
			// 回收后再次查找空闲单元
			unit = GCAllocator_findIdleUnit(page, chunk->pageCapacity, unitSize);
		}
		
		if (!unit) {
			unit =GCAllocator_allocateChunkPage(chunk);
		}
	}
	return (void*)(unit + 1);
}

void* GCAllocator_allocateLarge(GCAllocator* self, size_t size) {
	self->large;
}

GCObject* GCAllocator_allocateChunkPage(GCChunk* chunk) {
	struct TGCPage * page = (struct TGCPage*)malloc(chunk->mallocator->pageSize);
	if (!page) {
		printf_s("GCAllocator_AllocateChunkPage:无法分配内存");
		exit(1);
		return 0;
	}
	GCObject* unit = (GCObject*)&page->first;
	//全部设置为0
	for (size_t i = 0; i < chunk->pageCapacity; i++) {
		unit->ref = 0;
		unit = (GCObject*)((char*)unit + chunk->unitSize);
	}
	if (chunk->tail) {
		chunk->tail = chunk->tail->next = page;
	}
	else chunk->tail = chunk->head = page;
	unit = (GCObject*)&page->first;
	
	return unit;
	
}
const markNumber = 1 << (sizeof(size_t)-1);
const unmarkNumber = !(1 << (sizeof(size_t) - 1));

void GCAllocator_collectGarbages(GCAllocator* self) {
	GCAllocator_markChunkGarbages(self->chunk4);
	GCAllocator_markChunkGarbages(self->chunk6);
	GCAllocator_markChunkGarbages(self->chunk8);
	GCAllocator_markChunkGarbages(self->chunk10);
	GCAllocator_markChunkGarbages(self->chunk12);
	GCAllocator_markChunkGarbages(self->chunk16);
	GCAllocator_markChunkGarbages(self->chunk18);
	GCAllocator_markChunkGarbages(self->chunk20);
	GCAllocator_markChunkGarbages(self->chunk24);
	GCAllocator_markChunkGarbages(self->chunk28);
	GCAllocator_markChunkGarbages(self->chunk32);
	GCAllocator_markChunkGarbages(self->chunk36);
	GCAllocator_markChunkGarbages(self->chunk40);
	GCAllocator_markChunkGarbages(self->chunk48);
	GCAllocator_markChunkGarbages(self->chunk56);
	GCAllocator_markChunkGarbages(self->chunk64);
	GCAllocator_markChunkGarbages(self->chunk72);
	GCAllocator_markChunkGarbages(self->chunk80);
	GCAllocator_markChunkGarbages(self->chunk96);
	GCAllocator_markChunkGarbages(self->chunk112);
	GCAllocator_markChunkGarbages(self->chunk128);
}

void GCAllocator_markChunkGarbages(GCChunk* chunk) {
	//mark阶段
	GCPage* page = chunk->head;
	while (page) {
		GCObject* gcObj = (GCObject*)&page->first;
		for (size_t i = 0, j = chunk->pageCapacity; i < j;i++) {
			if (gcObj->ref == 0 || gcObj->ref & markNumber) return;
			GCAllocator_markGCObject(gcObj);
		}
		page = page->next;
		
	}
	//sweep阶段
	
}

void GCAllocator_sweepChunkGarbages(GCChunk* chunk) {
	GCPage* page = chunk->head;
	while (page) {
		GCObject* gcObj = (GCObject*)&page->first;
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
void GCAllocator_markGCObject(GCObject* gcObj) {
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
			GCObject* memberGCObj = GCObject_fromObject(memberObj);
			if (memberGCObj->ref == 0 || memberGCObj->ref & markNumber) continue;
			GCAllocator_markGCObject(memberGCObj);
		}
	}
}
