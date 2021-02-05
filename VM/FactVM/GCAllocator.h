#pragma once
#include "Type.h"
typedef struct TGCObject {
	/// <summary>
	/// 引用计数
	/// </summary>
	size_t ref;
	struct TAny;
} GCObject;
typedef struct TGCLargeObject {
	size_t size;
	struct TGCLargeObject* next;
	struct TGCObject;
} GCLargeObject;

inline GCObject* GCObject_fromObject(void* obj) {
	return (GCObject*)((char*)obj - sizeof(struct TGCObject));
}
inline void* GCObject_toObject(GCObject* gcObj) {
	return (void*)((char*)gcObj + sizeof(struct TGCObject));
}




struct TGCChunk;
struct TGCPage;

typedef struct TGCAllocator {
	size_t pageSize;
	// 2*16 = 32(16n), 4*16 = 64(8n),8*16 = 128(8n) >>32n;
	//2bytes,4bytes,6bytes,8bytes,10bytes,12bytes,14bytes,16bytes,20bytes,24bytes,28bytes,32bytes,36bytes,40bytes,44bytes,48bytes,52bytes,56bytes,60bytes,64bytes,
	struct GCLargeObject* large;
	struct TGCChunk* chunk4;
	struct TGCChunk* chunk6;
	struct TGCChunk* chunk8;
	struct TGCChunk* chunk10;
	struct TGCChunk* chunk12;
	struct TGCChunk* chunk14;
	struct TGCChunk* chunk16;
	struct TGCChunk* chunk18;
	struct TGCChunk* chunk20;
	struct TGCChunk* chunk24;
	struct TGCChunk* chunk28;
	struct TGCChunk* chunk32;
	struct TGCChunk* chunk36;
	struct TGCChunk* chunk40;
	struct TGCChunk* chunk48;
	struct TGCChunk* chunk56;
	struct TGCChunk* chunk64;
	struct TGCChunk* chunk72;
	struct TGCChunk* chunk80;
	struct TGCChunk* chunk96;
	struct TGCChunk* chunk112;
	struct TGCChunk* chunk128;
} GCAllocator;

typedef struct TGCChunk {
	struct TGCAllocator* mallocator;
	size_t unitSize;
	size_t pageCapacity;
	struct TGCPage* head;
	struct TGCPage* tail;
}GCChunk;

typedef struct TGCPage {
	struct TGCPage* next;
	GCObject first;
}GCPage;