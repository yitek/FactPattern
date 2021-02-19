#include "TAlignedMemory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


TAlignedMemoryVTBL alignedMemoryVTBL;


TAlignedMemoryChunk* TAlignedMemory__getNormalChunk(TAlignedMemory* self, size_t chunkIndex) {
	TAlignedMemoryChunk* chunk = self->chunks[chunkIndex];
	if (chunk) return chunk;
	size_t unitSize;
	if (chunkIndex < 16) {
		unitSize = (chunkIndex + 1) * sizeof(addr_t);
	}
	else if (chunkIndex < 24) {
		unitSize = 16 * sizeof(word_t) + (chunkIndex - 16 + 8) * sizeof(addr_t)*2;
	}
	else { //chunkIndex<32
		unitSize = 16 * sizeof(dword_t) + (chunkIndex - 24 + 8) * sizeof(addr_t)*4;
	}
	chunk = malloc(sizeof(TAlignedMemoryChunk));
	if (!chunk) {
		log_exit(1,"TAlignedMemory._getNormalChunk","Cannot alloc memory:%ld",(long)sizeof(TAlignedMemoryChunk));
		return 0;
	}
	else {
		self->allocatedBytes += sizeof(TAlignedMemoryChunk);
		self->chunks[chunkIndex] = chunk;
	}
	chunk->page = 0;
	chunk->memory = self;
	chunk->nextChunk = 0;
	chunk->pageSize = self->pageSize;
	chunk->unitSize = unitSize;
	chunk->pageCapacity = (self->pageSize - sizeof(TAlignedMemoryPage)) / unitSize;

	if (self->logger) {
		if (self->logger) TLogger_trace(self->logger, "TAlignedMemory._getLargeChunk", "<TAlignedMemoryChunk>[%p] for NORMAL is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld}"
			, chunk, chunk->unitSize, chunk->pageSize, chunk->pageCapacity, chunk->memory->allocatedBytes
		);
	}
	return chunk;
}

TAlignedMemoryChunk* TAlignedMemory__getLargeChunk(TAlignedMemory* self, size_t size) {
	//假设大内存不常用，用链表处理
	if (size % sizeof(addr_t)*4) size = (size / sizeof(addr_t)*4) + 1;
	TAlignedMemoryChunk* existed = self->large;
	TAlignedMemoryChunk* prev = 0;
	while (existed) {
		if (existed->unitSize == size) return existed;
		else if (existed->unitSize < size) {
			prev = existed;
			existed = existed->nextChunk;
		}
		else break;
	}
	size_t pageSize = self->pageSize;
	size_t pageUsableSize = (pageSize - sizeof(TAlignedMemoryPage));
	size_t pageCapacity = pageUsableSize / size;
	// 一页都无法装下一个
	if (pageCapacity == 0) {
		pageSize = size + sizeof(TAlignedMemoryPage);
		pageCapacity = 1;
	}
	// 一页只能装一个
	else if (pageCapacity < 2) {
		//剩余的太多
		if (pageUsableSize % size > pageUsableSize / 4) {
			pageSize = size + sizeof(TAlignedMemoryPage);
			pageCapacity = 1;
		}
	}
	TAlignedMemoryChunk* chunk = malloc(sizeof(TAlignedMemoryChunk));
	if (!chunk) {
		log_exit(1, "TAlignedMemory._getLargeChunk", "Cannot alloc memory:%ld", (long)sizeof(TAlignedMemoryChunk));
		return 0;
	}
	else {
		chunk->memory->allocatedBytes += sizeof(TAlignedMemoryChunk);
	}
	chunk->page = 0;
	chunk->memory = self;
	chunk->nextChunk = existed;
	chunk->pageSize = pageSize;
	chunk->unitSize = size;
	chunk->pageCapacity = pageCapacity;
	if (prev) prev->nextChunk = chunk;
	else self->large = chunk;

	if (self->logger) {
		if (self->logger) TLogger_trace(self->logger, "TAlignedMemory._getLargeChunk", "<TAlignedMemoryChunk>[%p] for LARGE is constructed:{ unitSize: %ld, pageSize: %ld, pageCapacity: $ld ,!allocatedBytes: %ld }"
			,chunk,(unsigned long)chunk->unitSize,(unsigned long)chunk->pageSize,(unsigned long) chunk->pageCapacity,chunk->memory->allocatedBytes
		);
	}

	return chunk;
}

void* TAlignedMemory__lookupUnit(TAlignedMemoryChunk* chunk, size_t unitSize) {
	TAlignedMemoryPage* page = chunk->page;
	while (page) {
		AlignedMemoryFreeUnit* unit = page->free;
		if (unit) {
			page->free = unit->next;
			return unit;
		}
		page = page->next;
	}
	return 0;
}

void* TAlignedMemory__initPageUnits(TAlignedMemoryChunk* chunk,TAlignedMemoryPage* page, size_t unitSize) {
	AlignedMemoryFreeUnit* unit = (AlignedMemoryFreeUnit*)(page +1);
	for (usize_t i = 0, j = chunk->pageCapacity - 1;i<j; i++) {
		unit->next = page->free;
		page->free = unit;
		unit = (AlignedMemoryFreeUnit*)(((byte_t*)unit) + unitSize);
	}
	return unit;
}

