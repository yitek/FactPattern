#include "GCMemory.h"



GCMemoryMETA gcMemoryMETA;
const markNumber = 1 << (sizeof(usize_t) - 1);
const unmarkNumber = !(1 << (sizeof(usize_t) - 1));

GCMemory* GCMemory__construct__(GCMemory* self, GCMemoryOptions* opts, Logger* logger) {
	GCMemory* p = (GCMemory*)AlignedMemory__construct__((AlignedMemory*)self, (AlignedMemoryOptions*)opts, logger);
	((TObject*)p)->__meta__ = (ObjectMetaLayout*)&gcMemoryMETA;
	if (opts) {
		p->sweepBytes = opts->sweepBytes;
	}
	else p->sweepBytes = 0;
	p->unitKind = MemoryUnitKind_ref;
	return p;
};

MemoryAllocatingDirectives GCMemory__allocating(GCMemory* memory, usize_t size,uword_t masks, AlignedMemoryChunk* chunk) {
	if (memory->totalBytes && memory->allocatedBytes + chunk->pageSize > memory->totalBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMETA*)memory->__meta__)->collectGarbages((AlignedMemory*)memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookup;
		return MemoryAllocatingDirective_fail;
	}
	if (memory->gcBytes && memory->allocatedBytes + chunk->pageSize > memory->gcBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMETA*)memory->__meta__)->collectGarbages((AlignedMemory*)memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookupOrNew;
		else return MemoryAllocatingDirective_new;
	}

	if (memory->sweepBytes && memory->allocatedBytes + chunk->pageSize > memory->sweepBytes) {
		AlignedMemoryReleaseInfo rs = ((AlignedMemoryMETA*)memory->__meta__)->collectGarbages((AlignedMemory*)memory, 0, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookupOrNew;
		else return MemoryAllocatingDirective_new;
	}
	return MemoryAllocatingDirective_new;
}


void GCMemory__markObject(TObject* obj) {
	// idle或已标记

	// 标记可达性
	(*((MemoryRefUnit*)obj-1)).ref |= markNumber;
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
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		MemoryRefUnit* gcUnit = (MemoryRefUnit*)&page->free;
		for (usize_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (gcUnit->ref == 0 || gcUnit->ref & markNumber) return;
			GCMemory__markObject((TObject*)(gcUnit +1));
			gcUnit = (MemoryRefUnit*)((byte_t*)gcUnit + chunk->unitSize);
		}
		page = page->next;
	}
}


usize_t GCMemory__sweepChunk(AlignedMemoryChunk* chunk) {
	AlignedMemoryPage* page = chunk->page;
	usize_t count = 0;
	while (page) {
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		ObjectLayout* gcObj = (ObjectLayout*)((byte_t*)page + sizeof(AlignedMemoryPage));
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

AlignedMemoryReleaseInfo GCMemory_collectGarbages(GCMemory* self, bool_t releasePage, AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.chunkCount = rs.pages = rs.pageSize = rs.units = rs.unitSize = 0;
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		GCMemory__markChunk(chunk);
		chunk = chunk->nextChunk;
	}
	for (usize_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			GCMemory__markChunk(chunk);
		}
	}

	while (chunk) {
		usize_t c = GCMemory__sweepChunk(chunk);
		//if (log) log(chunk,c);
		chunk = chunk->nextChunk;
	}
	for (usize_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			usize_t c = GCMemory__sweepChunk(chunk);
			//if (log) log(chunk, c);
		}
	}
	return rs;
}