#pragma once

typedef int bool;

struct TListNode {
	struct TListNode* next;
	void* value;
	
};


typedef struct TList {
	struct TListNode* head;
	struct TListNode* tail;
	int itemSize;
	void* (*allocator)(int size);
	void (*releaser)(void* p);
} List;

typedef int (*Lookup)(void* item, void* args);

List* list_construct(List* self,int itemSize, void* (*allocator)(unsigned int size), void (*releaser)(void* p));

void list_destruct(List* self);
struct TListNode* list_insert(List* self);
void* list_add(List* self);
struct TListNode* list_search(List* self,void* predicate, void* args);
void* list_find(List* self, void* predicate, void* args);
int list_remove(List* self, void* predicate, void* args);
int list_count(List* self);
void* list_index(List* self, int at);

