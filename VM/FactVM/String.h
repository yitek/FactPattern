#pragma once
#include "MAllocator.h"

typedef struct TStr {
	size_t length;
} String;

extern String* String_empty;

String* String_construct(String* self, const wchar_t* buffer, size_t count,MAllocator* mallocator);



inline size_t String_length(String* self) {
	return self?self->length:0;
}

inline const wchar_t* String_buffer(const String* self) {
	return (self->length == 0)? 0: (const wchar_t*)((char*)self + sizeof(struct TStr));
}

const String* String_concat(const String* self, const String* right);

const String* String_substr(const String* self, const size_t start,const size_t length);
int String_search(const String* self, const String* token, size_t at);

void String_print(String* self);
