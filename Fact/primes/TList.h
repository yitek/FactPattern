
/*****************************************************
*
* author:yiy
*
* description: 链表类
*
******************************************************/
#pragma once

#include "List.h"
#include "TArray.h"
#ifndef __TLIST_INCLUDED__ 
#define __TLIST_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif


	typedef struct { struct stTList; } TList;

	TList* TList__construct__(TList* self, TMemory* memory, void* itemType, MemoryKinds mkind);
	void TList__destruct__(TList* self, bool_t existed, TMemory* memory);
	void* TList_append(TList* self,  TMemory* memory,  MemoryKinds mkind);
	inline static void* TList_push(TList* self, TMemory* memory, MemoryKinds mkind) { return TList_append(self,  memory,  mkind); }
	void* TList_unshift(TList* self,TMemory* memory,  MemoryKinds mkind);
	inline static void* TList_searchByIndex(struct stTList* self, usize_t index) {
		return self->head && index < self->length ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}
	inline static LinkSearchResult TList_searchByValue(TList* self, word_t value) {
		if (self && self->head) return Link_searchByValue(self->head, value);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs;
	}
	inline static LinkSearchResult TList_searchByItem(TList* self, void* item, usize_t itemSize) {
		if (self && self->head) return Link_searchByItem(self->head, item, itemSize);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs;
	}
	inline static LinkSearchResult TList_searchByPredicate(TList* self, LinkPredicate predicate, void* predicateArgs) {
		LinkSearchResult rs;
		if (!self->head) { rs.index = -1; rs.item = 0; }
		else {
			rs = Link_searchByPredicate(self->head, predicate, predicateArgs);
			if (rs.item) rs.item = (void*)(((Link*)rs.item) + 1);
		}
		return rs;
	}


	bool_t TList__internalRemove(TList* self, LinkRemoveResult rrs, TMemory* memory);

	inline static bool_t TList_removeByIndex(TList* self, usize_t index, TMemory* memory) {
		return (self && self->head) ? TList__internalRemove(self, Link_removeByIndex((Link*)self, index), memory) : 0;
	}
	inline static bool_t TList_removeByValue(TList* self, word_t value, TMemory* memory) {
		return(self && self->head) ? TList__internalRemove(self, Link_removeByIndex((Link*)self, value), memory) : 0;
	}

	inline static bool_t TList_removeByItem(TList* self, void* item, usize_t itemSize, TMemory* memory) {
		return (self && self->head) ? TList__internalRemove(self, Link_removeByItem((Link*)self, item, itemSize), memory) : 0;
	}

	inline static bool_t TList_removeByPredicate(TList* self, LinkPredicate predicate, void* searchArgs, TMemory* memory) {
		return (self && self->head) ? TList__internalRemove(self, Link_removeByPredicate((Link*)self, predicate, searchArgs), memory) : 0;
	}
	bool_t TList_pop(TList* self, void* item,  TMemory* memory);
	bool_t TList_shift(TList* self, void* item,  TMemory* memory);
	word_t TList_popValue(TList* self, TMemory* memory);
	word_t TList_shiftValue(TList* self, TMemory* memory);


	static inline void* TList__get__(TList* self, usize_t index) {
		return self && self->head && self->length > index ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}


	static inline bool_t TList__getValue__(TList* self, usize_t index, word_t value) {
		void* p = TList__get__(self, index);
		if (p) {
			*((word_t*)p) = value;
			return 1;
		}
		return 0;
	}

	static inline bool_t  TList__set__(TList* self, usize_t index, void* item, usize_t itemSize) {
		if (!item || !itemSize) return 0;
		void* p = TList__get__(self, index);
		if (p) {
			TMemory_copy(p, item, itemSize);
			return 1;
		}return 0;

	}

	static inline bool_t  TList__setValue__(TList* self, usize_t index, word_t value) {
		void* p = TList__get__(self, index);
		if (p) {
			*((word_t*)p) = value;
			return 1;
		}
		return 0;
	}
	TArray* TList_toArray(TList* self,  TMemory* memory, MemoryKinds mkind);
	inline static usize_t TList_length(TList* self) { return self ? self->length : 0; };

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif // __TLIST_INCLUDED__
