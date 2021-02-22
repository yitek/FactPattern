
#include "List.h"
#define ALLOC(mm,size,arg,kind) m_alloc(mm,size,arg,kind)
#define ALLOC1(mm,size,arg,kind) m_alloc1(mm,size,arg,kind)
#define FREE(mm,p) m_free(mm,p)




List* List__construct__(List* self,TMemory* memory,void* listType,MemoryKinds mkind) {
	if (!memory)memory = TMemory_default;
	if (!self) self = ALLOC(memory, sizeof(List), listType,mkind);
	//self->itemSize = itemSize > 0 ? itemSize : sizeof(void*);
	self->head = self->tail = 0;
	self->length = 0;
	return self;
}

void List__destruct__(List* self,bool_t existed, TMemory* memory) {
	if (!memory)memory = TMemory_default;
	Link* node = self->head;
	while (node) {
		Link* next = node->next;
		FREE(memory,node);
		node = next;
	}
	if (!existed)  FREE(memory,self);
	//memory->release(memory, self);
}
void* List_append(List* self, usize_t itemSize,  TMemory* memory,void* itemType,MemoryKinds mkind) {
	if (!memory)memory = TMemory_default;
	Link* node = (Link*)ALLOC1(memory,itemSize + sizeof(Link), itemType,mkind);
	if (self->tail) self->tail = (self->tail->next = node);
	else self->head = self->tail = node;
	node->next = 0;
	self->length++;
	return node + 1;
}

 void* List_unshift(List* self, usize_t itemSize, TMemory* memory, void* itemType, MemoryKinds mkind) {
	if (!memory)memory = TMemory_default;
	Link* node = (Link*)ALLOC1(memory, itemSize + sizeof(Link), itemType, mkind);
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

 Link* List_popLink(List* self) {
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


bool_t List_pop(List* self, void* item, usize_t itemSize, TMemory* memory) {
	if (!self->tail) return 0;
	Link* link = List_popLink(self);
	if (item && itemSize) TMemory_copy(item, link + 1, itemSize);
	else return 0;
	if (!memory)memory = TMemory_default;
	FREE(memory, link);
	return 1;
	
}
word_t List_popValue(List* self, TMemory* memory) {
	if (!self->tail) return ~((word_t)0);
	Link* link = List_popLink(self);
	word_t rs = *((word_t*)(link + 1));
	if (!memory)memory = TMemory_default;
	FREE(memory, link);
	return rs;

}
bool_t List_shift(List* self, void* item, usize_t itemSize, TMemory* memory) {
	if (!self->head) return 0;
	Link* link = self->head;
	if (!(self->head = link->next)) self->tail = 0;
	if (item && itemSize) TMemory_copy(item, link + 1, itemSize);
	else return 0;
	if (!memory)memory = TMemory_default;
	FREE(memory, link);
	self->length--;
	return 1;

}
word_t List_shiftValue(List* self, TMemory* memory) {
	if (!self->head) return ~((word_t)0);
	Link* link = self->head;
	if (!(self->head = link->next)) self->tail = 0;
	word_t rs = *((word_t*)(link + 1));
	if (!memory)memory = TMemory_default;
	FREE(memory, link);
	self->length--;
	return rs;

}

bool_t List__internalRemove(List* self, LinkRemoveResult rrs, TMemory* memory) {
	if (rrs.link) {
		if (rrs.link == self->head) {
			self->head = rrs.link->next;
		}
		if (rrs.link == self->tail) {
			self->tail = rrs.prev;
		}
		if (!memory) memory = TMemory_default;
		FREE(memory, rrs.link);
		self->length--;
		return 1;
	}
	return 0;
}

Array* List_toArray(List* self, Array* target, const usize_t itemSize, TMemory* memory,void* arrType,MemoryKinds mkind) {
	if (!target) {
		target = (Array*)Array__construct__(0, 0, self->length, itemSize, memory,arrType,mkind);
	}
	char* item = (char*)self->head;
	char* dest = (char*)(target + 1);
	while (item) {
		TMemory_copy(dest, item + sizeof(Link), itemSize);
		item = (char*)(((Link*)item)->next);
		dest += itemSize;
	}
	return target;

}


#undef ALLOC
#undef FREE



