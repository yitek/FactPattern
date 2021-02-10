#pragma once
#include "Type.h"
#include "AlignAllocator.h"
typedef struct TGCObject {
	struct TAlignObject;
} GCObject;






typedef struct TGCAllocatorOptions {
	AlignAllocator;
} GCAllocatorOptions;


typedef struct TGCAllocator {
	struct TAlignAllocator;
} GCAllocator;

inline void* GCAllocator_require(GCAllocator* self, size_t size) {
	return AlignAllocator_require((AlignAllocator*)self, size,0);
}
int GCAllocator_release(AlignAllocator* self, void* p) {
	return AlignAllocator_release((AlignAllocator*)self, p);
}