void* TAlignedMemory__chunkResolveUnit(TAlignedMemoryChunk* chunk, size_t unitSize) {
	byte_t* unit;
	if (chunk->memory->allocating) {
		AllocatePageDirectives directive = chunk->memory->allocating((TMemory*)chunk->memory,chunk->pageSize,chunk);
		if (directive == AllocatePageDirective_Fail) return 0;
		if (directive == AllocatePageDirective_Recheck || directive == AllocatePageDirective_RecheckOrNewPage) {
			unit = chunk->memory->lookupUnit(chunk,unitSize);
			if (unit)return unit;
		}
		if (directive == AllocatePageDirective_Recheck) return 0;
	}

	TAlignedMemoryPage* page = (TAlignedMemoryPage*)malloc(chunk->pageSize);
	if (!page) {
		log_exit(1, "TAlignedMemory._resolveUnit", "Cannot alloc memory:%ld", (long)chunk->pageSize);
		return 0;
	}
	else {
		chunk->memory->allocatedBytes += chunk->pageSize;
	}
	page->free = 0;
	unit = chunk->memory->initPage(chunk,page,unitSize);
	page->next = chunk->page;
	chunk->page = page;
	if (chunk->memory->logger) {
		TLogger_trace(chunk->memory->logger,"TAlignedMemory._resolveUnit","<TAlignedMemoryPage>[%p] was allocated and initialized:{ size: %ld, capacity: %ld ,!allocatedBytes: %ld }",page, chunk->pageSize,chunk->pageCapacity,chunk->memory->allocatedBytes);
	}
	return unit;
}

void* TAlignedMemory_alloc(TAlignedMemory* self, usize_t size) {
	size_t chunkIndex;
	TAlignedMemoryChunk* chunk;
	if (size <= 16 * sizeof(addr_t)) {// 16 个 1word 增加的 最大的是 16word, 0-15
		chunkIndex = size / sizeof(addr_t)-1;
		if (size % sizeof(addr_t)) chunkIndex++;
		chunk = TAlignedMemory__getNormalChunk(self, chunkIndex);
	}
	else if (size <= 16 * sizeof(addr_t)*2) {// 最大的 32word 15 - 31 128 
		chunkIndex = 16 - 8 + size / (sizeof(addr_t)*2);
		if (size % sizeof(addr_t)*2) chunkIndex++;
		chunk = TAlignedMemory__getNormalChunk(self, chunkIndex);
	}
	else if (size <= 16 * sizeof(addr_t)*4) {// 最大 64 word 256bytes 24-28
		chunkIndex = 24 - 8 + size / (sizeof(addr_t)*4);
		if (size % sizeof(addr_t)*4) chunkIndex++;
		chunk = TAlignedMemory__getNormalChunk(self, chunkIndex);
	}
	else {
		chunk = TAlignedMemory__getLargeChunk(self, size);
	}
	void* unit = self->lookupUnit(chunk, chunk->unitSize);
	if (unit) return unit;
	return TAlignedMemory__chunkResolveUnit(chunk,chunk->unitSize);
	
}

bool_t TAlignedMemory_free(TAlignedMemory* self, void* p) {
	for (usize_t i = 0; i < 32; i++) {
		TAlignedMemoryChunk* chunk = self->chunks[i];
		if(!chunk) continue;
		TAlignedMemoryPage* page = chunk->page;
		usize_t maxOffset = chunk->pageCapacity* chunk->unitSize;
		while (page) {
			if (p > (void*)page) {
				usize_t offset = ((usize_t)p - sizeof(TAlignedMemoryPage) - (usize_t)page);
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
	TAlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		TAlignedMemoryPage* page = chunk->page;
		while (page) {
			if (p > (void*)page) {
				usize_t at = ((usize_t)p - sizeof(TAlignedMemoryPage) - (usize_t)page);
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
void TAlignedMemory__destructChunk(TAlignedMemory* self,usize_t index, TAlignedMemoryChunk* chunk, AlignedMemoryReleaseInfo*const rs) {
	usize_t pageCount = 0;
	
	TAlignedMemoryPage* page = chunk->page;
	while (page) {
		pageCount++;
		TAlignedMemoryPage* next = page->next;
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
			TLogger_trace(self->logger, "TAlignedMemory.__destruct__", "<TAlignedMemoryChunk>[%d][%p] for NORMAL is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(TAlignedMemoryChunk), unitCount, unitSize, pageSize);
		}
		else {
			TLogger_trace(self->logger, "TAlignedMemory.__destruct__", "<TAlignedMemoryChunk>[%d][%p] for LARGE is released: { pages: %ld, bytes: %ld + %d, units: %ld , unitSize: %ld, pageSize: %ld }.", index, chunk, pageCount, byteCount, sizeof(TAlignedMemoryChunk), unitCount, unitSize, pageSize);
		}
		
	}
	else {
		free(chunk);
	}

	rs->bytes += byteCount + sizeof(TAlignedMemoryChunk);
	rs->pages += pageCount;
	rs->units += unitCount;
	rs->chunkCount++;
}

void TAlignedMemory__destruct__(TAlignedMemory* self, bool_t existed) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount =rs.unitSize = 0;
	usize_t i = 0;
	for (; i < 32; i++) {
		TAlignedMemoryChunk* chunk = self->chunks[i];
		if (chunk) {
			TAlignedMemory__destructChunk(self,i,chunk,(AlignedMemoryReleaseInfo*const)&rs);
			
		}
	}
	TAlignedMemoryChunk* chunk = self->large;
	while (chunk) {
		TAlignedMemory__destructChunk(self, i, chunk, (AlignedMemoryReleaseInfo* const)&rs);
		i++;
	}
	self->large = 0;
	if (self->logger) {
		TLogger_trace(self->logger, "TAlignedMemory.__destruct__", "<TAlignedMemory>[%p] released: { pages: %ld, bytes: %ld, units: %ld, chunks: %d }.", self, rs.pages,rs.bytes,rs.units,rs.chunkCount);
	}
	TMemory__destruct__((TMemory*)self, existed);
}
bool_t TAlignedMemory__collectChunkGarbages(TAlignedMemory* self,AlignedMemoryReleaseInfo* rs, TAlignedMemoryChunk* chunk,TAlignedMemoryChunk* prevChunk, usize_t index,AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo info;
	usize_t pageCount = 0;
	TAlignedMemoryPage* page = chunk->page;
	TAlignedMemoryPage* next = 0;
	TAlignedMemoryPage* prev = 0;
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
			log_exit(2, "TAlignedMemory", "Critial error: the unit in Memory page is more than capacity(throw by TAlignedMemory_collectGarbages).");
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
		if(self->logger)TLogger_trace(self->logger, "TAlignedMemory.collectGarbages", "<TAlignedMemoryChunk>[%d][%p] memory pages[unitsize=%d] released: { pages: %ld, bytes: %ld, units: %ld, unitSize: %ld, pageSize: %ld }.",index ,chunk,info.unitSize, pageCount, info.bytes, info.units, info.unitSize, info.pageSize);
		if (index >= 32 && !chunk->page) {
			if (prevChunk) prevChunk->nextChunk = chunk->nextChunk;
			else self->large = chunk->nextChunk;
			free(chunk);
			info.bytes += sizeof(TAlignedMemoryChunk);
			rs->bytes += sizeof(TAlignedMemoryChunk);
			if (self->logger)TLogger_trace(self->logger, "TAlignedMemory.collectGarbages", "<TAlignedMemoryChunk>[%d][%p] released self: { unitSize: %d,pageSize: %d }.", index, chunk, info.unitSize, info.pageSize);

			info.chunkCount = 1;
		}
		else info.chunkCount = 0;


		if (callback)callback(info);
		return info.chunkCount;
	}
	return 0;
}
AlignedMemoryReleaseInfo TAlignedMemory_collectGarbages(TAlignedMemory* self, AlignedMemoryGCCallback callback) {
	AlignedMemoryReleaseInfo rs;
	rs.bytes = rs.pages = rs.pageSize = rs.units = rs.chunkCount = rs.unitSize = 0;
	
	TAlignedMemoryChunk* chunk;
	usize_t i;
	for (i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			TAlignedMemory__collectChunkGarbages(self, &rs, chunk,0,i, callback);
		}
	}

	chunk = self->large;
	TAlignedMemoryChunk* prevChunk=0;
	while (chunk) {
		TAlignedMemoryChunk* nextChunk = chunk->nextChunk;
		if (!TAlignedMemory__collectChunkGarbages(self, &rs, chunk, prevChunk, 0, callback)) {
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
		self = (TAlignedMemory*)malloc(sizeof(TAlignedMemory));
		if (!self) {
			return (TAlignedMemory*)log_exit(1, "TAlignedMemory.__construct__", "Cannot alloc memory:%ld", (long)sizeof(TAlignedMemory));
		}
		else {
			if (logger)  TLogger_trace(logger, "TAlignedTMemory.__construct__", "Memory is allocated for <TAlignedMemory>[%p]:%d", self,sizeof(TAlignedMemory));
		}
		self->allocatedBytes = sizeof(TAlignedMemory);
	}
	TMemory__construct__((TMemory*)self, (MemoryOptions*)opts, logger);
	self->vftptr = (vftptr_t)&alignedMemoryVTBL;
	if (opts) {
		m_copy(&self->initPage,&opts->initPage,sizeof(struct stAlignedMemoryOpts));
	} else {
		self->pageSize = 1024 * 2;
		self->totalBytes = 0;
		self->totalBytes = 0;
	}
	if (!self->initPage) self->initPage = &TAlignedMemory__initPageUnits;
	if (!self->lookupUnit) self->lookupUnit = &TAlignedMemory__lookupUnit;
	self->logger = logger;

	for (size_t i = 0; i < 32; i++) self->chunks[i] = 0;
	self->large = 0;
	if (logger) TLogger_trace(logger, "TAlignedTMemory.__construct__", "<TAlignedTMemory> constructed.");
	return self;
}