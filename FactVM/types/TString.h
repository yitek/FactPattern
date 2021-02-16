/*****************************************************
*
* author:yiy
*
* description: ×Ö´®Àà

*
******************************************************/

#pragma once
#include "TObject.h"
#include "../ads/String.h"
#ifndef __TSTRING_INCLUDED__ 
#define __TSTRING_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif



	inline const String* TString___construct__(String* self, const char_t* buffer, const size_t count) {
		size_t c = count;
		if (count == -1 && buffer) { c = 0; while (buffer[c])c++; }
		const Type* type = TObject_getType(self);
		return (String*)Array_construct((Array*)self, buffer, c, sizeof(char_t), (void*)type, type->memory);
	}
	inline size_t TString_length(const String* self) { return self?self->length:0; }
	inline char_t* TString_buffer(const String* self) { return self?(char_t*)(self + 1):0; }

	inline const String* TString_concat(const String* left, const String* right) {
		if (!left)return right; if (!right)return left;
		const Type* type = TObject_getType((void*)left);
		return (String*)Array_concat((Array*)left, (Array*)right, sizeof(char_t), (void*)type, type->memory);
	}

	inline const String* TString_substr(const String* arr, const size_t start, const size_t length) {
		const Type* type = TObject_getType((void*)arr);
		return type?(String*)Array_clip((Array*)arr, start, length, sizeof(char_t), (void*)type, type->memory):0;
	}

	inline const String* String_clip(const String* arr, const size_t start, const size_t length) {
		const Type* type = TObject_getType((void*)arr);
		return type?(String*)Array_clip((Array*)arr, start, length, sizeof(char_t), (void*)type, type->memory):0;
	}


	int TString_search(const String* self, const String* token, size_t at) {
		return String_search(self, token, at, TObject_getMemory((void*)self));
	}


	inline char_t TString___INDEXGETER__(const String* self, size_t index) { return self && index < self->length ? *((char_t*)((char*)self + sizeof(String)) + index):0; }




#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TSTRING_INCLUDED__



