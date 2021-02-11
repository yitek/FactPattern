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

	List* List___construct__(List* self, void* mmArgs, Memory* memory);
	void List___destruct__(List* self, Memory* mallocator);
	void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* mallocator);
	inline void* List_searchByIndex(List* self, size_t index) {
		return self->head && index<self->length ? (void*)(Link_searchByIndex(self->head, index)+1) : 0;
	}
	inline void* List_searchByValue(List* self, word_t value) {
		return self->head ? (void*)(Link_searchByValue(self->head, value) + 1) : 0;
	}
	inline void* List_searchByItem(List* self, void* item,size_t itemSize) {
		return self->head ? (void*)(Link_searchByItem(self->head, item,itemSize) + 1) : 0;
	}
	inline void* List_searchByPredicate(List* self, LinkPredicate predicate, void* predicateArgs) {
		return self->head ? (void*)(Link_searchByPredicate(self->head, predicate, predicateArgs) + 1) : 0;
	}
	
	bool_t List_removeByIndex(List* self, size_t index,Memory* memory);
	bool_t List_removeByValue(List* self, word_t value, Memory* memory);
	bool_t List_removeByItem(List* self, void* item, size_t itemSize, Memory* memory);
	bool_t List_removeByPredicate(List* self, LinkPredicate predicate, void* predicateArgs, Memory* memory);

	inline void* List___INDEXGETER__(List* self, size_t index) {
		return self->head && self->length>index ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}

	Array* List_toArray(List* self, Array* target, const size_t itemSize, void* mmArgs, Memory* mallocator);
	inline int List_length(List* self) { return self->length; };

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
