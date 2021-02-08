#pragma once
#ifndef __LIST_INCLUDED__ 
#define __LIST_INCLUDED__
#include "Array.h"

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stLink {
		struct stLink* next;
	}Link;

	typedef int (*LinkPredicate)(void* link, size_t index, void* args);

	size_t Link_count(Link* link);

	inline Link* Link_last(Link* link);
	Link* Link_append(Link* head, Link* item, Link* tail);
	Link* Link_remove(Link* link, LinkPredicate predicate, void* args);
	Link* Link_search(Link* link, LinkPredicate predicate, void* args);

	typedef struct stList {
		size_t length;
		Link* head;
		Link* tail;
		
	} List;

	List* List___construct__(List* self, void* mmArgs, Memory* memory);
	void List___destruct__(List* self, Memory* mallocator);
	void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* mallocator);
	void* List_search(List* self, LinkPredicate predicate, void* param);
	int List_remove(List* self, LinkPredicate predicate, void* param, Memory* memory);
	Array* toArray(List* self, Array* target, size_t itemSize);
	inline int List_length(List* self) { return self->length; }

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
