#pragma once
#ifndef __GCMEMORY_INCLUDED__ 
#define __GCMEMORY_INCLUDED__

#include "Memory.h"
#include "Type.h"




inline GCObject* GCObject_fromObject(void* obj) {
	return (GCObject*)((char*)obj - sizeof(GCObject));
}
inline void* GCObject_toObject(GCObject* gcObj) {
	return (void*)((char*)gcObj + sizeof(GCObject));
}


struct TGCMemory;
struct TMemoryChunk;
struct TLargeMemoryChunk;
struct TMemoryPage;

extern int GCMemory_printTrace;


typedef struct TGCMemoryOptions {
	size_t pageSize;
	size_t pageCount;
	int enableGC;
}GCMemoryOptions;

typedef struct TGCMemory {
	struct TMemory;
	struct TGCMemoryOptions;
	// 2*16 = 32(16n), 4*16 = 64(8n),8*16 = 128(8n) >>32n;
	//2bytes,4bytes,6bytes,8bytes,10bytes,12bytes,14bytes,16bytes,20bytes,24bytes,28bytes,32bytes,36bytes,40bytes,44bytes,48bytes,52bytes,56bytes,60bytes,64bytes,
	struct TLargeMemoryChunk* large;
	struct TMemoryChunk* chunk4;
	struct TMemoryChunk* chunk6;
	struct TMemoryChunk* chunk8;
	struct TMemoryChunk* chunk10;
	struct TMemoryChunk* chunk12;
	struct TMemoryChunk* chunk14;
	struct TMemoryChunk* chunk16;
	struct TMemoryChunk* chunk18;
	struct TMemoryChunk* chunk20;
	struct TMemoryChunk* chunk24;
	struct TMemoryChunk* chunk28;
	struct TMemoryChunk* chunk32;
	struct TMemoryChunk* chunk36;
	struct TMemoryChunk* chunk40;
	struct TMemoryChunk* chunk48;
	struct TMemoryChunk* chunk56;
	struct TMemoryChunk* chunk64;
	struct TMemoryChunk* chunk72;
	struct TMemoryChunk* chunk80;
	struct TMemoryChunk* chunk96;
	struct TMemoryChunk* chunk112;
	struct TMemoryChunk* chunk128;
} GCMemory;


typedef struct TMemoryChunk {
	struct TGCMemory* allocator;
	size_t unitSize;
	size_t pageSize;
	size_t pageCapacity;
	struct TMemoryPage* headPage;
	struct TMemoryPage* tailPage;
}MemoryChunk;

typedef struct TLargeMemoryChunk {
	struct TMemoryChunk;

	struct TLargeMemoryChunk* nextChunk;
	struct TLargeMemoryChunk* prevChunk;
}LargeMemoryChunk;

typedef struct TMemoryPage {
	struct TMemoryPage* next;
	GCObject first;
}MemoryPage;
GCMemory* GCMemory_construct(GCMemory* self, const GCMemoryOptions* params);
void* GCMemory_require(GCMemory* self, size_t size, Type* args);
int GCMemory_release(GCMemory* self, void* p);
#endif