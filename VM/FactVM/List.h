#pragma once
#ifndef __LIST_INCLUDED__ 
#define __LIST_INCLUDED__
#include "Memory.h"


typedef struct st_Link {
	struct st_Link* next;
}Link;

typedef int (*LinkPredicate)(void* link, size_t index,void* args);

size_t Link_count(Link* link);

inline Link* Link_last(Link* link);
Link* Link_append(Link* head, Link* item, Link* tail);
Link* Link_remove(Link* link, LinkPredicate predicate, void* args);
Link* Link_search(Link* link, LinkPredicate predicate, void* args);

typedef struct {
	Link* head;
	Link* tail;
	size_t count;
	size_t itemSize;
} List;

List* List_construct(List* self, size_t itemSize, Memory* mallocator);
void List_destruct(List* self, Memory* mallocator);
void* List_append(List* self, Memory* mallocator);
void* List_search(List* self, LinkPredicate predicate, void* args);
int List_remove(List* self, LinkPredicate predicate, void* args, Memory* mallocator);
int List_count(List* self);


#endif
