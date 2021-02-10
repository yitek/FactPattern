#pragma once
#include "List.h"
#ifndef __MAP_INCLUDED__ 
#define __MAP_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stMap {
		struct stList;

	} Map;

	Map* Map___construct__(Map* self,void* mmArgs, Memory* memory);
	//void List___destruct__(List* self, Memory* mallocator);
	//void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* mallocator);
	//void* List_search(List* self, LinkPredicate predicate, void* param);
	//int List_remove(List* self, LinkPredicate predicate, void* param, Memory* memory);
	//Array* List_toArray(List* self, Array* target, const size_t itemSize, void* mmArgs, Memory* mallocator);
	//inline int List_length(List* self) { return self->length; }

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
