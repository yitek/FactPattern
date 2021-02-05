#pragma once
#include "MAllocator.h"
#include "Type.h"

typedef struct TAlignObject {
	size_t ref;
	struct TType* type;
} AlignObject;

inline AlignObject* GCObject_fromObject(void* obj) {
	return (AlignObject*)((char*)obj - sizeof(AlignObject));
}
inline void* GCObject_toObject(AlignObject* gcObj) {
	return (void*)((char*)gcObj + sizeof(AlignObject));
}


struct TAlignAllocator;
struct TAlignChunk;
struct TAlignLargeChunk;
struct TAlignPage;

extern int AlignAllocator_printTrace;


typedef struct TAlignAllocatorOptions {
	size_t pageSize;
	size_t pageCount;
	int enableGC;
}AlignAllocatorOptions;

typedef struct TAlignAllocator {
	struct TMAllocator;
	struct TAlignAllocatorOptions;
	// 2*16 = 32(16n), 4*16 = 64(8n),8*16 = 128(8n) >>32n;
	//2bytes,4bytes,6bytes,8bytes,10bytes,12bytes,14bytes,16bytes,20bytes,24bytes,28bytes,32bytes,36bytes,40bytes,44bytes,48bytes,52bytes,56bytes,60bytes,64bytes,
	struct TAlignLargeChunk* large;
	struct TAlignChunk* chunk4;
	struct TAlignChunk* chunk6;
	struct TAlignChunk* chunk8;
	struct TAlignChunk* chunk10;
	struct TAlignChunk* chunk12;
	struct TAlignChunk* chunk14;
	struct TAlignChunk* chunk16;
	struct TAlignChunk* chunk18;
	struct TAlignChunk* chunk20;
	struct TAlignChunk* chunk24;
	struct TAlignChunk* chunk28;
	struct TAlignChunk* chunk32;
	struct TAlignChunk* chunk36;
	struct TAlignChunk* chunk40;
	struct TAlignChunk* chunk48;
	struct TAlignChunk* chunk56;
	struct TAlignChunk* chunk64;
	struct TAlignChunk* chunk72;
	struct TAlignChunk* chunk80;
	struct TAlignChunk* chunk96;
	struct TAlignChunk* chunk112;
	struct TAlignChunk* chunk128;
} AlignAllocator;


typedef struct TAlignChunk {
	struct TAlignAllocator* allocator;
	size_t unitSize;
	size_t pageSize;
	size_t pageCapacity;
	struct TAlignPage* headPage;
	struct TAlignPage* tailPage;
}AlignChunk;

typedef struct TAlignLargeChunk {
	struct TAlignChunk;
	
	struct TAlignLargeChunk* nextChunk;
	struct TAlignLargeChunk* prevChunk;
}AlignLargeChunk;

typedef struct TAlignPage {
	struct TAlignPage* next;
	AlignObject first;
}AlignPage;
AlignAllocator* AlignAllocator_construct(AlignAllocator* self, const AlignAllocatorOptions* params);
void* AlignAllocator_require(AlignAllocator* self, size_t size);
int AlignAllocator_release(AlignAllocator* self, void* p);

