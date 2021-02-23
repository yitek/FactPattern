/*****************************************************
*
* author:yiy
*
* description: 字符编码类
*
******************************************************/

#pragma once
#include "../def.h"
#ifndef __ENCODING_INCLUDED__ 
#define __ENCODING_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum {
		Encoding_Utf8,
		Encoding_Utf16,
		Encoding_Utf32
	}Encodings;

	typedef unsigned char utf8_t;
	typedef unsigned short utf16_t;
	typedef unsigned int utf32_t;

	typedef struct stEncodingCountResult {
		usize_t charCount;
		usize_t byteCount;
	}EncodingCountResult;

	inline static EncodingCountResult UTF32_count(const utf32_t* src) {
		EncodingCountResult rs;
		utf32_t b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		usize_t c = 0;
		while (b) {
			src++;
			c++;
		}
		rs.charCount = c;
		rs.byteCount = c * sizeof(utf32_t);
		return rs;
	}
	inline static const utf32_t* UTF32_get(const utf32_t* src,usize_t index) {
		utf32_t b;
		if (!src || (b = *src) == 0) {
			return 0;
		}
		usize_t c = 0;
		while (b) {
			if (c == index) return src;
			src++;
			c++;
		}
		return 0;
	}
	inline static usize_t UTF32_convertToUTF8(utf32_t src, utf8_t* const des)
	{
		if (src == 0) return 0;

		static const byte_t PREFIX[] = { (byte_t)0x00, (byte_t)0xC0, (byte_t)0xE0, (byte_t)0xF0, (byte_t)0xF8, (byte_t)0xFC };
		static const utf32_t CODE_UP[] =
		{
			0x80,           // U+00000000 - U+0000007F  
			0x800,          // U+00000080 - U+000007FF  
			0x10000,        // U+00000800 - U+0000FFFF  
			0x200000,       // U+00010000 - U+001FFFFF  
			0x4000000,      // U+00200000 - U+03FFFFFF  
			0x80000000      // U+04000000 - U+7FFFFFFF  
		};

		usize_t i, len = sizeof(CODE_UP) / sizeof(utf32_t);
		for (i = 0; i < len; ++i)
			if (src < CODE_UP[i]) break;

		if (i == len) return 0; // the src is invalid  

		len = i + 1;
		if (des)
		{
			for (; i > 0; --i)
			{
				des[i] = (utf8_t)((src & 0x3F) | 0x80);
				src >>= 6;
			}
			des[0] = (utf8_t)(src | PREFIX[len - 1]);
		}
		return len;
	}


	inline static usize_t UTF32_convertToUTF16(utf32_t src, utf16_t* const des)
	{
		if (src == 0) return 0;

		if (src <= 0xFFFF)
		{
			if (des) (*des) = (utf16_t)(src);
			return 1;
		}
		else
			if (src <= 0xEFFFF)
			{
				if (des)
				{
					des[0] = (utf16_t)(0xD800 + (src >> 10) - 0x40);  // high  
					des[1] = (utf16_t)(0xDC00 + (src & 0x03FF));      // low  
				}
				return 2;
			}
		return 0;
	}


	inline static EncodingCountResult UTF16_count(const utf16_t* const src) {
		EncodingCountResult rs;
		utf16_t b = 0;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		usize_t c = 0;
		const utf16_t* p = (const utf16_t*)src;
		while (b) {
			if (b >= 0xD800 && b <= 0xDFFF) {
				p += 1;
			}
			p += 1;
			b = *src;
			c++;
		}
		rs.charCount = c;
		rs.byteCount = ((usize_t)(p - src)) * sizeof(utf16_t);
		return rs;
	}

	inline static const utf16_t* UTF16_get(const utf16_t* const src,usize_t index) {
		utf16_t b = 0;
		if (!src || (b = *src) == 0) return 0;
		usize_t c = 0;
		const utf16_t* p = (const utf16_t*)src;
		while (b) {
			if (c == index) return p;
			if (b >= 0xD800 && b <= 0xDFFF) {
				p += 1;
			}
			p += 1;
			b = *src;
			c++;
		}
		return 0;
	}


	inline static usize_t UTF16_convertToUTF32(const utf16_t* src, utf32_t*const des)
	{
		if (!src || (*src) == 0) return 0;

		utf16_t w1 = src[0];
		// 需要辅助平面
		if (w1 >= 0xD800 && w1 <= 0xDFFF)
		{
			if (w1 < 0xDC00)
			{
				utf16_t w2 = src[1];
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



	inline static EncodingCountResult UTF8_count(const utf8_t* const src) {
		EncodingCountResult rs;
		utf8_t b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		usize_t c = 0;
		utf8_t* p = (utf8_t*)src;
		while (b) {
			usize_t len;
			if (b < 0x80)len = 1;
			else if (b < 0xE0)len = 2;
			else if (b < 0xF0)len = 3;
			else if (b < 0xF8)len = 4;
			else if (b < 0xFC)len = 5;
			else len = 6;
			//你1好2吗3.4I5'6a7m8 9Y0i1.
			p += len;
			b = *p;
			c++;
		}
		rs.byteCount = (usize_t)(p - src);
		rs.charCount = c;
		return rs;
	}

	inline static const utf8_t* UTF8_get(const utf8_t* const src,usize_t index) {
		utf8_t b;
		if (!src || (b = *src) == 0)return 0;
		usize_t c = 0;
		utf8_t* p = (utf8_t*)src;
		while (b) {
			if (c == index) return p;
			usize_t len;
			if (b < 0x80)len = 1;
			else if (b < 0xE0)len = 2;
			else if (b < 0xF0)len = 3;
			else if (b < 0xF8)len = 4;
			else if (b < 0xFC)len = 5;
			else len = 6;
			//你1好2吗3.4I5'6a7m8 9Y0i1.
			p += len;
			b = *p;
			c++;
		}
		return 0;
	}


	inline static usize_t UTF8_convertToUTF32(const utf8_t* src, utf32_t* const des)
	{
		utf8_t b;
		if (!src || (b = *src) == 0) return 0;
		if (b < 0x80)
		{
			*des = b;
			return 1;
		}

		if (b < 0xC0 || b > 0xFD) return 0; // the src is invalid  
		usize_t len;

		if (b < 0xE0)
		{
			*des = b & 0x1F;
			len = 2;
		}
		else if (b < 0xF0) {
			*des = b & 0x0F;
			len = 3;
		}
		else if (b < 0xF8) {
			*des = b & 0x07;
			len = 4;
		}
		else if (b < 0xFC) {
			*des = b & 0x03;
			len = 5;
		}
		else {
			*des = b & 0x01;
			len = 6;
		}
		src++;
		for (usize_t i = 1; i < len; ++i)
		{
			b = *src;
			if (b < 0x80 || b > 0xBF) return 0; // the src is invalid  
			*des = (*des << 6) + (b & 0x3F);// 00111111
			src++;
		}
		return len;
	}


	inline static EncodingCountResult String_countUtf81(const utf8_t* const src) {
		EncodingCountResult rs;
		utf8_t b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		usize_t c = 0;
		const utf8_t* p = src;
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
	


	inline static usize_t UTF16_convertToUTF8(utf16_t src, utf8_t* const des)
	{
		utf32_t code;
		if (UTF16_convertToUTF32(&src, &code) != 1) return 0;
		return UTF32_convertToUTF8(code, des);
	}

	inline static usize_t UTF8_convertToUTF16(const utf8_t* src, utf16_t* const des)
	{
		utf32_t code;
		usize_t len = UTF8_convertToUTF32(src, &code);
		if (len == 0) return 0;
		if (UTF32_convertToUTF16(code, (utf16_t* const)&des) != 1) return 0;
		return len;
	}

	inline static const utf8_t* UTF8_skip(const utf8_t* p,usize_t count) {
		utf8_t b = *p;
		while (b && count--) {
			usize_t len;
			if (b < 0x80)len = 1;
			else if (b < 0xE0)len = 2;
			else if (b < 0xF0)len = 3;
			else if (b < 0xF8)len = 4;
			else if (b < 0xFC)len = 5;
			else len = 6;
			//你1好2吗3.4I5'6a7m8 9Y0i1.
			p += len;
			b = *p;
		}
		return p;
	}



	inline static usize_t UTF32_sunday(const utf32_t* searchBuffer, usize_t searchLength, const utf32_t* patternBuffer, usize_t patternLength, void* mm) {
		// 模式串开始位置在主串的哪里
		usize_t searchAt = 0;
		usize_t step = searchLength - patternLength;
		usize_t index = 0;
		// 模式串已经匹配到的位置
		usize_t patternAt;
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
			usize_t shiftCount = searchLength + 1;
			utf32_t c = searchBuffer[searchAt + patternLength];
			for (usize_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}

			searchAt += shiftCount;
		}
		return -1;
	}

	inline static usize_t UTF16_sunday(const utf16_t* searchBuffer, usize_t searchLength, const utf16_t* patternBuffer, usize_t patternLength, void* mm) {
		// 模式串开始位置在主串的哪里
		usize_t searchAt = 0;
		usize_t step = searchLength - patternLength;
		usize_t index = 0;
		// 模式串已经匹配到的位置
		usize_t patternAt;
		while (searchAt <= step) {
			patternAt = 0;
			while (searchBuffer[searchAt + patternAt] == patternBuffer[patternAt]) {
				patternAt++;
				// 匹配成功
				if (patternAt >= searchLength) {
					return searchAt;// return s;由于是utf8，需要转换一下index
				}
			}
			usize_t shiftCount = searchLength + 1;
			utf16_t c = searchBuffer[searchAt + patternLength];
			for (usize_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}

			searchAt += shiftCount;
		}
		return -1;
	}


	inline static EncodingCountResult UTF8_sunday(const utf8_t* searchBuffer, usize_t searchLength, const utf8_t* patternBuffer, usize_t patternLength, void* mm) {
		EncodingCountResult rs;
		rs.byteCount = rs.charCount = (usize_t)-1;
		// 模式串开始位置在主串的哪里
		usize_t searchAt = 0;
		usize_t step = searchLength - patternLength;
		usize_t index = 0;
		//字符个数位置
		usize_t cc = 0;
		usize_t len = 0;
		// 模式串已经匹配到的位置
		usize_t patternAt;
		while (searchAt <= step) {
			patternAt = 0;
			if (len) { len--; } else {
				cc++;
				utf8_t b= searchBuffer[searchAt];
				if (b < 0x80)len = 0;
				else if (b < 0xE0)len = 1;
				else if (b < 0xF0)len = 2;
				else if (b < 0xF8)len = 3;
				else if (b < 0xFC)len = 4;
				else len = 5;
			}
			while (searchBuffer[searchAt + patternAt] == patternBuffer[patternAt]) {
				patternAt++;
				// 匹配成功
				if (patternAt >= searchLength) {
					rs.charCount = cc; rs.byteCount = searchAt;
					return rs;// return s;由于是utf8，需要转换一下index
				}
			}
			usize_t shiftCount = searchLength + 1;
			utf8_t c = searchBuffer[searchAt + patternLength];
			for (usize_t k = 0, q = patternLength; k < q; k++) {
				if (patternBuffer[k] == c) {
					shiftCount = patternLength - k;
					break;
				}
			}
			//首先计算移动了多少个字符
			if (len != 0) {
				searchAt += len; shiftCount -= len;
				if (shiftCount < 0) {
					searchAt += shiftCount; // searchAt+ len + shiftCount-len; 只移动了shiftCount个字节
					len -= shiftCount; // shiftCount已经被消耗掉了。
				}
				while (shiftCount) {
					if (len) { len--; }
					else {
						cc++;
						utf8_t b = searchBuffer[searchAt];
						if (b < 0x80)len = 0;
						else if (b < 0xE0)len = 1;
						else if (b < 0xF0)len = 2;
						else if (b < 0xF8)len = 3;
						else if (b < 0xFC)len = 4;
						else len = 5;
					}
					shiftCount--;
					searchAt++;
				}
			}
			searchAt += shiftCount;

		}
		return rs;
	}
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ENCODING_INCLUDED__



