#pragma once
#include "Encoding.h"
#include "Array.h"

#ifndef __STRING_INCLUDED__ 
#define __STRING_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stString {
		size_t length;
		size_t bytes;
	} String;

	

	extern const String* String_emptyInstance;

	typedef struct stStringEnumerator {
		bool_t(*next)(struct stStringEnumerator* enumerator);
		size_t bytes;
		size_t index;
		lchar_t current;
		const byte_t* buffer;
	}StringEnumerator;
	inline bool_t StringEnumerator8_next(StringEnumerator* enumerator) {
		lchar_t c;
		size_t cost = UTF8_convertToUTF32(enumerator->buffer, (lchar_t* const)&c);
		if (!cost)return 0;
		enumerator->bytes += cost;
		enumerator->current = c;
		enumerator->index++;
		return 1;
	}

	inline StringEnumerator* StringEnumerator_construct(StringEnumerator* enumerator,byte_t* buffer) {
		enumerator->buffer = buffer;
		enumerator->next = StringEnumerator8_next;
		return enumerator;
	}
	inline void StringEnumerator_reset(StringEnumerator* enumerator) {
		enumerator->bytes = 0;
		enumerator->index = -1;
		enumerator->current = 0;
	}
	inline char_t StringEnumerator_current(StringEnumerator* enumerator) { return enumerator->current; }
	inline bool_t StringEnumerator_next(StringEnumerator* enumerator) {
		return enumerator->next(enumerator);
	}
	
	const String* UTF32String_construct(String* self, const UTF32* buffer, size_t byteCount, void* mmArgs, Memory* memory);
	const String* UTF16String_construct(String* self, const UTF16* buffer, size_t byteCount, void* mmArgs, Memory* memory);
	const String* UTF8String_construct(String* self, const UTF8* buffer, size_t byteCount, void* mmArgs, Memory* memory);

	inline size_t String_length(const String* self) { return self?self->length:0; }
	inline byte_t* String_buffer(const String* self) { return self?(byte_t*)(self + 1):0; }
	const String* String_concat(const String* left, const String* right, void* mmArgs, Memory* memory);

	const String* String_substr(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory);
	

	/*inline size_t String_index(const String* self, lchar_t ch, size_t start) {
		if (!self || self->length == 0 || self->length <= start) return -1;
		byte_t* p = (byte_t*)(self + 1);
		size_t cost = 0; lchar_t c; size_t index = 0;
		while (cost = String_convertUtf8To32(p, (utf32char_t* const)&c)) {
			if (index >= start && c == ch)return index;
			index++;
		}
		return -1;
	}*/

	//size_t String_search(const String* self, const String* token, size_t start, Memory* memory);

	
	inline char_t String_get(const String* self, size_t index) { return index >= self->length ? 0 : *((char_t*)((char*)self + sizeof(String)) + index); }

	int String_compare(const String* left, const String* right);

	inline bool_t String_EQ(const String* left, const String* right) { return String_compare(left, right) == 0; }
	inline bool_t String_GT(const String* left, const String* right) { return String_compare(left, right) > 0; }
	inline bool_t String_LT(const String* left, const String* right) { return String_compare(left, right) < 0; }
	inline bool_t String_GTE(const String* left, const String* right) { return String_compare(left, right) >= 0; }
	inline bool_t String_LTE(const String* left, const String* right) { return String_compare(left, right) <= 0; }

	void String_cout(const String* self);
	void String_coutln(const String* self);



	inline size_t String_sunday32(const UTF32* searchBuffer, size_t searchLength, const UTF32* patternBuffer, size_t patternLength, Memory* mm) {
		// 模式串开始位置在主串的哪里
		size_t searchAt = 0;
		size_t step = searchLength - patternLength;
		size_t index = 0;
		// 模式串已经匹配到的位置
		size_t patternAt;
		while (searchAt <= step) {
			patternAt = 0;
			while (searchBuffer[searchAt + patternAt] == patternBuffer[patternAt]) {
				patternAt++;
				// 匹配成功
				if (patternAt >= searchLength) {
					return searchAt;// return s;由于是utf8，需要转换一下index
				}
			}
			// 找到主串中当前跟模式串匹配的最末字符的下一个字符
			// 在模式串中出现最后的位置
			// 所需要从(模式串末尾+1)移动到该位置的步数
			//默认是m+1
			size_t shiftCount = searchLength + 1;
			byte_t c = searchBuffer[searchAt + patternLength];
			for (size_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}

			searchAt += shiftCount;
		}
		return -1;
}

	inline size_t String_sunday16(const UTF16* searchBuffer, size_t searchLength, const UTF16* patternBuffer, size_t patternLength, Memory* mm) {
		// 模式串开始位置在主串的哪里
		size_t searchAt = 0;
		size_t step = searchLength - patternLength;
		size_t index = 0;
		// 模式串已经匹配到的位置
		size_t patternAt;
		while (searchAt <= step) {
			patternAt = 0;
			while (searchBuffer[searchAt + patternAt] == patternBuffer[patternAt]) {
				patternAt++;
				// 匹配成功
				if (patternAt >= searchLength) {
					return searchAt;// return s;由于是utf8，需要转换一下index
				}
			}
			size_t shiftCount = searchLength + 1;
			byte_t c = searchBuffer[searchAt + patternLength];
			for (size_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}

			searchAt += shiftCount;
		}
		return -1;
	}


	inline size_t String_sunday8(const UTF8* searchBuffer, size_t searchLength, const UTF8* patternBuffer, size_t patternLength, Memory* mm) {
		// 模式串开始位置在主串的哪里
		size_t searchAt = 0;
		size_t step = searchLength - patternLength;
		size_t index = 0;
		// 模式串已经匹配到的位置
		size_t patternAt;
		while (searchAt <= step) {
			patternAt = 0;
			while (searchBuffer[searchAt + patternAt] == patternBuffer[patternAt]) {
				patternAt++;
				// 匹配成功
				if (patternAt >= searchLength) {
					return searchAt;// return s;由于是utf8，需要转换一下index
				}
			}
			size_t shiftCount = searchLength + 1;
			byte_t c = searchBuffer[searchAt + patternLength];
			for (size_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}

			searchAt += shiftCount;
		}
		return -1;
	}

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif
