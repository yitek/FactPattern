#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GCAllocator.h"
#include "Type.h"

/// <summary>
/// ��Chunk������ҳ
/// </summary>
/// <param name="chunk"></param>
/// <returns>���ص�һ�����õ��ڴ浥Ԫ</returns>
GCObject* GCAllocator_allocateChunkPage(GCChunk* chunk);

void GCAllocator_collectGarbages(GCAllocator* self);
/// <summary>
/// ��������
/// </summary>
/// <param name="self"></param>
void GCAllocator_markChunkGarbages(GCChunk* chunk);

void GCAllocator_markGCObject(GCObject* gcObj);

GCAllocator* GCAllocator_construct(GCAllocator* self,void* params) {
	//�Լ��Ľṹ+32��chunkָ��
	if (!self) self = (GCAllocator*)malloc(sizeof(GCAllocator) + 32*sizeof(void*));
	if (!self) {
		printf_s("GCAllocator_construct:�޷������ڴ�");
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
		//û�ҵ����е�Ԫ����next ҳ
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
		//TODO: ��ȡ�����
	}
	GCChunk** chunks = (GCChunk**)(&self->large);
	GCChunk* chunk = chunks[chunkIndex];
	size_t unitSize;
	if (!chunk) {
		chunk = (GCChunk*)malloc(self->pageSize);
		if (!chunk) {
			printf_s("GCAllocator_AllocateBox:�޷������ڴ�");
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
		// û�����Ѿ����ڵ�ҳ���ҵ����е�Ԫ
		// ����Ѿ���ҳ������
		if (chunk->head) {
			// GC�����������ڴ�
			GCAllocator_collectGarbages(self);
			// ���պ��ٴβ��ҿ��е�Ԫ
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
		printf_s("GCAllocator_AllocateChunkPage:�޷������ڴ�");
		exit(1);
		return 0;
	}
	GCObject* unit = (GCObject*)&page->first;
	//ȫ������Ϊ0
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
	//mark�׶�
	GCPage* page = chunk->head;
	while (page) {
		GCObject* gcObj = (GCObject*)&page->first;
		for (size_t i = 0, j = chunk->pageCapacity; i < j;i++) {
			if (gcObj->ref == 0 || gcObj->ref & markNumber) return;
			GCAllocator_markGCObject(gcObj);
		}
		page = page->next;
		
	}
	//sweep�׶�
	
}

void GCAllocator_sweepChunkGarbages(GCChunk* chunk) {
	GCPage* page = chunk->head;
	while (page) {
		GCObject* gcObj = (GCObject*)&page->first;
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcObj->ref | markNumber) {
				//��ǹ���������,��ԭ���ü���
				gcObj->ref &= unmarkNumber;
			}
			//�������ü�����0
			else gcObj->ref = 0;

		}
		page = page->next;

	}
}
void GCAllocator_markGCObject(GCObject* gcObj) {
	// idle���ѱ��
	
	// ��ǿɴ���
	gcObj->ref |= markNumber;
	// ��ȡ������
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
