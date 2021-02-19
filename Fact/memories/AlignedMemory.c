#include "AlignedMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


AlignedMemoryMETA alignedMemoryMETA;







void* AlignedMemory__initPageUnits(AlignedMemoryChunk* chunk,AlignedMemoryPage* page, size_t unitSize) {
	AlignedMemoryFreeUnit* unit = (AlignedMemoryFreeUnit*)(page +1);
	for (usize_t i = 0, j = chunk->pageCapacity - 1;i<j; i++) {
		unit->next = page->free;
		page->free = unit;
		unit = (AlignedMemoryFreeUnit*)(((byte_t*)unit) + unitSize);
	}
	return unit;
}

void* AlignedMemory__chunkResolveUnit(AlignedMemoryChunk* chunk, size_t unitSize) {
	void* unit=0;
	MemoryAllocating allocating = ((MemoryMETA*)chunk->memory->__meta__)->allocating;
	if (allocating) {
		AllocatePageDirectives directive = allocating((Memory*)chunk->memory,chunk->pageSize,chunk);
		if (directive == AllocatePageDirective_Fail) return 0;
		if (directive == AllocatePageDirective_Recheck || directive == AllocatePageDirective_RecheckOrNewPage) {
#if defined(AlignedMemoryLookupUnit)
			AlignedMemoryLookupUnit(unit, chunk, unitSize)
#endif
			if (unit)return unit;
		}
		
		if (directive == AllocatePageDirective_Recheck) return 0;
	}

	AlignedMemoryPage* page = (AlignedMemoryPage*)malloc(chunk->pageSize);
	if (!page) {
		log_exit(1, "AlignedMemory._resolveUnit", "Cannot alloc memory:%ld", (long)chunk->pageSize);
		return 0;
	}
	else {
		chunk->memory->allocatedBytes += chunk->pageSize;
	}
	page->free = 0;
	unit = chunk->memory->opts.initPage(chunk,page,unitSize);
	page->next = chunk->page;
	chunk->page = page;
	if (chunk->memory->logger) {
		Logger_trace(chunk->memory->logger,"AlignedMemory._resolveUnit","<AlignedMemoryPage>[%p] was allocated and initialized:{ size: %ld, capacity: %ld ,!allocatedBytes: %ld }",page, chunk->pageSize,chunk->pageCapacity,chunk->memory->allocatedBytes);
	}
	return unit;
}


bool_t AlignedMemory_free(AlignedMemory* self, void* p) {
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
				AlignedMemoryFreeUnit* unit = (AlignedMemoryFreeUnit*)p;
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
				AlignedMemoryFreeUnit* unit = (AlignedMemoryFreeUnit*)p;
				unit->next = page->free;
				page->free = unit;
				return 1;
			}

		}
		chunk = chunk->nextChunk;
	}
	return 0;
}
void AlignedMemory__destructChunk(AlignedMemory* self,usize_t index, AlignedMemoryChunk* chunk, AlignedMemoryReleaseInfo*const rs) {
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
			Logger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemoryChunk>[%d][%p] for NORMAL is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(AlignedMemoryChunk), unitCount, unitSize, pageSize);
		}
		else {
			Logger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemoryChunk>[%d][%p] for LARGE is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(AlignedMemoryChunk), unitCount, unitSize, pageSize);
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

