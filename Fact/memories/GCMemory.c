#include "GCMemory.h"



GCMemoryMETA gcMemoryMETA;
const markNumber = 1 << (sizeof(usize_t) - 1);
const unmarkNumber = !(1 << (sizeof(usize_t) - 1));

word_t GCMemory__allocating(GCMemory* self, usize_t t, void* param) {
	GCMemory_collectGarbages(self, 0, 0);
	return AllocatePageDirective_RecheckOrNewPage;
}


void GCMemory__markObject(TObject* obj) {
	// idle或已标记

	// 标记可达性
	(*((GCUnitLayout*)obj-1)).ref |= markNumber;
	usize_t memberIndex = 0;
	TType* type = get_type(obj);
	TField* field = (TField*)(&type->fields + 1);
	for (usize_t i = 0; i < type->fields->length; i++) {
		if (field->type->decorators & TypeKind_class) {
			TObject* refObj = (TObject*)(((byte_t*)obj) + field->offset);
			GCMemory__markObject(refObj );
		}
	}	
}

void GCMemory__markChunk(AlignedMemoryChunk* chunk) {
	//mark阶段
	AlignedMemoryPage* page = chunk->page;
	
	while (page) {
		GCUnitLayout* gcUnit = (GCUnitLayout*)&page->free;
		for (size_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcUnit->ref == 0 || gcUnit->ref & markNumber) return;
			GCMemory__markObject((TObject*)(gcUnit +1));
			gcUnit = (GCUnitLayout*)((byte_t*)gcUnit + chunk->unitSize);
		}
		page = page->next;
	}
}


size_t GCMemory__sweepChunk(AlignedMemoryChunk* chunk) {
	AlignedMemoryPage* page = chunk->page;
	size_t count = 0;
	while (page) {
		ObjectLayout* gcObj = (ObjectLayout*)((byte_t*)page + sizeof(AlignedMemoryPage));
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

AlignedMemoryReleaseInfo GCMemory_collectGarbages(GCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.chunkCount = rs.pages = rs.pageSize = rs.units = rs.unitSize = 0;
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		GCMemory__markChunk(chunk);
		chunk = chunk->nextChunk;
	}
	for (size_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			GCMemory__markChunk(chunk);
		}
	}

	while (chunk) {
		size_t c = GCMemory__sweepChunk(chunk);
		//if (log) log(chunk,c);
		chunk = chunk->nextChunk;
	}
	for (size_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			size_t c = GCMemory__sweepChunk(chunk);
			//if (log) log(chunk, c);
		}
	}
	return rs;
}