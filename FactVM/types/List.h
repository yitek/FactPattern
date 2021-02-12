
/*****************************************************
*
* author:yiy
*
* description: Á´±íÀà
*
******************************************************/
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
	void List___destruct__(List* self, Memory* memory);
	void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* memory);
	inline void* List_push(List* self, size_t itemSize, void* mmArgs, Memory* memory) { return List_append(self,itemSize,mmArgs,memory); }
	void* List_unshift(List* self, size_t itemSize, void* mmArgs, Memory* memory);
	inline void* List_searchByIndex(List* self, size_t index) {
		return self->head && index<self->length ? (void*)(Link_searchByIndex(self->head, index)+1) : 0;
	}
	inline LinkSearchResult List_searchByValue(List* self, word_t value) {
		if (self->head) return Link_searchByValue(self->head, value);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs; 
	}
	inline LinkSearchResult List_searchByItem(List* self, void* item,size_t itemSize) {
		if (self->head) return Link_searchByItem(self->head, item,itemSize);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs;
	}
	inline LinkSearchResult List_searchByPredicate(List* self, LinkPredicate predicate, void* predicateArgs) {
		LinkSearchResult rs;
		if (!self->head) { rs.index = -1; rs.item = 0; }
		else {
			rs = Link_searchByPredicate(self->head, predicate, predicateArgs);
			if (rs.item) rs.item = (void*)(((Link*)rs.item) + 1);
		}
		return rs;
	}
	

	inline bool_t List_internalRemove(List* self, LinkRemoveResult rrs, Memory* memory) {
		if (rrs.link) {
			if (rrs.link == self->head) {
				self->head = rrs.link->next;
			}
			if (rrs.link == self->tail) {
				self->tail = rrs.prev;
			}
			if (!memory) memory = Memory_default();
			memory->decrease(rrs.link);
			self->length--;
			return 1;
		}
		return 0;
	}

	inline bool_t List_removeByIndex(List* self, size_t index, Memory* memory) {
		return (self->head) ? List_internalRemove(self, Link_removeByIndex((Link*)self, index), memory) : 0;
	}
	inline bool_t List_removeByValue(List* self, word_t value, Memory* memory) {
		return(self->head) ? List_internalRemove(self, Link_removeByIndex((Link*)self, value), memory) : 0;
	}

	inline bool_t List_removeByItem(List* self, void* item, size_t itemSize, Memory* memory) {
		return (self->head) ? List_internalRemove(self, Link_removeByItem((Link*)self, item, itemSize), memory) : 0;
	}

	inline bool_t List_removeByPredicate(List* self, LinkPredicate predicate, void* searchArgs, Memory* memory) {
		return (self->head) ? List_internalRemove(self, Link_removeByPredicate((Link*)self, predicate, searchArgs), memory) : 0;
	}
	bool_t List_pop(List* self,void* item,size_t itemSize, Memory* memory);
	bool_t List_shift(List* self, void* item, size_t itemSize, Memory* memory);
	word_t List_popValue(List* self, Memory* memory);
	word_t List_shiftValue(List* self, Memory* memory);
	

	inline void* List___INDEXGETER__(List* self, size_t index) {
		return self->head && self->length>index ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}

	bool_t List___INDEXSETTER__(List* self, size_t index, void* item, size_t itemSize);
	bool_t List___INDEX_value__(List* self, size_t index, word_t value);
	Array* List_toArray(List* self, Array* target, const size_t itemSize, void* mmArgs, Memory* mallocator);
	inline int List_length(List* self) { return self->length; };

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
