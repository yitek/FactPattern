#pragma once
#include "Set.h"
#include "List.h"

Set* Set___construct__(Set* self, void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	if (!self) self = memory->require(memory, sizeof(Set), mmArgs);
	//self->itemSize = itemSize > 0 ? itemSize : sizeof(void*);
	self->headOrRoot = self->tail = 0;
	self->length = 0;
	return self;
}

void* Set_put(Set* self, void* key, size_t keySize, void* value, size_t valueSize, void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	byte_t* item = List_append((List*)self, keySize + valueSize, mmArgs, memory);
	Memory_copy(item,key,keySize);
	Memory_copy(item + keySize,value,valueSize);
	return item;
}


void* Set_putValue(Set* self, void* key, size_t keySize, word_t value, void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	byte_t* item = List_append((List*)self, sizeof(word_t) * 2, mmArgs, memory);
	Memory_copy(item, key, keySize);
	*((word_t*)(item + keySize)) = value;
	return item;
}

void* Set_putByValue(Set* self,  word_t key, void* value, size_t valueSize, void* mmArgs, Memory* memory) {
	if (!memory)memory = Memory_default();
	byte_t* item = List_append((List*)self, sizeof(word_t) * 2, mmArgs, memory);
	*((word_t*)item) = key;
	Memory_copy(item + sizeof(word_t), value, valueSize);
	return item;
}
