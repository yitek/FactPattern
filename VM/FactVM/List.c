#include "List.h"


inline Link* Link_last(Link* link) {
	Link* result = 0;
	while (link) { link = (result = link)->next; }
	return result;
}
Link* Link_append(Link* head, Link* item, Link* tail) {
	item->next = 0;
	if (!head) return 0;
	if (!tail) tail = Link_last(head);
	tail->next = item;
	return 0;
}
Link* Link_remove(Link* link, LinkPredicate predicate, void* args) {
	Link* prev = 0;
	size_t index = 0;
	while (link) {
		if ((args && predicate(link, index, args)) || (size_t)predicate == index) {
			if (prev) prev->next = link->next;
			return link;
		}
		link = (prev = link)->next;
		index++;
	}
	return 0;
}

Link* Link_search(Link* link, LinkPredicate predicate, void* args) {
	size_t index = 0;
	while (link) {
		if ((args && predicate(link, index, args)) || (size_t)predicate == index) {
			return link;
		}
		link = link->next;
		index++;
	}
	return 0;
}


List* List_construct(List* self, size_t itemSize, Memory* mallocator) {
	if (!mallocator)mallocator = Memory_default();
	if (!self) self = mallocator->require(mallocator, sizeof(List), 0);
	self->itemSize = itemSize > 0 ? itemSize : sizeof(void*);
	self->head = self->tail = 0;
	self->count = 0;
	return self;
}

void List_destruct(List* self, Memory* mallocator) {
	if (!mallocator)mallocator = Memory_default();
	Link* node = self->head;
	while (node) {
		Link* next = node->next;
		mallocator->release(mallocator, node);
		node = next;
	}
}
void* List_append(List* self, Memory* mallocator) {
	if (!mallocator)mallocator = Memory_default();
	Link* node = (Link*)mallocator->require(mallocator, self->itemSize + sizeof(Link), 0);
	if (self->tail) self->tail = (self->tail->next = node);
	else self->head = self->tail = node;
	node->next = 0;
	self->count++;
	return node + 1;
}
void* List_search(List* self, LinkPredicate predicate, void* args) {
	size_t index = 0;
	Link* link = self->head;
	while (link) {
		if ((args && predicate && predicate((void*)(link + 1), index, args)) || (size_t)predicate == index) {
			return link + 1;
		}
		link = link->next;
		index++;
	}
	return 0;
}
int List_remove(List* self, LinkPredicate predicate, void* args, Memory* mallocator) {
	Link* prev = 0;
	size_t index = 0;
	Link* link = self->head;
	if (args && predicate) {
		size_t count = 0;
		while (link) {
			if (predicate((void*)(link + 1), index, args)) {
				if (prev) prev->next = link->next;
				else self->head = link->next;
				if (!mallocator)mallocator = Memory_default();
				mallocator->release(mallocator, link);
				self->count--;
				count++;
			}
			link = (prev = link)->next;
			index++;
		}
		return count;
	}
	else {
		while (link) {
			if ((size_t)predicate == index) {
				if (prev) prev->next = link->next;
				else self->head = link->next;
				if (!mallocator)mallocator = Memory_default();
				mallocator->release(mallocator, link);
				self->count--;
				return 1;
			}
			link = (prev = link)->next;
			index++;
		}
		return 0;
	}
}
inline int List_count(List* self) {
	return Link_count(self->head);
}