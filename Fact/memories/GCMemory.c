#include "GCMemory.h"




const markNumber = 1 << (sizeof(usize_t) - 1);
const unmarkNumber = !(1 << (sizeof(usize_t) - 1));


void GCMemory_markObject(ObjectLayout* gcObj,FindReferenceObject findReferenceObject) {
	// idle或已标记

	// 标记可达性
	gcObj->ref |= markNumber;
	size_t memberIndex = 0;
	ObjectLayout* member = findReferenceObject(gcObj,memberIndex);
	while (member) {
		GCMemory_markObject(member, findReferenceObject);
		member = findReferenceObject(gcObj, ++memberIndex);
	}
	
}

void GCMemory_markChunk(AlignedMemoryChunk* chunk) {
	//mark阶段
	AlignedMemoryPage* page = chunk->page;
	FindReferenceObject findReferenceObject = 0;
	while (page) {
		ObjectLayout* gcObj = (ObjectLayout*)((byte_t*)page + sizeof(AlignedMemoryPage));
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
		//if (log) log(chunk,c);
		chunk = chunk->nextChunk;
	}
	for (size_t i = 0; i < 32; i++) {
		chunk = self->chunks[i];
		if (chunk) {
			size_t c = GCMemory_sweepChunk(chunk);
			//if (log) log(chunk, c);
		}
	}

}