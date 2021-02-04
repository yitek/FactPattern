#pragma once
#include <stddef.h>

struct TListNode {
	struct TListNode* next;
	void* value;
	
};


typedef struct TList {
	struct TListNode* head;
	struct TListNode* tail;
	size_t count;
	size_t itemSize;
} List;

typedef int (*Lookup)(void* item, void* args);

List* List_construct(List* self, size_t itemSize);

void List_destruct(List* self);
struct TListNode* List_insert(List* self);
void* List_add(List* self);
struct TListNode* List_search(List* self,void* predicate, void* args);
void* List_find(List* self, void* predicate, void* args);
int List_remove(List* self, void* predicate, void* args);
int List_count(List* self);

