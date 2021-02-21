/*****************************************************
*
* author:yiy
*
* description: ×Ö´®Àà

*
******************************************************/

#pragma once
#include "TObject.h"
#include "../ads/List.h"
#ifndef __TLIST_INCLUDED__ 
#define __TLIST_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	inline List* TList___construct__(List* self) {
		return List___construct__(self, 0,0);
	}
	inline void TList___destruct__(List* self, Memory* memory) {
		const Type* type = TObject_getType(self);
		List___destruct__(self,type->memory);
	}
	inline void* TList_append(List* self) {
		Type* itemType = TObject_getIndexType(self,0);
		return itemType?List_append(self,itemType->size,itemType,itemType->memory):0;
	}
	inline void* TList_push(List* self) { return TList_append(self); }
	inline void* TList_unshift(List* self) {
		Type* itemType = TObject_getIndexType(self, 0);
		return itemType?List_unshift(self,itemType->size,itemType,itemType->memory):0;
	}
	
	inline LinkSearchResult TList_searchByItem(List* self, void* item) {
		Type* itemType = TObject_getIndexType(self, 0);
		if (itemType&&self->head) return Link_searchByItem(self->head, item, itemType->size);
		LinkSearchResult rs;
		rs.index = -1; rs.item = 0;
		return rs;
	}	

	inline bool_t TList_removeByIndex(List* self, size_t index) {
		return List_removeByIndex(self, index, TObject_getMemory(self));
	}
	inline bool_t TList_removeByValue(List* self, word_t value) {
		return List_removeByValue(self, value, TObject_getMemory(self));
	}

	inline bool_t TList_removeByItem(List* self, void* item) {
		Type* itemType = TObject_getIndexType(self,0);
		return  itemType?List_removeByItem(self, item, itemType->size, itemType->memory):0;
	}

	inline bool_t TList_removeByPredicate(List* self, LinkPredicate predicate, void* searchArgs) {
		return  List_removeByPredicate(self, predicate, searchArgs, TObject_getMemory(self));
	}
	inline bool_t TList_pop(List* self, void* item, size_t itemSize) {
		Type* itemType = TObject_getIndexType(self, 0);
		return itemType?List_pop(self,  item, itemType->size, itemType->memory):0;
	}
	inline bool_t TList_shift(List* self, void* item) {
		Type* itemType = TObject_getIndexType(self, 0);
		return itemType ? List_shift(self, item, itemType->size, itemType->memory) : 0;
	}
	inline word_t TList_popValue(List* self) {
		return List_popValue(self,TObject_getMemory(self));
	}
	inline word_t TList_shiftValue(List* self) {
		return List_shiftValue(self,TObject_getMemory(self));
	}


	inline void* List___INDEXGETER__(List* self, size_t index) {
		return self->head && self->length > index ? (void*)(Link_searchByIndex(self->head, index) + 1) : 0;
	}
	inline Array* TList_toArray(List* self) {
		Type* itemType = TObject_getIndexType(self, 0);
		return itemType?List_toArray(self, 0, itemType->size, itemType, itemType->memory):0;
	}
	inline size_t TList_length(List* self) { return self?self->length:0; };



#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TLIST_INCLUDED__



