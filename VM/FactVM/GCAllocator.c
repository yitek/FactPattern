#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GCAllocator.h"
#include "Type.h"



void GCAllocator_collectGarbages(GCAllocator* self, AlignChunk* chunk);
/// <summary>
/// ªÿ ’¿¨ª¯
/// </summary>
/// <param name="self"></param>
void GCAllocator_markChunkGarbages(AlignChunk* chunk);

void GCAllocator_markGCObject(GCObject* gcObj);



GCAllocator* GCAllocator_construct(GCAllocator* self, GCAllocatorOptions* params) {
	GCAllocator* allocator = (GCAllocator*)AlignAllocator_construct((AlignAllocator*)self, (AlignAllocatorOptions*)params);
	//allocator->onNoIdleUnit = (OnNoIdleUnitCallback)GCAllocator_collectGarbages;
	return allocator;
}

