#pragma once
#include "Array.h"

#ifndef __STRING_INCLUDED__ 
#define __STRING_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct stString {
		size_t length;
	} String;

	extern const String* String_empty;

	inline const String* String___construct__(String* self, const char_t* buffer,const size_t count, void* mmArgs, Memory* memory) {
		size_t c = count;
		if (count == -1 && buffer) { c = 0; while (buffer[c])c++; }
		return (String*)Array___construct__((Array*)self, buffer, c, sizeof(char_t), mmArgs, memory);
	}
	inline size_t String_length(const String* self) { return self->length; }
	inline char_t* String_buffer(const String* self) { return (char_t*)(self + 1); }

	inline const String* String_concat(const String* left, const String* right, void* mmArgs, Memory* memory) {
		return (String*)Array_concat((Array*)left, (Array*)right, sizeof(char_t), mmArgs, memory);
	}

	inline const String* String_substr(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory) {
		return (String*)Array_clip((Array*)arr, start, length, sizeof(char_t), mmArgs, memory);
	}

	inline const String* String_clip(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory) {
		return (String*)Array_clip((Array*)arr, start, length, sizeof(char_t), mmArgs, memory);
	}


	int String_search(const String* self, const String* token, size_t at, Memory* memory);

	
	inline char_t String___INDEXGETER__(const String* self, size_t index) { return index >= self->length ? 0 : *((char_t*)((char*)self + sizeof(String)) + index); }

	int String_compare(const String* left, const String* right);

	inline bool_t String___EQ__(const String* left, const String* right) { return String_compare(left, right) == 0; }
	inline bool_t String___GT__(const String* left, const String* right) { return String_compare(left, right) > 0; }
	inline bool_t String___LT__(const String* left, const String* right) { return String_compare(left, right) < 0; }
	inline bool_t String___GTE__(const String* left, const String* right) { return String_compare(left, right) >= 0; }
	inline bool_t String___LTE__(const String* left, const String* right) { return String_compare(left, right) <= 0; }

	void String_cout(const String* self);
	void String_coutln(const String* self);

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif
