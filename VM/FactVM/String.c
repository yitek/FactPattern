#pragma once
#include "String.h"
#include "MAllocator.h"
#include <string.h>
#include <stdio.h>
String* String_empty = 0;
String* String_construct(String* self,const wchar_t* initString, size_t count,MAllocator* mallocator) {
	if (!mallocator)mallocator = MAllocator_default();
	if (initString) {
		if (count == 0) {
			const wchar_t* bf = initString;
			while (*bf++) { count++; }
		}
	}
	if (count == 0) {
		self = String_empty;
	}
	else {
		if(!self)self = (String*)mallocator->require(mallocator, sizeof(struct TStr) + count * sizeof(wchar_t)+1);
		self->length = count;
		wchar_t* buffer = (void*)String_buffer(self);
		if (initString) {
			memcpy(buffer, initString, count * sizeof(wchar_t));
		}
		buffer += count;
		*buffer = 0;
	}
	return self;
	
}
void String_destruct(String* self) {
	if (self->length) {
		MAllocator_release(MAllocator_default(), self);
	}
}

const String* String_concat(const String* left,const String* right) {
	if (right == 0) return left;
	if (!left || !left->length) return right;
	if (!right || !right->length) return right;
	size_t leftLen = left->length, rightLen = right->length;
	size_t count = leftLen + rightLen;
	size_t concatedSize = sizeof(struct TStr) + (count+1) * sizeof(wchar_t);
	//if (concatedSize < sizeof(String)) concatedSize = sizeof(String);
	String* concatedString = (String*)MAllocator_require(MAllocator_default(), concatedSize);
	concatedString->length = count;
	char* buffer = ((char*)concatedString) + sizeof(struct TStr);
	memcpy(buffer,(char*)left + sizeof(struct TStr),leftLen * sizeof(wchar_t));
	buffer += leftLen * sizeof(wchar_t);
	memcpy(buffer, (char*)right + sizeof(struct TStr), rightLen* sizeof(wchar_t));
	buffer += rightLen;
	*buffer = 0;
	return concatedString;
}

const String* String_substr(const String* str, const size_t start, const size_t count) {
	size_t len = str->length;
	if (start >= len) return 0;
	size_t c = count==-1?len-start:count;
	if (len - start > c) c = len - start;
	if (c == 0)return String_empty;
	size_t size = sizeof(struct TStr) +(c+1) * sizeof(wchar_t);
	//if (size < sizeof(String)) size = sizeof(String);
	String* substr = (String*)MAllocator_require(MAllocator_default(), size);
	substr->length = c;
	const wchar_t* src = (const wchar_t*)((char*)str + sizeof(struct TStr)) + start;
	wchar_t* dest =(wchar_t*) ((char*)substr + sizeof(struct TStr));
	memcpy((void*)dest,(void*)src,c*sizeof(wchar_t));
	dest += c;
	*dest = 0;
	return (String*)substr;
}

void getKMPNext(size_t* next,const wchar_t* p,size_t length) {
	
	next[0] = -1;
	size_t j = 0;
	size_t k = -1;
	while (j <length - 1) {
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

int String_search(const String* search, const String* pattern, size_t start) {
	const size_t slength = search->length;
	if (pattern ==0 || slength ==0 || start >= slength) return -1;
	const size_t plength = pattern->length;
	
	if (plength + start > slength) return -1;
	if (plength == 0) return start;
	const wchar_t* s = (wchar_t*)((char*)search + sizeof(struct TStr)) + start;
	const wchar_t* p = (wchar_t*)((char*)pattern + sizeof(struct TStr));
	
	// KMP算法
	//分配next空间
	size_t* next = (int*)MAllocator_require(MAllocator_default(), plength * sizeof(size_t));
	getKMPNext(next,p,plength);
	size_t i = 0; // 主串的位置
	size_t j = 0; // 模式串的位置
	
	while (i < slength && (j==-1 ||j < plength)) {
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
	return j == plength ? i - j + start : -1;

}

void String_print(String* self) {
	wchar_t* buffer = (wchar_t*)((char*)self + sizeof(struct TStr));
	for (size_t i = 0,j = self->length; i < j; i++) putwchar(*buffer++);
}
