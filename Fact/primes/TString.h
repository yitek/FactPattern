/*****************************************************
*
* author:yiy
*
* description: 字符串类(带类型)
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "String.h"
#ifndef __TSTRING_INCLUDED__ 
#define __TSTRING_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTString TString;
	const TString* TString__construct__(TString* self, const byte_t* buffer, usize_t byteCount, TMemory* memory,MemoryKinds mkind);
	void String__destruct__(String* self, bool_t existed, TMemory* memory);
	inline static usize_t TString_length(const TString* self) { return self ? self->length : 0; }
	inline static  byte_t* TString_buffer(const TString* self) { return self ? (byte_t*)(self + 1) : 0; }
	inline static usize_t TString_find(const TString* search, utf32_t ch, usize_t start, TMemory* mm) {
		if (search && search->length) {
			if (ch < 128 && start == 0) for (utf8_t* str = (utf8_t*)(search + 1); *str; str++) {
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
	inline static usize_t TString_search(const TString* search, const TString* pattern, usize_t start, TMemory* mm) {
		if (search && pattern && search->length >= pattern->length + start) {
			const utf8_t* searchBuffer = UTF8_skip((utf8_t*)(search + 1), start);
			EncodingCountResult rs = UTF8_sunday(searchBuffer, search->bytes, (utf8_t*)(pattern + 1), pattern->bytes, mm);
			return rs.charCount;
		}
		else return -1;
	}


	inline static unichar_t TString__get__(const TString* self, usize_t index) {
		return (self && index < self->length) ? UTF8_get((utf8_t*)(self + 1), index) : 0;
	}



	static inline favor_t TString__compare__(const TString* left, const TString* right) {
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

	inline static bool_t TString__eq__(const TString* left, const TString* right) { return TString__compare__(left, right) == 0; }
	inline static bool_t TString__gt__(const TString* left, const TString* right) { return TString__compare__(left, right) > 0; }
	inline static bool_t TString_lt__(const TString* left, const TString* right) { return TString__compare__(left, right) < 0; }
	inline static bool_t TString__gte__(const TString* left, const TString* right) { return TString__compare__(left, right) >= 0; }
	inline static bool_t TString_lte__(const TString* left, const TString* right) { return TString__compare__(left, right) <= 0; }

	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TSTRING_INCLUDED__