void AlignedMemory__destruct__(AlignedMemory* self, bool_t existed) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount =rs.unitSize = 0;
	usize_t i = 0;
	for (; i < 32; i++) {
		AlignedMemoryChunk* chunk = self->chunks[i];
		if (chunk) {
			AlignedMemory__destructChunk(self,i,chunk,(AlignedMemoryReleaseInfo*const)&rs);
			
		}
	}
	AlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		AlignedMemory__destructChunk(self, i, chunk, (AlignedMemoryReleaseInfo* const)&rs);
		i++;
	}
	self->large = 0;
	if (self->logger) {
		Logger_trace(self->logger, "AlignedMemory.__destruct__", "<AlignedMemory>[%p] released: { pages: %ld, bytes: %ld, units: %ld, chunks: %d }.", self, rs.pages,rs.bytes,rs.units,rs.chunkCount);
	}
	Memory__destruct__((Memory*)self, existed);
}
bool_t AlignedMemory__collectChunkGarbages(AlignedMemory* self,AlignedMemoryReleaseInfo* rs, AlignedMemoryChunk* chunk,AlignedMemoryChunk* prevChunk, usize_t index,AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo info;
	usize_t pageCount = 0;
	AlignedMemoryPage* page = chunk->page;
	AlignedMemoryPage* next = 0;
	AlignedMemoryPage* prev = 0;
	while (page) {
		usize_t c = 0;
		next = page->next;
		AlignedMemoryFreeUnit* unit = page->free;

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
			log_exit(2, "AlignedMemory", "Critial error: the unit in Memory page is more than capacity(throw by AlignedMemory_collectGarbages).");
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
		if(self->logger)Logger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] memory pages[unitsize=%d] released: { pages: %ld, bytes: %ld, units: %ld, unitSize: %ld, pageSize: %ld }.",index ,chunk,info.unitSize, pageCount, info.bytes, info.units, info.unitSize, info.pageSize);
		if (index >= 32 && !chunk->page) {
			if (prevChunk) prevChunk->nextChunk = chunk->nextChunk;
			else self->large = chunk->nextChunk;
			free(chunk);
			info.bytes += sizeof(AlignedMemoryChunk);
			rs->bytes += sizeof(AlignedMemoryChunk);
			if (self->logger)Logger_trace(self->logger, "AlignedMemory.collectGarbages", "<AlignedMemoryChunk>[%d][%p] released self: { unitSize: %d,pageSize: %d }.", index, chunk, info.unitSize, info.pageSize);

			info.chunkCount = 1;
		}
		else info.chunkCount = 0;


		if (callback)callback(info);
		return info.chunkCount;
	}
	return 0;
}
AlignedMemoryReleaseInfo AlignedMemory_collectGarbages(AlignedMemory* self,bool_t releasePage ,AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount = rs.unitSize = 0;
	
	AlignedMemoryChunk* chunk;
	usize_t i;
	for (i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			AlignedMemory__collectChunkGarbages(self, &rs, chunk,0,i, callback);
		}
	}

	chunk = self->large;
	AlignedMemoryChunk* prevChunk=0;
	while (chunk) {
		AlignedMemoryChunk* nextChunk = chunk->nextChunk;
		if (!AlignedMemory__collectChunkGarbages(self, &rs, chunk, prevChunk, 0, callback)) {
			prevChunk = chunk;
		}
		chunk = nextChunk;
		i++;
	}
	self->allocatedBytes -= rs.bytes;
	return rs;
}

AlignedMemory* AlignedMemory__construct__(AlignedMemory* self, AlignedMemoryOptions* opts, Logger* logger) {
	if (!self) {
		self = (AlignedMemory*)malloc(sizeof(AlignedMemory));
		if (!self) {
			return (AlignedMemory*)log_exit(1, "AlignedMemory.__construct__", "Cannot alloc memory:%ld", (long)sizeof(AlignedMemory));
		}
		else {
			if (logger)  Logger_trace(logger, "AlignedMemory.__construct__", "Memory is allocated for <AlignedMemory>[%p]:%d", self,sizeof(AlignedMemory));
		}
		self->allocatedBytes = sizeof(AlignedMemory);
	}
	Memory__construct__((Memory*)self, (MemoryOptions*)opts, logger);
	self->__meta__ = (ObjectMetaLayout*)&alignedMemoryMETA;
	if (opts) {
		m_copy(&self->opts.initPage,&opts->initPage,sizeof(struct stAlignedMemoryOpts));
	} else {
		self->opts.pageSize = 1024 * 2;
		self->totalBytes = 0;
		self->allocatedBytes = 0;
	}
	if (!self->opts.initPage) self->opts.initPage = &AlignedMemory__initPageUnits;
	//if (!self->lookupUnit) self->lookupUnit = &AlignedMemory__lookupUnit;
	self->logger = logger;

	for (size_t i = 0; i < 32; i++) self->chunks[i] = 0;
	self->large = 0;
	if (logger) Logger_trace(logger, "AlignedMemory.__construct__", "<AlignedMemory> constructed.");
	return self;
}