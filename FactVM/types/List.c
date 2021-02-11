
#include "List.h"
#include <string.h>




List* List___construct__(List* self, void* mmArgs, Memory* mallocator) {
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
		mallocator->weekRelease(mallocator, node);
		node = next;
	}
}
void* List_append(List* self, size_t itemSize, void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	Link* node = (Link*)memory->require(memory, itemSize + sizeof(Link), mmArgs);
	if (self->tail) self->tail = (self->tail->next = node);
	else self->head = self->tail = node;
	node->next = 0;
	self->length++;
	return node + 1;
}

bool_t List_remove(List* self, LinkRemoveResult rrs,Memory* memory) {
	if (rrs.link) {
		if (rrs.link == self->head) {
			self->head = rrs.link->next;
		}
		if (rrs.link == self->tail) {
			self->tail = rrs.prev;
		}
		if (!memory) memory = Memory_default();
		memory->weekRelease(memory, rrs.link);
		self->length--;
		return 1;
	}
	return 0;
}

bool_t List_removeByIndex(List* self, size_t index, Memory* memory) {
	if (self->head) {
		return List_remove(self, Link_removeByIndex((Link*)self,index),memory);
	}
	return 0;
}
bool_t List_removeByValue(List* self, word_t value, Memory* memory) {
	if (self->head) {
		return List_remove(self, Link_removeByIndex((Link*)self, value), memory);
	}
	return 0;
}

bool_t List_removeByItem(List* self, void* item,size_t itemSize, Memory* memory) {
	if (self->head) {
		return List_remove(self, Link_removeByItem((Link*)self, item,itemSize), memory);
	}
	return 0;
}

bool_t List_removeByPredicate(List* self, LinkPredicate predicate, void* searchArgs, Memory* memory) {
	if (self->head) {
		return List_remove(self, Link_removeByPredicate((Link*)self, predicate, searchArgs), memory);
	}
	return 0;
}

Array* List_toArray(List* self, Array* target, const size_t itemSize, void* mmArgs, Memory* memory) {
	if (!target) {
		target = (Array*)Array___construct__(0, 0, self->length, itemSize, mmArgs, memory);
	}
	char* item = (char*)self->head;
	char* dest = (char*)(target + 1);
	while (item) {
		memcpy(dest, item + sizeof(Link), itemSize);
		item = (char*)(((Link*)item)->next);
	}
	return target;

}



