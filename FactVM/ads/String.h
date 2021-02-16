#pragma once
#include "Array.h"

#ifndef __STRING_INCLUDED__ 
#define __STRING_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stStringCountResult {
		size_t charCount;
		size_t byteCount;
	}StringCountResult;

	inline size_t String_convertUtf32To8(utf32char_t src, utf8char_t* const des)
	{
		if (src == 0) return 0;

		static const word_t PREFIX[] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
		static const utf32char_t CODE_UP[] =
		{
			0x80,           // U+00000000 - U+0000007F  
			0x800,          // U+00000080 - U+000007FF  
			0x10000,        // U+00000800 - U+0000FFFF  
			0x200000,       // U+00010000 - U+001FFFFF  
			0x4000000,      // U+00200000 - U+03FFFFFF  
			0x80000000      // U+04000000 - U+7FFFFFFF  
		};

		size_t i, len = sizeof(CODE_UP) / sizeof(utf32char_t);
		for (i = 0; i < len; ++i)
			if (src < CODE_UP[i]) break;

		if (i == len) return 0; // the src is invalid  

		len = i + 1;
		if (des)
		{
			for (; i > 0; --i)
			{
				des[i] = (utf8char_t)((src & 0x3F) | 0x80);
				src >>= 6;
			}
			des[0] = (utf8char_t)(src | PREFIX[len - 1]);
		}
		return len;
	}

	inline StringCountResult String_countUtf8(const utf8char_t* const src) {
		StringCountResult rs;
		utf8char_t b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		size_t c = 0;
		const utf8char_t* p = src;
		while (b) {
			if (b & 0b11111000) p += 5;
			else if (b & 0b11110000) p += 4;
			else if (b & 0b11100000) p += 3;
			else if (b & 0b11000000) p += 2;
			else if (b & 0b10000000) {
				rs.byteCount = p - src;
				rs.charCount = -1;
				return rs;
			}
			else p += 1;
			b = *(p);
			c++;
		}
		rs.byteCount = p - src;
		rs.charCount = c;
		return rs;
	}
	inline StringCountResult String_countUtf16(const utf16char_t* const src) {
		StringCountResult rs;
		utf16char_t b = 0;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		size_t c = 0;
		const utf16char_t* p = (const utf16char_t*)src;
		while (b) {
			if (b >= 0xD800 && b <= 0xDFFF) {
				p += 1;
			}
			p += 1;
			b = *src;
			c++;
		}
		rs.charCount = c;
		rs.byteCount = ((size_t)(p - src)) * sizeof(utf16char_t);
		return rs;
	}
	inline StringCountResult String_countUtf32(const utf32char_t* src) {
		StringCountResult rs;
		utf32char_t b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		size_t c = 0;
		while (b) {
			src++;
			c++;
		}
		rs.charCount = c;
		rs.byteCount = c * sizeof(utf32char_t);
		return rs;
	}

	inline size_t String_convertUtf8To32(const utf8char_t* src, utf32char_t* const des)
	{
		if (!src || (*src) == 0) return 0;

		utf8char_t b = *(src++);

		if (b < 0x80)
		{
			*des = b;
			return 1;
		}

		if (b < 0xC0 || b > 0xFD) return 0; // the src is invalid  

		size_t len;

		if (b < 0xE0)
		{
			*des = b & 0x1F;
			len = 2;
		}
		else
			if (b < 0xF0)
			{
				*des = b & 0x0F;
				len = 3;
			}
			else
				if (b < 0xF8)
				{
					*des = b & 0x07;
					len = 4;
				}
				else
					if (b < 0xFC)
					{
						*des = b & 0x03;
						len = 5;
					}
					else
					{
						*des = b & 0x01;
						len = 6;
					}

		size_t i = 1;
		for (; i < len; ++i)
		{
			b = *(src++);
			if (b < 0x80 || b > 0xBF) return 0; // the src is invalid  
			*des = (*des << 6) + (b & 0x3F);
		}
		return len;
	}
	inline size_t String_convertUtf32To16(utf32char_t src, utf16char_t* const des)
	{
		if (src == 0) return 0;

		if (src <= 0xFFFF)
		{
			if (des) (*des) = (utf16char_t)(src);
			return 1;
		}
		else
			if (src <= 0xEFFFF)
			{
				if (des)
				{
					des[0] = (utf16char_t)(0xD800 + (src >> 10) - 0x40);  // high  
					des[1] = (utf16char_t)(0xDC00 + (src & 0x03FF));      // low  
				}
				return 2;
			}
		return 0;
	}
	inline size_t String_convertUtf16To32(const utf16char_t* src, utf32char_t* des)
	{
		if (!src || (*src) == 0) return 0;

		utf16char_t w1 = src[0];
		// 需要辅助平面
		if (w1 >= 0xD800 && w1 <= 0xDFFF)
		{
			if (w1 < 0xDC00)
			{
				utf16char_t w2 = src[1];
				if (w2 >= 0xDC00 && w2 <= 0xDFFF)
				{
					*des = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
					return 2;
				}
			}
			//辅助平面的码点不在规范之内
			return 0; // the src is invalid  
		}
		else
		{
			*des = w1;
			return 1;
		}
	}

	inline size_t String_convertUtf16To8(utf16char_t src, utf8char_t* const des)
	{
		utf32char_t code;
		if (String_convertUtf16To32(&src, &code) != 1) return 0;
		return String_convertUtf32To8(code, des);
	}

	inline size_t String_convertUtf8To16(const utf8char_t* src, utf16char_t* const des)
	{
		utf32char_t code;
		size_t len = String_convertUtf8To32(src, &code);
		if (len == 0) return 0;
		if (String_convertUtf32To16(code, (utf16char_t* const)&des) != 1) return 0;
		return len;
	}

	typedef enum {
		Encoding_Utf8,
		Encoding_Utf16,
		Encoding_Utf32
	}Encodings;
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
		size_t cost = String_convertUtf8To32(enumerator->buffer, (lchar_t* const)&c);
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
	


	const String* String_construct(String* self, const byte_t* buffer, size_t byteCount,  void* mmArgs, Memory* memory);
	
	inline size_t String_length(const String* self) { return self?self->length:0; }
	inline byte_t* String_buffer(const String* self) { return self?(byte_t*)(self + 1):0; }
	const String* String_concat(const String* left, const String* right, void* mmArgs, Memory* memory);

	const String* String_substr(const String* arr, const size_t start, const size_t length, void* mmArgs, Memory* memory);
	

	inline size_t String_index(const String* self, lchar_t ch, size_t start) {
		if (!self || self->length == 0 || self->length <= start) return -1;
		byte_t* p = (byte_t*)(self + 1);
		size_t cost = 0; lchar_t c; size_t index = 0;
		while (cost = String_convertUtf8To32(p, (utf32char_t* const)&c)) {
			if (index >= start && c == ch)return index;
			index++;
		}
		return -1;
	}

	size_t String_search(const String* self, const String* token, size_t start, Memory* memory);

	
	inline char_t String_get(const String* self, size_t index) { return index >= self->length ? 0 : *((char_t*)((char*)self + sizeof(String)) + index); }

	int String_compare(const String* left, const String* right);

	inline bool_t String_EQ(const String* left, const String* right) { return String_compare(left, right) == 0; }
	inline bool_t String_GT(const String* left, const String* right) { return String_compare(left, right) > 0; }
	inline bool_t String_LT(const String* left, const String* right) { return String_compare(left, right) < 0; }
	inline bool_t String_GTE(const String* left, const String* right) { return String_compare(left, right) >= 0; }
	inline bool_t String_LTE(const String* left, const String* right) { return String_compare(left, right) <= 0; }

	void String_cout(const String* self);
	void String_coutln(const String* self);



#ifdef __cplusplus 
}//extern "C" {
#endif
#endif
