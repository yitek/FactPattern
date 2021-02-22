#include "TAlignedMemory.h"
#include <stdio.h>
#include <stdlib.h>


TAlignedMemoryMeta TAlignedMemory__meta__ = {
	.alloc = (void* (*)(TMemory*, usize_t,void*,MemoryKinds))TAlignedMemory_alloc,
	.alloc1 = (void* (*)(TMemory*, usize_t,void*,MemoryKinds))TAlignedMemory_alloc1,
	.allocating = TAlignedMemory__allocating,
	.collectGarbages = TAlignedMemory_collectGarbages,
	.free = (bool_t(*)(TMemory*, void*))TAlignedMemory_free,
	.get_type=0,
	.offset=0,
	.__destruct__ = (void(*)(TMemory*, bool_t))TAlignedMemory__destruct__
};



static inline void* TAlignedMemory__initPageLinkUnits(AlignedMemoryChunk* chunk,AlignedMemoryPage* page, size_t unitSize,uword_t masks) {
	MLnkUnit* unit = (MLnkUnit*)(page +1);
	for (usize_t i = 0, j = chunk->pageCapacity - 1;i<j; i++) {
		unit->next = page->free;
		page->free = unit;
		unit = (MLnkUnit*)(((byte_t*)unit) + unitSize);
	}
	return unit;
}

static inline void* TAlignedMemoryMemory__initPageRefUnits(AlignedMemoryChunk* chunk, AlignedMemoryPage* page, size_t unitSize, uword_t masks) {
	byte_t* unit = (byte_t*)&page->free;
	for (usize_t i = 0; i < chunk->pageCapacity; i++) {
		((MTObjUnit*)unit)->ref = 0;
		unit = unit + unitSize;
	}
	return (void*)&page->free;
}

void* TAlignedMemory__chunkResolveUnit(AlignedMemoryChunk* chunk, size_t unitSize,MemoryKinds masks) {
	void* unit=0;
	MemoryAllocatingDirectives directive = ((TAlignedMemoryMeta*)chunk->memory->__meta__)->allocating(chunk->memory, chunk->pageSize, masks,chunk);
	if (directive == MemoryAllocatingDirective_fail) return 0;
	if (directive == MemoryAllocatingDirective_lookup || directive == MemoryAllocatingDirective_lookupOrNew) {
		if (chunk->memory->unitKind == MemoryUnitKind_link) {
			MemoryLookupLinkUnit(unit, chunk, unitSize, masks)
		}
		else if(chunk->memory->unitKind == MemoryUnitKind_link) {
			MemoryLookupRefUnit(unit, chunk, unitSize, masks)
		}
		else {
			log_exit(ExitCode_argument,"AlignedMemory._chunkResolveUnit","unitKind must be MemoryKind_link or MemoryKind_ref");
		}
		
		if (unit)return unit;
	}

	if (directive == MemoryAllocatingDirective_lookup) return 0;

	AlignedMemoryPage* page = (AlignedMemoryPage*)TMemory_alloc(0,chunk->pageSize,0,MemoryKind_disCollect);
	page->kind = masks;
	if (!page) {
		log_exit(ExitCode_memory, "AlignedMemory._resolveUnit", "Cannot alloc memory:%ld", (long)chunk->pageSize);
		return 0;
	}
	else {
		chunk->memory->allocatedBytes += chunk->pageSize;
	}
	page->free = 0;
	if (chunk->memory->unitKind == MemoryUnitKind_link) {
		unit = TAlignedMemory__initPageLinkUnits(chunk, page, unitSize,masks);
	}
	else {
		unit = TAlignedMemoryMemory__initPageRefUnits(chunk, page, unitSize, masks);
	}
	page->next = chunk->page;
	chunk->page = page;
	if (chunk->memory->logger) {
		TLogger_trace(chunk->memory->logger,"AlignedMemory._resolveUnit","<AlignedMemoryPage>[%p] was allocated and initialized:{ size: %ld, capacity: %ld ,!allocatedBytes: %ld }",page, chunk->pageSize,chunk->pageCapacity,chunk->memory->allocatedBytes);
	}
	return unit;
}



