#pragma once
#ifndef __STRING_INCLUDED__ 
#define __STRING_INCLUDED__
#include "Memory.h"

typedef struct {
	size_t length;
} Array;


const Array* Array_construct(Array* self, const char* buffer,const size_t unitSize,const size_t count,const Memory* mallocator);



inline size_t Array_length(const Array* self) {
	return self ? self->length : 0;
}

inline const void* Array_buffer(const Array* self) {
	return (self->length == 0) ? 0 : (const char_t*)((char*)self + sizeof(Array));
}

const Array* Array_concat(const Array* self, const Array* right,const Memory* mallocator);

const Array* Array_subArray(const Array* self,const size_t unitSize, const size_t start, const size_t length, const Memory* mallocator);

typedef struct{
	size_t length;
} String;

extern const String* String_empty;

const String* String_construct(String* self, const char_t* buffer, size_t count,Memory* mallocator);



inline size_t String_length(String* self) {
	return self?self->length:0;
}

inline const char_t* String_buffer(const String* self) {
	return (self->length == 0)? 0: (const char_t*)((char*)self + sizeof(String));
}

const String* String_concat(const String* self, const String* right,const Memory* mallocator);

const String* String_substr(const String* self, const size_t start,const size_t length, const Memory* mallocator);
int String_search(const String* self, const String* token, size_t at);

void String_print(const String* self);

#endif
