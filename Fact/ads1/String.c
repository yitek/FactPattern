#pragma once
#include "String.h"
#include "../memories/Memory.h"
#include <stdio.h>

const String* String_emptyInstance = 0;

const String* UTF32String_construct(String* self, const UTF32* buffer, size_t byteCount, void* mmArgs, Memory* memory) {
	size_t charCount = -1;
	if (byteCount == 0) {
		if (buffer) {
			EncodingCountResult c = UTF32_count((const byte_t*const)buffer);
			byteCount = c.byteCount;
			charCount = c.charCount;
		}
	}
	if (!self) {
		if (!memory) memory = Memory_default();
		self = memory->require(memory, byteCount + sizeof(UTF32) + sizeof(String), mmArgs);
	}
	if (buffer&& byteCount) {
		Memory_copy(self + 1, buffer, byteCount);
		self->bytes = byteCount;
	}
	else {
		Memory_clear(self + 1, buffer, byteCount);
	}
	UTF32* p = (((UTF32*)(self + 1)) + byteCount);
	*p = 0;
	if (charCount != -1) self->length = charCount;
	return self;
}
const String* UTF16String_construct(String* self, const UTF16* buffer, size_t byteCount, void* mmArgs, Memory* memory) {
	size_t charCount = -1;
	if (byteCount == 0) {
		if (buffer) {
			EncodingCountResult c = UTF16_count((const byte_t* const)buffer);
			byteCount = c.byteCount;
			charCount = c.charCount;
		}
	}
	if (!self) {
		if (!memory) memory = Memory_default();
		self = memory->require(memory, byteCount + sizeof(UTF16) + sizeof(String), mmArgs);
	}
	if (buffer && byteCount) {
		Memory_copy(self + 1, buffer, byteCount);
		self->bytes = byteCount;
	}
	else {
		Memory_clear(self + 1, buffer, byteCount);
	}
	UTF16* p = (((UTF16*)(self + 1)) + byteCount);
	*p = 0;
	if (charCount != -1) self->length = charCount;
	return self;
}


const String* UTF8String_construct(String* self, const UTF8* buffer, size_t byteCount, void* mmArgs, Memory* memory) {
	size_t charCount = -1;
	if (byteCount == 0) {
		if (buffer) {
			EncodingCountResult c = UTF8_count((const byte_t* const)buffer);
			byteCount = c.byteCount;
			charCount = c.charCount;
		}
	}
	if (!self) {
		if (!memory) memory = Memory_default();
		self = memory->require(memory, byteCount + sizeof(UTF8) + sizeof(String), mmArgs);
	}
	if (buffer && byteCount) {
		Memory_copy(self + 1, buffer, byteCount);
		self->bytes = byteCount;
	}
	else {
		Memory_clear(self + 1, buffer, byteCount);
	}
	UTF8* p = (((UTF8*)(self + 1)) + byteCount);
	*p = 0;
	if (charCount != -1) self->length = charCount;
	return self;
}

const String* UTF8String_substr(const String* arr, const size_t start, size_t length, void* mmArgs, Memory* memory) {
	if (!arr || arr->length <= start)return 0;
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF8String_construct(0, 0, 0, mmArgs, memory);
	}
	size_t end = start + length;
	if (end >= arr->length) { end = arr->length-1; length = end - start; }
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF8String_construct(0, 0, 0, mmArgs, memory);
	}
	const UTF8* buffer = (const UTF8*)(arr + 1);
	UTF8* startp;
	size_t bytes;
	if (arr->length == arr->length) {
		startp = buffer + start;
		bytes = length;
	}
	else {
		startp =(UTF8*) UTF8_get(buffer, start);
		const UTF8* endp = UTF8_get(startp, length);
		bytes = endp - startp;
	}
	
	if (!memory) memory = Memory_default();
	String* sub = (String*)memory->require(memory, sizeof(String) + bytes + 1, mmArgs);
	sub->length = length;
	sub->bytes = bytes;
	Memory_copy(sub + 1, startp, bytes);
	startp += bytes;
	*startp = 0;
	return sub;
}

