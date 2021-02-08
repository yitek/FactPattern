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

	inline const String* String___construct__(String* self, const char_t* buffer, size_t count, void* mmArgs, Memory* memory) {
		size_t c = 0;
		if (count = -1 && buffer) {while (buffer[c])c++;}
		return (String*)Array___construct__((Array*)self,buffer,count,sizeof(char_t),mmArgs,memory);
	}

	inline const String* String_concat(const String* left, const String* right, void* mmArgs, Memory* memory) {
		return (String*)Array_concat((Array*)left, (Array*)right,sizeof(char_t),mmArgs,memory);
	}

	inline const String* String_substr(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory){
		return (String*)Array_clip((Array*)arr,start,length, sizeof(char_t), mmArgs, memory);
	}

	inline const String* String_clip(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory) {
		return (String*)Array_clip((Array*)arr, start, length, sizeof(char_t), mmArgs, memory);
	}


	int String_search(const String* self, const String* token, size_t at);

	inline const char_t* String___INDEX__(const String* self, size_t index) {
		return (char_t*)((index >= self->length) ? 0 : ((char*)self + sizeof(String) + index * sizeof(char_t)));
	}
	inline const char_t* String_buffer(const String* self) { return (const char_t*)((char*)self + sizeof(String)); }

	void String_cout(const String* self);

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif
