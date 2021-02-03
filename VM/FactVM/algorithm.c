#include "algorithm.h"
#include <malloc.h>
List* list_construct(List* self, int itemSize, void* (*allocator)(unsigned int size), void (*releaser)(void* p)) {
	if (!allocator) allocator = malloc;
	if (!self) self = allocator(sizeof(List));
	self->allocator = allocator;
	self->releaser = releaser ? releaser : 0;
	self->itemSize = itemSize>0?itemSize:sizeof(void*);
	self->head = self->tail = 0;
	return self;
}

void list_destruct(List* self) {
	
	struct TListNode* node = self->head;
	while (node) {
		struct  TListNode* next = node->next;
		self->releaser(node);
		node = next;
	}
}

struct TListNode* list_insert(List* self) {
	struct TListNode* node = (struct TListNode*)self->allocator(sizeof(struct TListNode) + self->itemSize);
	if (self->tail) {
		self->tail->next = node;
		self->tail = node;
	}
	else {
		self->head = self->tail = node;
	}
	return node;
}

void* list_add(List* self) {
	struct TListNode* node = list_insert(self);
	return &node->value;
}

int list_remove(List* self, void* predicate, void* args) {
	struct TListNode* node = self->head;
	struct TListNode* prev = 0;
	struct TListNode* removed = 0;
	int c = 0;
	if (args != 0) {
		while (node) {
			if (((Lookup)predicate)(&node->value, args)) {
				removed = node;
				c++;
				if (prev) {
					node = prev->next = node->next;
				}
				else {
					node = self->head = node->next;
				}
				self->releaser(removed);
				continue;
			}
			else {
				prev = node;
				node = node->next;
			}
		}
	}
	else {
		while (node) {
			if (node->value==predicate) {
				removed = node;
				c++;
				if (prev) {
					node = prev->next = node->next;
				}
				else {
					node = self->head = node->next;
				}
				self->releaser(removed);
				continue;
			}
			else {
				prev = node;
				node = node->next;
			}
		}
	}
	self->tail = prev;
	return 0;
}

struct TListNode* list_search(List* self, void* predicate, void* args) {
	struct TListNode* node = self->head;
	if (args != 0) {
		while (node) {
			if (((Lookup)predicate)(&node->value,args)) return node;
			node = node->next;
		}
	}
	else {
		while (node) {
			if (node->value == predicate) return node;
			node = node->next;
		}
	}
	return 0;
}

void* list_find(List* self, void* predicate, void* args) {
	struct TListNode* node = list_search(self, predicate, predicate);
	if (node) return &node->value;
	return 0;
}

int list_count(List* self) {
	struct TListNode* node = self->head;
	int c = 0;
	while (node) {
		c++;
		node = node->next;
	}
	return c;
}


void* list_index(List* self, int at) {
	struct TListNode* node = self->head;
	int c = 0;
	while (node) {
		if (c == at) return &node->value;
		c++;
		node = node->next;
	}
	return c;
}


