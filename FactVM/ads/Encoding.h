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

	typedef unsigned char UTF8;
	typedef unsigned short UTF16;
	typedef unsigned int UTF32;

	typedef struct stEncodingCountResult {
		size_t charCount;
		size_t byteCount;
	}EncodingCountResult;

	inline EncodingCountResult UTF32_count(const UTF32* src) {
		EncodingCountResult rs;
		UTF32 b;
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
		rs.byteCount = c * sizeof(UTF32);
		return rs;
	}
	inline const UTF32* UTF32_get(const UTF32* src,size_t index) {
		UTF32 b;
		if (!src || (b = *src) == 0) {
			return 0;
		}
		size_t c = 0;
		while (b) {
			if (c == index) return src;
			src++;
			c++;
		}
		return 0;
	}
	inline size_t UTF32_convertToUTF8(UTF32 src, UTF8* const des)
	{
		if (src == 0) return 0;

		static const byte_t PREFIX[] = { (byte_t)0x00, (byte_t)0xC0, (byte_t)0xE0, (byte_t)0xF0, (byte_t)0xF8, (byte_t)0xFC };
		static const UTF32 CODE_UP[] =
		{
			0x80,           // U+00000000 - U+0000007F  
			0x800,          // U+00000080 - U+000007FF  
			0x10000,        // U+00000800 - U+0000FFFF  
			0x200000,       // U+00010000 - U+001FFFFF  
			0x4000000,      // U+00200000 - U+03FFFFFF  
			0x80000000      // U+04000000 - U+7FFFFFFF  
		};

		size_t i, len = sizeof(CODE_UP) / sizeof(UTF32);
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


	inline size_t UTF32_convertToUTF16(UTF32 src, UTF16* const des)
	{
		if (src == 0) return 0;

		if (src <= 0xFFFF)
		{
			if (des) (*des) = (UTF16)(src);
			return 1;
		}
		else
			if (src <= 0xEFFFF)
			{
				if (des)
				{
					des[0] = (UTF16)(0xD800 + (src >> 10) - 0x40);  // high  
					des[1] = (UTF16)(0xDC00 + (src & 0x03FF));      // low  
				}
				return 2;
			}
		return 0;
	}


	inline EncodingCountResult UTF16_count(const UTF16* const src) {
		EncodingCountResult rs;
		UTF16 b = 0;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		size_t c = 0;
		const UTF16* p = (const UTF16*)src;
		while (b) {
			if (b >= 0xD800 && b <= 0xDFFF) {
				p += 1;
			}
			p += 1;
			b = *src;
			c++;
		}
		rs.charCount = c;
		rs.byteCount = ((size_t)(p - src)) * sizeof(UTF16);
		return rs;
	}

	inline const UTF16* UTF16_get(const UTF16* const src,size_t index) {
		UTF16 b = 0;
		if (!src || (b = *src) == 0) return 0;
		size_t c = 0;
		const UTF16* p = (const UTF16*)src;
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


	inline size_t UTF16_convertToUTF32(const UTF16* src, UTF32*const des)
	{
		if (!src || (*src) == 0) return 0;

		UTF16 w1 = src[0];
		// 需要辅助平面
		if (w1 >= 0xD800 && w1 <= 0xDFFF)
		{
			if (w1 < 0xDC00)
			{
				UTF16 w2 = src[1];
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



	inline EncodingCountResult UTF8_count(const UTF8* const src) {
		EncodingCountResult rs;
		UTF8 b;
		if (!src || (b = *src) == 0) {
			rs.byteCount = rs.charCount = 0;
			return rs;
		}
		size_t c = 0;
		UTF8* p = (UTF8*)src;
		while (b) {
			size_t len;
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
		rs.byteCount = (size_t)(p - src);
		rs.charCount = c;
		return rs;
	}

	inline const UTF8* UTF8_get(const UTF8* const src,size_t index) {
		UTF8 b;
		if (!src || (b = *src) == 0)return 0;
		size_t c = 0;
		UTF8* p = (UTF8*)src;
		while (b) {
			if (c == index) return p;
			size_t len;
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


	inline size_t UTF8_convertToUTF32(const UTF8* src, UTF32* const des)
	{
		UTF8 b;
		if (!src || (b = *src) == 0) return 0;
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
		for (size_t i = 1; i < len; ++i)
		{
			b = *src;
			if (b < 0x80 || b > 0xBF) return 0; // the src is invalid  
			*des = (*des << 6) + (b & 0x3F);// 00111111
			src++;
		}
		return len;
	}


	inline EncodingCountResult String_countUtf81(const utf8char_t* const src) {
		EncodingCountResult rs;
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
	


	inline size_t UTF16_convertToUTF8(UTF16 src, UTF8* const des)
	{
		UTF32 code;
		if (UTF16_convertToUTF32(&src, &code) != 1) return 0;
		return UTF32_convertToUTF8(code, des);
	}

	inline size_t UTF8_convertToUTF16(const UTF8* src, UTF16* const des)
	{
		UTF32 code;
		size_t len = UTF8_convertToUTF32(src, &code);
		if (len == 0) return 0;
		if (UTF32_convertToUTF16(code, (UTF16* const)&des) != 1) return 0;
		return len;
	}


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ENCODING_INCLUDED__