bool_t TAlignedMemory_freeLink(TAlignedMemory* self, void* p) {
	for (usize_t i = 0; i < 32; i++) {
		AlignedMemoryChunk* chunk = self->chunks[i];
		if(!chunk) continue;
		AlignedMemoryPage* page = chunk->page;
		usize_t maxOffset = chunk->pageCapacity* chunk->unitSize;
		while (page) {
			if (p > (void*)page) {
				usize_t offset = ((usize_t)p - sizeof(AlignedMemoryPage) - (usize_t)page);
				if (offset > maxOffset) {
					page = page->next; continue;
				}
				if (offset % chunk->unitSize) {
					return 0;
				}
				MLnkUnit* unit = (MLnkUnit*)p;
				unit->next = page->free;
				page->free = unit;
				return 1;
			}
			page = page->next;
		}
	}
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		AlignedMemoryPage* page = chunk->page;
		while (page) {
			if (p > (void*)page) {
				usize_t at = ((usize_t)p - sizeof(AlignedMemoryPage) - (usize_t)page);
				if (at > at / chunk->unitSize) {
					page = page->next; continue;
				}
				if (at % chunk->unitSize) {
					page = page->next; continue;
				}
				MLnkUnit* unit = (MLnkUnit*)p;
				unit->next = page->free;
				page->free = unit;
				return 1;
			}

		}
		chunk = chunk->nextChunk;
	}
	return 0;
}
void TAlignedMemory__destructChunk(TAlignedMemory* self,usize_t index, AlignedMemoryChunk* chunk, AlignedMemoryReleaseInfo*const rs) {
	usize_t pageCount = 0;
	
	AlignedMemoryPage* page = chunk->page;
	while (page) {
		pageCount++;
		AlignedMemoryPage* next = page->next;
		free(page);
		
		page = next;
	}
	usize_t byteCount = pageCount * chunk->pageSize;
	usize_t unitCount = chunk->pageCapacity * pageCount;
	if (self->logger) {
		usize_t unitSize = chunk->unitSize;
		usize_t pageSize = chunk->pageSize;
		free(chunk);
		if (index <32) {
			TLogger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemoryChunk>[%d][%p] for NORMAL is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(AlignedMemoryChunk), unitCount, unitSize, pageSize);
		}
		else {
			TLogger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemoryChunk>[%d][%p] for LARGE is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(AlignedMemoryChunk), unitCount, unitSize, pageSize);
		}
		
	}
	else {
		free(chunk);
	}

	rs->bytes += byteCount + sizeof(AlignedMemoryChunk);
	rs->pages += pageCount;
	rs->units += unitCount;
	rs->chunkCount++;
}

