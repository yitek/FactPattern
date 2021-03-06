/*****************************************************
*
* author:yiy
*
* description: 链表节点基本操作
*
******************************************************/

#pragma once
#include "../def.h"
#ifndef __LINK_INCLUDED__ 
#define __LINK_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif
	///
	typedef struct stLink {
		struct stLink* next;
	}Link;

	typedef struct stLinkRemoveResult {
		Link* prev;
		Link* link;
	}LinkRemoveResult;

	typedef struct stLinkSearchResult {
		size_t index;
		void* item;
	}LinkSearchResult;

	typedef bool_t (*LinkPredicate)(void* item, size_t index, void* args);

	size_t Link_length(Link* link);

	inline Link* Link_last(Link* link) {
		Link* result = 0;
		while (link) { link = (result = link)->next; }
		return result;
	}
	inline Link* Link_append(Link* head, Link* item, Link* tail) {
		item->next = 0;
		if (!head) return 0;
		if (!tail) tail = Link_last(head);
		tail->next = item;
		return 0;
	}

	inline Link* Link_searchByIndex(Link* link, size_t index) {
		size_t c = 0;
		while (link) {
			if (c == index) return link;
			link = (link)->next;
			c++;
		}
		return 0;
	}
	LinkSearchResult Link_searchByValue(Link* link, word_t value);
	LinkSearchResult Link_searchByItem(Link* link, void* compareItem, size_t itemSize);
	LinkSearchResult Link_searchByPredicate(Link* link, LinkPredicate predicate, void* pParam);


	LinkRemoveResult Link_removeByIndex(Link* link, size_t index);
	LinkRemoveResult Link_removeByValue(Link* link, word_t value);
	LinkRemoveResult Link_removeByItem(Link* link, void* compareItem, size_t itemSize);
	LinkRemoveResult Link_removeByPredicate(Link* link, LinkPredicate predicate, void* predicateArgs);
	inline void* Link___INDEXGETER__(Link* link, size_t index) {
		return (void*)(Link_searchByIndex(link,index) +1);
	}
#ifdef __cplusplus 
}//extern "C" 
#endif

#endif // __LINK_INCLUDED__
