
#include "TList.h"
#define _ALLOC_(size,arg,kind,mm) m_alloc(size,arg,kind,mm)
#define _ALLOC1_(size,arg,kind,mm) m_alloc1(size,arg,kind,mm)
#define _FREE_(p,mm) m_free(p,mm)




TList* TList__construct__(TList* self, TMemory* memory, void* listType, MemoryKinds mkind) {
	if (!memory)memory = TMemory_default;
	if (!self) self = _ALLOC_(sizeof(List), listType, mkind, memory);
	//self->itemSize = itemSize > 0 ? itemSize : sizeof(void*);
	self->head = self->tail = 0;
	self->length = 0;
	return self;
}

void TList__destruct__(TList* self, bool_t existed, TMemory* memory) {
	if (!memory)memory = TMemory_default;
	Link* node = self->head;
	while (node) {
		Link* next = node->next;
		_FREE_(node, memory);
		node = next;
	}
	if (!existed)  _FREE_(self, memory);
	//memory->release(memory, self);
}
void* TList_append(TList* self ,TMemory* memory, MemoryKinds mkind) {
	struct stTType* itemType = get_genericArgument((struct stTObject*)self, 0);

	Link* node = (Link*)_ALLOC1_(itemType->size + sizeof(Link), itemType, mkind, memory);
	if (self->tail) self->tail = (self->tail->next = node);
	else self->head = self->tail = node;
	node->next = 0;
	self->length++;
	return node + 1;
}

void* TList_unshift(TList* self,  TMemory* memory, MemoryKinds mkind) {
	if (!memory)memory = TMemory_default;
	struct stTType* itemType = get_genericArgument((struct stTObject*)self, 0);

	Link* node = (Link*)_ALLOC1_(itemType->size + sizeof(Link), itemType, mkind, memory);
	if (self->head) {
		node->next = self->head;
		self->head = node;
	}
	else {
		self->head = self->tail = node;
		node->next = 0;
	}
	self->length++;
	return (void*)(node + 1);
}

Link* TList_popLink(TList* self) {
	Link* node;
	if (self->tail == self->head) {
		node = self->head;
		self->head = self->tail = 0;
		self->length--;
	}
	else if (self->head->next == self->tail) {
		node = self->tail;
		self->tail = self->head;
		self->length--;
	}
	else {
		LinkRemoveResult rrs = Link_removeByIndex(self->head, self->length - 1);
		node = rrs.link;
		self->tail = rrs.prev;
		self->length--;
	}
	return node;
}


bool_t TList_pop(TList* self, void* item, TMemory* memory) {
	if (!self->tail) return 0;
	struct stTType* itemType = get_genericArgument((struct stTObject*)self, 0);

	Link* link = List_popLink(self);
	if (item && itemType->size) TMemory_copy(item, link + 1, itemType->size);
	else return 0;
	if (!memory)memory = TMemory_default;
	_FREE_(link, memory);
	return 1;

}
word_t TList_popValue(TList* self, TMemory* memory) {
	if (!self->tail) return ~((word_t)0);
	Link* link = TList_popLink(self);
	word_t rs = *((word_t*)(link + 1));
	if (!memory)memory = TMemory_default;
	_FREE_(link, memory);
	return rs;

}
bool_t TList_shift(TList* self, void* item,  TMemory* memory) {
	if (!self->head) return 0;
	Link* link = self->head;
	if (!(self->head = link->next)) self->tail = 0;
	usize_t itemSize = get_genericArgument((struct stTObject*)self, 0)->size;

	if (item && itemSize) TMemory_copy(item, link + 1, itemSize);
	else return 0;
	if (!memory)memory = TMemory_default;
	_FREE_(link, memory);
	self->length--;
	return 1;

}
word_t TList_shiftValue(TList* self, TMemory* memory) {
	if (!self->head) return ~((word_t)0);
	Link* link = self->head;
	if (!(self->head = link->next)) self->tail = 0;
	word_t rs = *((word_t*)(link + 1));
	if (!memory)memory = TMemory_default;
	_FREE_(link, memory);
	self->length--;
	return rs;

}

bool_t TList__internalRemove(TList* self, LinkRemoveResult rrs, TMemory* memory) {
	if (rrs.link) {
		if (rrs.link == self->head) {
			self->head = rrs.link->next;
		}
		if (rrs.link == self->tail) {
			self->tail = rrs.prev;
		}
		if (!memory) memory = TMemory_default;
		_FREE_(rrs.link, memory);
		self->length--;
		return 1;
	}
	return 0;
}

TArray* TList_toArray(List* self,TMemory* memory, MemoryKinds mkind) {
	usize_t itemSize = get_genericArgument((struct stTObject*)self, 0);
	TArray* target = 0;
	 TArray__construct__(target, 0, self->length, memory, 0, mkind);
	char* item = (char*)self->head;
	char* dest = (char*)(target + 1);
	
	while (item) {
		TMemory_copy(dest, item + sizeof(Link), itemSize);
		item = (char*)(((Link*)item)->next);
		dest += itemSize;
	}
	return target;

}


#undef _ALLOC_
#undef _ALLOC1_
#undef _FREE_



