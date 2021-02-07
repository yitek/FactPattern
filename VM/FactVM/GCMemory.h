#pragma once
#ifndef __GCMEMORY_INCLUDED__ 
#define __GCMEMORY_INCLUDED__

#include "Memory.h"
#include "Type.h"

#ifdef __cplusplus 
extern "C" {
#endif
	inline GCObject* GCObject_fromObject(void* obj) {
		return (GCObject*)((char*)obj - sizeof(GCObject));
	}
	inline void* GCObject_toObject(GCObject* gcObj) {
		return (void*)((char*)gcObj + sizeof(GCObject));
	}


	struct stGCMemory;
	struct stMemoryChunk;
	struct stLargeMemoryChunk;
	struct stMemoryPage;

	extern int GCMemory_printTrace;


	typedef struct stGCMemoryOptions {
		size_t pageSize;
		size_t pageCount;
		int enableGC;
	}GCMemoryOptions;

	typedef struct stGCMemory {
		struct stMemory;
		struct stGCMemoryOptions;
		// 2*16 = 32(16n), 4*16 = 64(8n),8*16 = 128(8n) >>32n;
		//2bytes,4bytes,6bytes,8bytes,10bytes,12bytes,14bytes,16bytes,20bytes,24bytes,28bytes,32bytes,36bytes,40bytes,44bytes,48bytes,52bytes,56bytes,60bytes,64bytes,
		struct stLargeMemoryChunk* large;
		struct stMemoryChunk* chunk4;
		struct stMemoryChunk* chunk6;
		struct stMemoryChunk* chunk8;
		struct stMemoryChunk* chunk10;
		struct stMemoryChunk* chunk12;
		struct stMemoryChunk* chunk14;
		struct stMemoryChunk* chunk16;
		struct stMemoryChunk* chunk18;
		struct stMemoryChunk* chunk20;
		struct stMemoryChunk* chunk24;
		struct stMemoryChunk* chunk28;
		struct stMemoryChunk* chunk32;
		struct stMemoryChunk* chunk36;
		struct stMemoryChunk* chunk40;
		struct stMemoryChunk* chunk48;
		struct stMemoryChunk* chunk56;
		struct stMemoryChunk* chunk64;
		struct stMemoryChunk* chunk72;
		struct stMemoryChunk* chunk80;
		struct stMemoryChunk* chunk96;
		struct stMemoryChunk* chunk112;
		struct stMemoryChunk* chunk128;
	} GCMemory;


	typedef struct stMemoryChunk {
		struct stGCMemory* allocator;
		size_t unitSize;
		size_t pageSize;
		size_t pageCapacity;
		struct stMemoryPage* headPage;
		struct stMemoryPage* tailPage;
	}MemoryChunk;

	typedef struct stLargeMemoryChunk {
		struct stMemoryChunk;

		struct stLargeMemoryChunk* nextChunk;
		struct stLargeMemoryChunk* prevChunk;
	}LargeMemoryChunk;

	typedef struct stMemoryPage {
		struct stMemoryPage* next;
		GCObject first;
	}MemoryPage;
	GCMemory* GCMemory_construct(GCMemory* self, const GCMemoryOptions* params);
	void* GCMemory_require(GCMemory* self, size_t size, Type* args);
	int GCMemory_release(GCMemory* self, void* p);

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif