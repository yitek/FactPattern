/*****************************************************
*
* author:yiy
*
* description: 字符串
*
******************************************************/

#pragma once
#include "../runtime.h"
#include "Encoding.h"
#ifndef __STRING_INCLUDED__ 
#define __STRING_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stString String;
	struct stStringEmpty {
		struct stMRefUnit;
		struct stTObject __ob__;
		struct stString inst;
		unichar_t endl;
	};


	extern const String*const String_empty;
	extern const struct stStringEmpty String__emptyInstance;

	//const String* UTF32String_construct(String* self, const UTF32* buffer, usize_t byteCount, void* mmArgs, Memory* memory);
	//const String* UTF16String_construct(String* self, const UTF16* buffer, usize_t byteCount, void* mmArgs, Memory* memory);
	const String* String__construct__(String* self, const byte_t* buffer, usize_t byteCount, TMemory* memory, void* type, MemoryKinds mkind);
	const String* String_concat(const String* left, const String* right, TMemory* memory, void* type, MemoryKinds mkind);
	const String* String_clip(const String* arr, const usize_t start, usize_t length, TMemory* memory, void* type, MemoryKinds mkind);
	inline static void String__destruct__(String* self, bool_t existed,TMemory* memory) {
		if (!existed) m_free(self,memory?memory:TMemory_default);
	}
	inline static usize_t String_length(const String* self) { return self ? self->length : 0; }
	inline static  byte_t* String_buffer(const String* self) { return self ? (byte_t*)(self + 1) : 0; }
	inline static usize_t String_find(const String* search, utf32_t ch, usize_t start, TMemory* mm) {
		if (search && search->length) {
			if (ch < 128 && start==0) for (utf8_t* str = (utf8_t*)(search + 1); *str; str++) {
				if (*str == ch) return str - (utf8_t*)(search - 1);
			}
			else {
				utf8_t pattern[6];
				ufavor_t patternLen = UTF32_convertToUTF8(ch, pattern);
				if (patternLen) {
					const utf8_t* searchBuffer = UTF8_skip((utf8_t*)(search + 1), start);
					EncodingCountResult rs = UTF8_sunday(searchBuffer, search->bytes, pattern, patternLen, mm);
					return rs.charCount;
				}
			}
		}
		return -1;
	}
	inline static usize_t String_search(const String* search, const String* pattern, usize_t start,TMemory * mm) {
		if (search && pattern && search->length >= pattern->length + start) {
			const utf8_t* searchBuffer = UTF8_skip((utf8_t*)(search + 1),start);
			EncodingCountResult rs = UTF8_sunday(searchBuffer, search->bytes, (utf8_t*)(pattern + 1), pattern->bytes, mm);
			return rs.charCount;
		}
		else return -1;
	}


	inline static unichar_t String__get__(const String* self, usize_t index) {
		return (self && index < self->length)? UTF8_get((utf8_t*)(self+1),index):0;
	}



	static inline favor_t String__compare__(const String* left,const String* right) {
		if (left) {
			if (right) {
				return m_compare(left + 1, right + 1);
			}
			else return 1;
		}
		else {
			if (right) return -1;
			else return 0;
		}
	}

	inline static bool_t String_EQ(const String* left, const String* right) { return String__compare__(left, right) == 0; }
	inline static bool_t String_GT(const String* left, const String* right) { return String__compare__(left, right) > 0; }
	inline static bool_t String_LT(const String* left, const String* right) { return String__compare__(left, right) < 0; }
	inline static bool_t String_GTE(const String* left, const String* right) { return String__compare__(left, right) >= 0; }
	inline static bool_t String_LTE(const String* left, const String* right) { return String__compare__(left, right) <= 0; }

	//void String_cout(const String* self);
	//void String_coutln(const String* self);


#ifdef __cplusplus 
}//extern "C" 
#endif

#endif // __STRING_INCLUDED__