void TAlignedMemory__destruct__(TAlignedMemory* self, bool_t existed) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount =rs.unitSize = 0;
	usize_t i = 0;
	for (; i < 32; i++) {
		AlignedMemoryChunk* chunk = self->chunks[i];
		if (chunk) {
			TAlignedMemory__destructChunk(self,i,chunk,(AlignedMemoryReleaseInfo*const)&rs);
			
		}
	}
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		TAlignedMemory__destructChunk(self, i, chunk, (AlignedMemoryReleaseInfo* const)&rs);
		i++;
	}
	self->large = 0;
	if (self->logger) {
		TLogger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemory>[%p] released: { pages: %ld, bytes: %ld, units: %ld, chunks: %d }.", self, rs.pages,rs.bytes,rs.units,rs.chunkCount);
	}
	TMemory__destruct__((TMemory*)self, existed);
}
bool_t TAlignedMemory__collectChunkLinkGarbages(TAlignedMemory* self,AlignedMemoryReleaseInfo* rs, AlignedMemoryChunk* chunk,AlignedMemoryChunk* prevChunk, usize_t index,AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo info;
	usize_t pageCount = 0;
	AlignedMemoryPage* page = chunk->page;
	AlignedMemoryPage* next = 0;
	AlignedMemoryPage* prev = 0;
	while (page) {
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		usize_t c = 0;
		next = page->next;
		MLnkUnit* unit = page->free;

		while (unit) {
			c++; unit = unit->next;
		}
		if (c == chunk->pageCapacity) {

			if (prev)prev->next = next;
			else chunk->page = next;
			free(page);
			pageCount++;
		}
		else if (c > chunk->pageCapacity) {
			log_exit(ExitCode_critical, "AlignedMemory", "Critial error: the unit in Memory page is more than capacity(throw by AlignedMemory_collectGarbages).");
			return 0;
		}
		prev = page;
		page = next;

	}
	if (pageCount) {
		
		info.bytes = pageCount * chunk->pageSize;
		info.units = pageCount * chunk->pageCapacity;
		info.pages = pageCount;
		info.unitSize = chunk->unitSize;
		info.pageSize = chunk->pageSize;
		rs->bytes += info.bytes;
		rs->pages += pageCount;
		rs->units = info.units;
		rs->chunkCount++;
		if(self->logger)TLogger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] memory pages[unitsize=%d] released: { pages: %ld, bytes: %ld, units: %ld, unitSize: %ld, pageSize: %ld }.",index ,chunk,info.unitSize, pageCount, info.bytes, info.units, info.unitSize, info.pageSize);
		if (index >= 32 && !chunk->page) {
			if (prevChunk) prevChunk->nextChunk = chunk->nextChunk;
			else self->large = chunk->nextChunk;
			free(chunk);
			info.bytes += sizeof(AlignedMemoryChunk);
			rs->bytes += sizeof(AlignedMemoryChunk);
			if (self->logger)TLogger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] released self: { unitSize: %d,pageSize: %d }.", index, chunk, info.unitSize, info.pageSize);

			info.chunkCount = 1;
		}
		else info.chunkCount = 0;


		if (callback)callback(info);
		return info.chunkCount;
	}
	return 0;
}
bool_t TAlignedMemory__collectChunkRefGarbages(TAlignedMemory* self, AlignedMemoryReleaseInfo* rs, AlignedMemoryChunk* chunk, AlignedMemoryChunk* prevChunk, usize_t index, AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo info;
	usize_t pageCount = 0;
	AlignedMemoryPage* page = chunk->page;
	AlignedMemoryPage* next = 0;
	AlignedMemoryPage* prev = 0;
	usize_t chunkUnitSize = chunk->unitSize;
	while (page) {
		if (page->kind & MemoryKind_disCollect) { page = page->next; continue; }
		usize_t c = 0;
		next = page->next;
		MRefUnit* unit = (MRefUnit*)&page->free;
		bool_t hasAllocated = 0;
		for (usize_t i = 0, j = chunk->pageCapacity; i < j; i++) {
			if (unit->__ref__) { hasAllocated = 1; break; }
			unit = (MRefUnit*)((byte_t*)unit + chunkUnitSize);
		}
		if (!hasAllocated) {
			if (prev)prev->next = next;
			else chunk->page = next;
			free(page);
			pageCount++;
		}
		prev = page;
		page = next;

	}
	if (pageCount) {

		info.bytes = pageCount * chunk->pageSize;
		info.units = pageCount * chunk->pageCapacity;
		info.pages = pageCount;
		info.unitSize = chunk->unitSize;
		info.pageSize = chunk->pageSize;
		rs->bytes += info.bytes;
		rs->pages += pageCount;
		rs->units = info.units;
		rs->chunkCount++;
		if (self->logger)TLogger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] memory pages[unitsize=%d] released: { pages: %ld, bytes: %ld, units: %ld, unitSize: %ld, pageSize: %ld }.", index, chunk, info.unitSize, pageCount, info.bytes, info.units, info.unitSize, info.pageSize);
		if (index >= 32 && !chunk->page) {
			if (prevChunk) prevChunk->nextChunk = chunk->nextChunk;
			else self->large = chunk->nextChunk;
			free(chunk);
			info.bytes += sizeof(AlignedMemoryChunk);
			rs->bytes += sizeof(AlignedMemoryChunk);
			if (self->logger)TLogger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] released self: { unitSize: %d,pageSize: %d }.", index, chunk, info.unitSize, info.pageSize);

			info.chunkCount = 1;
		}
		else info.chunkCount = 0;


		if (callback)callback(info);
		return info.chunkCount;
	}
	return 0;
}
AlignedMemoryReleaseInfo TAlignedMemory_collectGarbages(TAlignedMemory* self,bool_t releasePage ,AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount = rs.unitSize = 0;
	
	AlignedMemoryChunk* chunk;
	usize_t i;
	for (i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			if(self->unitKind == MemoryUnitKind_link)TAlignedMemory__collectChunkLinkGarbages(self, &rs, chunk,0,i, callback);
			else if (self->unitKind == MemoryUnitKind_ref)TAlignedMemory__collectChunkRefGarbages(self, &rs, chunk, 0, i, callback);
		}
	}

	chunk = self->large;
	AlignedMemoryChunk* prevChunk=0;
	while (chunk) {
		AlignedMemoryChunk* nextChunk = chunk->nextChunk;
		bool_t chunkCollectRs = 0;
		if (self->unitKind == MemoryUnitKind_link)TAlignedMemory__collectChunkLinkGarbages(self, &rs, chunk, prevChunk, i, callback);
		else if (self->unitKind == MemoryUnitKind_ref)TAlignedMemory__collectChunkRefGarbages(self, &rs, chunk, prevChunk, i, callback);
		if (chunkCollectRs==0) {
			// chunk 没被移除
			prevChunk = chunk;
		}
		chunk = nextChunk;
		i++;
	}
	self->allocatedBytes -= rs.bytes;
	return rs;
}

