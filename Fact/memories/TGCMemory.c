#include "TGCMemory.h"



GCMemoryMeta TGCMemory__meta__;
const markNumber = 1 << (sizeof(usize_t) - 1);
const unmarkNumber = !(1 << (sizeof(usize_t) - 1));

TGCMemory* TGCMemory__construct__(TGCMemory* self, GCMemoryOptions* opts, TLogger* logger) {
	TGCMemory* p = (TGCMemory*)TAlignedMemory__construct__((TAlignedMemory*)self, (AlignedMemoryOptions*)opts, logger);
	((TObject*)p)->__meta__ = (ClazzMeta*)&TGCMemory__meta__;
	if (opts) {
		p->sweepBytes = opts->sweepBytes;
	}
	else p->sweepBytes = 0;
	p->unitKind = MemoryUnitKind_ref;
	return p;
};

MemoryAllocatingDirectives TGCMemory__allocating(TGCMemory* memory, usize_t size,uword_t masks, AlignedMemoryChunk* chunk) {
	if (memory->totalBytes && memory->allocatedBytes + chunk->pageSize > memory->totalBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMeta*)memory->__meta__)->collectGarbages((TAlignedMemory*)memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookup;
		return MemoryAllocatingDirective_fail;
	}
	if (memory->gcBytes && memory->allocatedBytes + chunk->pageSize > memory->gcBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMeta*)memory->__meta__)->collectGarbages((TAlignedMemory*)memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookupOrNew;
		else return MemoryAllocatingDirective_new;
	}

	if (memory->sweepBytes && memory->allocatedBytes + chunk->pageSize > memory->sweepBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMeta*)memory->__meta__)->collectGarbages((TAlignedMemory*)memory, 0, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookupOrNew;
		else return MemoryAllocatingDirective_new;
	}
	return MemoryAllocatingDirective_new;
}


void TGCMemory__markObject(TObject* obj) {
	// idle或已标记

	// 标记可达性
	(*((MRefUnit*)obj-1)).ref |= markNumber;
	usize_t memberIndex = 0;
	TType* type = get_type(obj);
	TField* field = (TField*)(&type->fields + 1);
	for (usize_t i = 0; i < type->fields->length; i++) {
		if (field->type->decorators & TypeKind_class) {
			TObject* refObj = (TObject*)(((byte_t*)obj) + field->offset);
			TGCMemory__markObject(refObj );
		}
	}	
}

void TGCMemory__markChunk(AlignedMemoryChunk* chunk) {
	//mark阶段
	AlignedMemoryPage* page = chunk->page;
	
	while (page) {
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		MRefUnit* gcUnit = (MRefUnit*)&page->free;
		for (usize_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcUnit->ref == 0 || gcUnit->ref & markNumber) return;
			TGCMemory__markObject((TObject*)(gcUnit +1));
			gcUnit = (MRefUnit*)((byte_t*)gcUnit + chunk->unitSize);
		}
		page = page->next;
	}
}


usize_t TGCMemory__sweepChunk(AlignedMemoryChunk* chunk) {
	AlignedMemoryPage* page = chunk->page;
	usize_t count = 0;
	while (page) {
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		MTObjUnit* gcObj = (MTObjUnit*)((byte_t*)page + sizeof(AlignedMemoryPage));
		for (usize_t i = 0, j = chunk->pageCapacity; i < j; i++) {
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

AlignedMemoryReleaseInfo TGCMemory_collectGarbages(TGCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.chunkCount = rs.pages = rs.pageSize = rs.units = rs.unitSize = 0;
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		TGCMemory__markChunk(chunk);
		chunk = chunk->nextChunk;
	}
	for (usize_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			TGCMemory__markChunk(chunk);
		}
	}

	while (chunk) {
		usize_t c = TGCMemory__sweepChunk(chunk);
		//if (log) log(chunk,c);
		chunk = chunk->nextChunk;
	}
	for (usize_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			usize_t c = TGCMemory__sweepChunk(chunk);
			//if (log) log(chunk, c);
		}
	}
	if (releasePage) TAlignedMemory_collectGarbages((TAlignedMemory*)self,1,callback);
	return rs;
}