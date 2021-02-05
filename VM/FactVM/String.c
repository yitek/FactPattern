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
		//p[k]��ʾǰ׺��p[j]��ʾ��׺
		if (k == -1 || p[j] == p[k]) {
			//��֮ǰnext�����󷨣��Ķ�������4��
			if (p[++j] == p[++k]) {
				next[j] = next[k];// �������ַ����ʱҪ����
			}
			else {
				next[j] = k;//֮ǰֻ����һ��
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
	
	// KMP�㷨
	//����next�ռ�
	size_t* next = (int*)MAllocator_require(MAllocator_default(), plength * sizeof(size_t));
	getKMPNext(next,p,plength);
	size_t i = 0; // ������λ��
	size_t j = 0; // ģʽ����λ��
	
	while (i < slength && (j==-1 ||j < plength)) {
		//�����j=-1�����ߵ�ǰ�ַ�ƥ��ɹ�����S[i]==P[j]��������i++��j++
		if (j == -1 || s[i] == p[j]) { // ��jΪ-1ʱ��Ҫ�ƶ�����i����ȻjҲҪ��0
			i++;
			j++;
		}
		else {
			//�����j!=-1���ҵ�ǰ�ַ�ƥ��ʧ�ܣ���S[i]!=P[j]��������i���䣬j=next[j]��j����i-next[j]
			j = next[j];
		}
	}
	return j == plength ? i - j + start : -1;

}

void String_print(String* self) {
	wchar_t* buffer = (wchar_t*)((char*)self + sizeof(struct TStr));
	for (size_t i = 0,j = self->length; i < j; i++) putwchar(*buffer++);
}
