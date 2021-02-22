
/*****************************************************
*
* author:yiy
*
* description: 链表类
*
******************************************************/
#pragma once

#include "link.h"
#include "Array.h"
#ifndef __LIST_INCLUDED__ 
#define __LIST_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif
	

	typedef struct stList {
		usize_t length;
		Link* head;
		Link* tail;

	} List;

	List* List__construct__(List* self, TMemory* memory, void* itemType, MemoryKinds mkind);
	void List__destruct__(List* self, bool_t existed, TMemory* memory);
	void* List_append(List* self, usize_t itemSize, TMemory* memory, void* itemType, MemoryKinds mkind);
	inline static void* List_push(List* self, usize_t itemSize, TMemory* memory, void* itemType, MemoryKinds mkind) { return List_append(self,itemSize,memory,itemType,mkind); }
	void* List_unshift(List* self, usize_t itemSize, TMemory* memory, void* itemType, MemoryKinds mkind);
	inline static void* List_searchByIndex(List* self, usize_t index) {
		return self->head && index<self->length ? (void*)(Link_searchByIndex(self->head, index)+1) : 0;
	}
	inline static LinkSearchResult List_searchByValue(List* self, word_t value) {
		if (self && self->head) return Link_searchByValue(self->head, value);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs; 
	}
	inline static LinkSearchResult List_searchByItem(List* self, void* item,usize_t itemSize) {
		if (self && self->head) return Link_searchByItem(self->head, item,itemSize);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs;
	}
	inline static LinkSearchResult List_searchByPredicate(List* self, LinkPredicate predicate, void* predicateArgs) {
		LinkSearchResult rs;
		if (!self->head) { rs.index = -1; rs.item = 0; }
		else {
			rs = Link_searchByPredicate(self->head, predicate, predicateArgs);
			if (rs.item) rs.item = (void*)(((Link*)rs.item) + 1);
		}
		return rs;
	}
	

	bool_t List__internalRemove(List* self, LinkRemoveResult rrs, TMemory* memory);

	inline static bool_t List_removeByIndex(List* self, usize_t index, TMemory* memory) {
		return (self && self->head) ? List__internalRemove(self, Link_removeByIndex((Link*)self, index), memory) : 0;
	}
	inline static bool_t List_removeByValue(List* self, word_t value, TMemory* memory) {
		return(self && self->head) ? List__internalRemove(self, Link_removeByIndex((Link*)self, value), memory) : 0;
	}

	inline static bool_t List_removeByItem(List* self, void* item, usize_t itemSize, TMemory* memory) {
		return (self && self->head) ? List__internalRemove(self, Link_removeByItem((Link*)self, item, itemSize), memory) : 0;
	}

	inline static bool_t List_removeByPredicate(List* self, LinkPredicate predicate, void* searchArgs, TMemory* memory) {
		return (self && self->head) ? List__internalRemove(self, Link_removeByPredicate((Link*)self, predicate, searchArgs), memory) : 0;
	}
	bool_t List_pop(List* self,void* item,usize_t itemSize, TMemory* memory);
	bool_t List_shift(List* self, void* item, usize_t itemSize, TMemory* memory);
	word_t List_popValue(List* self, TMemory* memory);
	word_t List_shiftValue(List* self, TMemory* memory);
	

	static inline void* List__get__(List* self, usize_t index) {
		return self && self->head && self->length>index ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}


	static inline bool_t List__getValue__(List* self, usize_t index, word_t value) {
		void* p = List__get__(self, index);
		if (p) {
			*((word_t*)p) = value;
			return 1;
		}
		return 0;
		}

	static inline bool_t  List__set__(List* self, usize_t index, void* item, usize_t itemSize) {
		if (!item || !itemSize) return 0;
		void* p = List__get__(self, index);
		if (p) {
			TMemory_copy(p, item, itemSize);
			return 1;
		}return 0;

	}

	static inline bool_t  List__setValue__(List* self, usize_t index, word_t value) {
		void* p = List__get__(self, index);
		if (p) {
			*((word_t*)p) = value;
			return 1;
		}
		return 0;
	}
	Array* List_toArray(List* self, Array* target, const usize_t itemSize ,TMemory* memory, void* arrType, MemoryKinds mkind);
	inline static usize_t List_length(List* self) { return self?self->length:0; };

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