TAlignedMemory* TAlignedMemory__construct__(TAlignedMemory* self, AlignedMemoryOptions* opts, TLogger* logger) {
	if (!self) {
		self = (TAlignedMemory*)TMemory_alloc(0,sizeof(TAlignedMemory),0,MemoryKind_disCollect);
		if (!self) {
			(TAlignedMemory*)log_exit(ExitCode_memory, "AlignedMemory.__construct__", "Cannot alloc memory:%ld", (long)sizeof(TAlignedMemory));
		}
		else {
			if (logger)  TLogger_trace(logger, "AlignedMemory.__construct__", "Memory is allocated for <AlignedMemory>[%p]:%d", self,sizeof(TAlignedMemory));
		}
		self->allocatedBytes = sizeof(TAlignedMemory);
	}
	self->__meta__ = (ClazzMeta*)&TAlignedMemory__meta__;
	if (opts) {
		self->gcBytes = opts->gcBytes;
		self->pageSize = opts->pageSize;
		self->unitKind = opts->unitKind;
	} else {
		self->pageSize = 1024 * 2;
		self->gcBytes = 0;
		self->unitKind = MemoryUnitKind_link;
	}
	if(self->unitKind!= MemoryUnitKind_link && self->unitKind!= MemoryUnitKind_ref){
		log_exit(ExitCode_argument,"AlignedMemory.__construct__","opts.unitSize should be MemoryUnitKind_link or MemoryUnitKind_ref");
	}
	if(self->pageSize<sizeof(AlignedMemoryPage) + sizeof(addr_t)){
		log_exit(ExitCode_argument,"AlignedMemory.__construct__","opts.pageSize should be greater than :%ld(sizeof(AlignedMemoryPage)) + %ld(sizeof(addr_t))",sizeof(AlignedMemoryPage),sizeof(addr_t));
	}
	self->logger = logger;

	for (size_t i = 0; i < 32; i++) self->chunks[i] = 0;
	self->large = 0;

	

	if (logger) TLogger_trace(logger, "AlignedMemory.__construct__", "<AlignedMemory> constructed.");
	return self;
}

MemoryAllocatingDirectives TAlignedMemory__allocating(TAlignedMemory* memory, usize_t size,MemoryKinds masks, AlignedMemoryChunk* chunk) {
	if (memory->totalBytes && memory->allocatedBytes + chunk->pageSize > memory->totalBytes) {
		AlignedMemoryReleaseInfo rs = ((TAlignedMemoryMeta*)memory->__meta__)->collectGarbages(memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookup;
		return MemoryAllocatingDirective_fail;
	}
	if (memory->gcBytes && memory->allocatedBytes + chunk->pageSize > memory->gcBytes) {
		AlignedMemoryReleaseInfo rs = ((TAlignedMemoryMeta*)memory->__meta__)->collectGarbages(memory, 1, 0);
		if (rs.bytes > chunk->pageSize) return MemoryAllocatingDirective_lookupOrNew;
		else return MemoryAllocatingDirective_new;
	}
	return MemoryAllocatingDirective_new;

}