const String* UTF16String_substr(const String* arr, const size_t start, size_t length, void* mmArgs, Memory* memory) {
	if (!arr || arr->length <= start)return 0;
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF16String_construct(0, 0, 0, mmArgs, memory);
	}
	size_t end = start + length;
	if (end >= arr->length) { end = arr->length - 1; length = end - start; }
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF16String_construct(0, 0, 0, mmArgs, memory);
	}
	const UTF16* buffer = (const UTF16*)(arr + 1);
	UTF16* startp;
	size_t bytes;
	if (arr->length == arr->length) {
		startp = buffer + start;
		bytes = length;
	}
	else {
		startp = (UTF16*)UTF16_get(buffer, start);
		const UTF16* endp = UTF16_get(startp, length);
		bytes = ((size_t)(endp - startp))*sizeof(UTF16);
	}

	if (!memory) memory = Memory_default();
	String* sub = (String*)memory->require(memory, sizeof(String) + bytes + sizeof(UTF16), mmArgs);
	sub->length = length;
	sub->bytes = bytes;
	Memory_copy(sub + 1, startp, bytes);
	startp =(UTF16*)(((byte_t*)startp) + bytes);
	*startp = 0;
	return sub;
}

const String* UTF32String_substr(const String* arr, const size_t start, size_t length, void* mmArgs, Memory* memory) {
	if (!arr || arr->length <= start)return 0;
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF32String_construct(0, 0, 0, mmArgs, memory);
	}
	size_t end = start + length;
	if (end >= arr->length) { end = arr->length - 1; length = end - start; }
	if (length == 0) {
		if (String_emptyInstance)return String_emptyInstance;
		return UTF32String_construct(0, 0, 0, mmArgs, memory);
	}
	const UTF32* buffer = (const UTF32*)(arr + 1);
	UTF16* startp = buffer + start;
	size_t bytes = length*sizeof(UTF32);
	
	if (!memory) memory = Memory_default();
	String* sub = (String*)memory->require(memory, sizeof(String) + bytes + sizeof(UTF32), mmArgs);
	sub->length = length;
	sub->bytes = bytes;
	Memory_copy(sub + 1, startp, bytes);
	startp += length;
	*startp = 0;
	return sub;
}



size_t UTF8String_search(const String* search, const String* pattern, size_t start, Memory* memory) {
	if (!search || !pattern || search->length <= pattern->length + start) return -1;
	size_t selfIndex = 0;
	byte_t* selfBuffer = (byte_t*)(search + 1);
	byte_t* tokenBuffer = (byte_t*)(pattern + 1);
	if (search->bytes == search->length && pattern->bytes== pattern->length) {
		return String_sunday8(selfBuffer, search->length, tokenBuffer,pattern->length,memory);
	}
	lchar_t selfCh = 0; lchar_t tokenCh;
	for (size_t i = 0, j = search->length - pattern->length; i < j; i++) {
		size_t selfCost = UTF8_convertToUTF32(selfBuffer, (lchar_t* const)&selfCh);
		if (selfCost == 0) return -1;
		byte_t* selfp = (selfBuffer += selfCost); byte_t* tokenp = tokenBuffer;
		for (size_t k = 0; k < pattern->length; k++) {
			size_t tokenCost = UTF8_convertToUTF32(tokenp, (lchar_t* const)&tokenCh);
			if (tokenCh != selfCh) {
				selfp = 0; break;
			}
			selfCost = UTF8_convertToUTF32(selfp, (lchar_t* const)&selfCh);
			selfp += selfCost;
		}
		if (selfp) return selfIndex;
		selfIndex++;

	}
	return selfIndex;
}

