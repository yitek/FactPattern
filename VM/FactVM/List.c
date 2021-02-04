#include "List.h"
#include "MAllocator.h"
List* List_construct(List* self, size_t itemSize) {
	MAllocator* mallocator = MAllocator_default();
	
	if (!self) self = mallocator->allocate(sizeof(List));
	self->itemSize = itemSize>0?itemSize:sizeof(void*);
	self->head = self->tail = 0;
	self->count = 0;
	
	return self;
}

void List_destruct(List* self) {
	
	struct TListNode* node = self->head;
	while (node) {
		struct  TListNode* next = node->next;
		MAllocator_release(MAllocator_default(),(node));
		node = next;
	}
}

struct TListNode* List_insert(List* self) {
	struct TListNode* node = (struct TListNode*)MAllocator_allocate(MAllocator_default(),sizeof(struct TListNode) + self->itemSize);
	node->next = 0;
	if (self->tail) {
		self->tail->next = node;
		self->tail = node;
	}
	else {
		self->head = self->tail = node;
	}
	
	self->count++;
	return node;
}

void* List_add(List* self) {
	struct TListNode* node = List_insert(self);
	return &node->value;
}

int List_remove(List* self, void* predicate, void* args) {
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
				MAllocator_release(MAllocator_default(), removed);
				self->count--;
			}
			else {
				prev = node;
				node = node->next;
			}

		}
	}
	else {
		size_t c = 0;
		while (node) {
			if (c==(size_t)predicate) {
				removed = node;
				if (prev) {
					node = prev->next = node->next;
				}
				else {
					node = self->head = node->next;
				}
				MAllocator_release(MAllocator_default(), removed);
				self->count--;
				return 1;
			}
			else {
				prev = node;
				node = node->next;
			}
			c++;
		}
	}
	self->tail = prev;
	return 0;
}

struct TListNode* List_search(List* self, void* predicate, void* args) {
	struct TListNode* node = self->head;
	if (args != 0) {
		while (node) {
			if (((Lookup)predicate)(&node->value,args)) return node;
			node = node->next;
		}
	}
	else {
		size_t c = 0;
		while (node) {
			if (c == (size_t)predicate) return node;
			node = node->next;
			c++;
		}
	}
	return 0;
}

void* List_find(List* self, void* predicate, void* args) {
	struct TListNode* node = List_search(self, predicate, args);
	if (node) return &node->value;
	return 0;
}

int List_count(List* self) {
	struct TListNode* node = self->head;
	int c = 0;
	while (node) {
		c++;
		node = node->next;
	}
	return c;
}





