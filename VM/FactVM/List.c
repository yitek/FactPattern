#include <string.h>
#include "GCMemory.h"
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

size_t Link_count(Link* link) {
	size_t index = 0;
	while (link) {
		link = link->next;
		index++;
	}
	return index;
}





List* List___construct__(List* self, void* mmArgs ,Memory* mallocator) {
	if (!mallocator)mallocator = Memory_default();
	if (!self) self = mallocator->require(mallocator, sizeof(List), mmArgs);
	//self->itemSize = itemSize > 0 ? itemSize : sizeof(void*);
	self->head = self->tail = 0;
	self->length = 0;
	return self;
}

void List___destruct__(List* self, Memory* mallocator) {
	if (!mallocator)mallocator = Memory_default();
	Link* node = self->head;
	while (node) {
		Link* next = node->next;
		mallocator->release(mallocator, node);
		node = next;
	}
}
void* List_append(List* self,size_t itemSize,void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	Link* node = (Link*)memory->require(memory, itemSize + sizeof(Link), mmArgs);
	if (self->tail) self->tail = (self->tail->next = node);
	else self->head = self->tail = node;
	node->next = 0;
	self->length++;
	return node+1;
}
void* List_search(List* self, LinkPredicate predicate, void* args) {
	size_t index = 0;
	Link* link = self->head;
	while (link) {
		if ((args && predicate && predicate((void*)(link + 1), index, args)) || (size_t)predicate == index) {
			return link +1;
		}
		link = link->next;
		index++;
	}
	return 0;
}
int List_remove(List* self, LinkPredicate predicate, void* param, Memory* memory) {
	Link* prev=0;
	size_t index = 0;
	Link* link = self->head;
	if (param && predicate) {
		size_t count = 0;
		while (link) {
			if (predicate((void*)(link + 1), index, param)) {
				if (prev) prev->next = link->next;
				else self->head = link->next;
				if (!memory)memory = Memory_default();
				memory->release(memory,link);
				self->length--;
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
				if (!memory)memory = Memory_default();
				memory->release(memory, link);
				self->length--;
				return 1;
			}
			link = (prev = link)->next;
			index++;
		}
		return 0;
	}
}

Array* List_toArray(List* self, Array* target,const size_t itemSize, void* mmArgs, Memory* memory) {
	if (!target) {
		target = (Array*)Array___construct__(0, 0, self->length, itemSize, mmArgs, memory);
	}
	char* item = (char*)self->head;
	char* dest = (char*)(target + 1);
	while (item) {
		memcpy(dest,item + sizeof(Link),itemSize);
		item = (char*)(((Link*)item)->next);
	}
	return target;

}