size_t UTF16String_search(const String* search, const String* pattern, size_t start, Memory* memory) {
	if (!search || !pattern || search->length <= pattern->length + start) return -1;
	size_t selfIndex = 0;
	byte_t* selfBuffer = (byte_t*)(search + 1);
	byte_t* tokenBuffer = (byte_t*)(pattern + 1);
	if (search->bytes == search->length && pattern->bytes == pattern->length) {
		return String_sunday16(selfBuffer, search->length, tokenBuffer, pattern->length, memory);
	}
	lchar_t selfCh = 0; lchar_t tokenCh;
	for (size_t i = 0, j = search->length - pattern->length; i < j; i++) {
		size_t selfCost = UTF16_convertToUTF32(selfBuffer, (lchar_t* const)&selfCh);
		if (selfCost == 0) return -1;
		byte_t* selfp = (selfBuffer += selfCost); byte_t* tokenp = tokenBuffer;
		for (size_t k = 0; k < pattern->length; k++) {
			size_t tokenCost = UTF16_convertToUTF32(tokenp, (lchar_t* const)&tokenCh);
			if (tokenCh != selfCh) {
				selfp = 0; break;
			}
			selfCost = UTF8_convertToUTF32(selfp, (lchar_t* const)&selfCh);
			selfp += selfCost;
		}
		if (selfp) return selfIndex;
		selfIndex++;

	}
	return selfIndex;
}


size_t UTF32String_search(const String* search, const String* pattern, size_t start, Memory* memory) {
	return String_sunday32((UTF32)(search+1), search->length, (UTF32)(pattern + 1), pattern->length, memory);
}






void getKMPNext(size_t* next, const char_t* p, size_t length) {

	next[0] = -1;
	size_t j = 0;
	size_t k = -1;
	while (j < length - 1) {
		//p[k]表示前缀，p[j]表示后缀
		if (k == -1 || p[j] == p[k]) {
			//较之前next数组求法，改动在下面4行
			if (p[++j] == p[++k]) {
				next[j] = next[k];// 当两个字符相等时要跳过
			}
			else {
				next[j] = k;//之前只有这一行
			}
		}
		else {
			k = next[k];
		}
	}
}

int String_searchKMP(const String* search, const String* pattern, size_t start,Memory* memory) {
	const size_t slength = search->length;
	if (pattern == 0 || slength == 0 || start >= slength) return -1;
	const size_t plength = pattern->length;

	if (plength + start > slength) return -1;
	if (plength == 0) return start;
	const char_t* s = (char_t*)((char*)search + sizeof(String)) + start;
	const char_t* p = (char_t*)((char*)pattern + sizeof(String));

	// KMP算法
	//分配next空间
	if (!memory) memory = Memory_default();
	size_t* next = (size_t*)memory->require(memory, plength * sizeof(size_t), 0);
	getKMPNext(next, p, plength);
	size_t i = 0; // 主串的位置
	size_t j = 0; // 模式串的位置

	while (i < slength && (j == -1 || j < plength)) {
		//①如果j=-1，或者当前字符匹配成功（即S[i]==P[j]），都令i++，j++
		if (j == -1 || s[i] == p[j]) { // 当j为-1时，要移动的是i，当然j也要归0
			i++;
			j++;
		}
		else {
			//②如果j!=-1，且当前字符匹配失败（即S[i]!=P[j]），则令i不变，j=next[j]，j右移i-next[j]
			j = next[j];
		}
	}
	memory->release(memory, next);
	return j == plength ? i - j + start : -1;

}
void String_cout(const String* self) {
	byte_t* buffer = (byte_t*)(self +1);
	printf_s("%s",buffer);
}
void String_coutln(const String* self) {
	byte_t* buffer = (byte_t*)(self + 1);
	printf_s("%s\n", buffer);
	
}


int String_compare(const String* left, const String* right) {
	if (left == right) return 0;
	if (left) {
		if (right) {
			size_t c = left->length;
			if (c > right->length) c = right->length;
			byte_t* leftBuffer = (byte_t*)((char*)left + sizeof(String));
			byte_t* rightBuffer = (byte_t*)((char*)right + sizeof(String));
			for (int i = 0, j = c; i < j; i++) {
				if (*leftBuffer > * rightBuffer) return 1;
				if (*leftBuffer < *rightBuffer) return -1;
				leftBuffer++; rightBuffer++;
			}
			if (left->length == right->length) return 0;
			return left->length > right->length?1:-1;
		}
		else return 1;
	}
	else {
		return -1;
	}
}



