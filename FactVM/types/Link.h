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

	typedef bool_t (*LinkPredicate)(void* item, size_t index, void* args);

	size_t Link_length(Link* link);

	inline Link* Link_last(Link* link);
	Link* Link_append(Link* head, Link* item, Link* tail);

	Link* Link_searchByIndex(Link* link, size_t index);
	Link* Link_searchByValue(Link* link, word_t value);
	Link* Link_searchByItem(Link* link, void* compareItem, size_t itemSize);
	Link* Link_searchByPredicate(Link* link, LinkPredicate predicate, void* pParam);


	Link* Link_removeByIndex(Link* link, size_t index);
	Link* Link_removeByValue(Link* link, word_t value);
	Link* Link_removeByItem(Link* link, void* compareItem, size_t itemSize);
	Link* Link_removeByPredicate(Link* link, LinkPredicate predicate, void* pParam);
	inline void* Link___INDEXGETER__(Link* link, size_t index) {
		return (void*)(Link_searchByIndex(link,index) +1);
	}
#ifdef __cplusplus 
}//extern "C" 
#endif

#endif // __LINK_INCLUDED__
