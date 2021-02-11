#pragma once

#include "Array.h"
#include "link.h"
#ifndef __LIST_INCLUDED__ 
#define __LIST_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif
	

	typedef struct stList {
		size_t length;
		Link* head;
		Link* tail;

	} List;

	/*List* List___construct__(List* self, void* mmArgs, Memory* memory);
	void List___destruct__(List* self, Memory* mallocator);
	void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* mallocator);
	void* List_search(List* self, LinkPredicate predicate, void* param);
	int List_remove(List* self, LinkPredicate predicate, void* param, Memory* memory);
	Array* List_toArray(List* self, Array* target, const size_t itemSize, void* mmArgs, Memory* mallocator);
	inline int List_length(List* self) { return self->length; }*/

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
