#pragma once
#include "String.h"
#include "../memories/Memory.h"
#include <stdio.h>

const String* String_emptyInstance = 0;


const String* String_construct(String* self, const byte_t* buffer, size_t byteCount, void* mmArgs, Memory* memory) {
	size_t charCount = -1;
	if (!byteCount) {
		if (buffer) {
			StringCountResult c = String_countUtf8((const byte_t*const)buffer);
			byteCount = c.byteCount;
			charCount = c.charCount;
		}
	}
	if (!self) {
		if (!memory) memory = Memory_default();
		self = memory->require(memory, byteCount + sizeof(String), mmArgs);
	}
	if (byteCount != 0 && byteCount != -1) {
		Memory_copy(self + 1, buffer, byteCount);
		self->bytes = byteCount;
	}
	if (charCount != -1) self->length = charCount;
	return self;
}

const String* String_concat(const String* left, const String* right, void* mmArgs, Memory* memory) {
	// �ұߵ�û�У�������ߵ�
	if (right == 0 || right->length == 0) return left;
	// ��ߵ�û�У������ұߵ�
	if (!left || !left->length) return right;
	size_t leftLen = left->bytes, rightLen = right->bytes;
	// �ܳ��ȵ�����߳���+�ұ߳���
	size_t count = leftLen + rightLen;
	if (count == 0 && String_emptyInstance) return String_emptyInstance;
	size_t concatedSize = count + sizeof(String);
	// �ڴ��С���ڳ��ȳ˵�Ԫ��С

	// ����һ��������
	if (!memory)memory = Memory_default();
	String* concatedArray = memory->require(memory, concatedSize+1, mmArgs);
	concatedArray->length = left->length + right->length;
	concatedArray->bytes = left->bytes + right->bytes;

	// ��ȡ������Ԫ�صĿ�ʼλ��
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//������ߵ������Ԫ��
	Memory_copy(buffer, (char*)left + sizeof(Array), leftLen);
	// �ƶ�����ָ�뵽β��
	buffer += leftLen ;
	// �����ұߵ�Ԫ��
	Memory_copy(buffer, (char*)right + sizeof(Array), rightLen);
	buffer += rightLen;
	*buffer = 0;
	// �������Ӻ������
	return (const String*)concatedArray;
}

const String* String_substr(const String* arr, const size_t start, size_t length, void* mmArgs, Memory* memory) {
	if (!arr || arr->length <= start)return 0;
	if (length == 0) {
		if(String_emptyInstance)return String_emptyInstance;
		return String_construct(0, 0, 0, mmArgs, memory);

	}
	size_t indexEnd = start + length;
	if (indexEnd >= arr->length) { indexEnd = arr->length; length = arr->length - start; }
	const utf8char_t* p = ( const utf8char_t*)(arr + 1);
	wchar_t c = 0;
	utf8char_t* pstart = 0; size_t byteCount = 0; size_t byteCost = 0;
	while (byteCost = String_convertUtf8To32(p, (utf32char_t* const)&c)) {
		if (c == start) {
			pstart = (utf8char_t*)p;
		}
		else if (pstart) {
			byteCount += byteCost;
		}
		p += byteCost;
		c++;
		if (c == indexEnd)break;
	}
	if (!memory) memory = Memory_default();
	String* sub = (String*)memory->require(memory, sizeof(String) + byteCount + 1,mmArgs);
	sub->length = length;
	sub->bytes = byteCount;
	Memory_copy(sub+1,pstart,byteCount);
	pstart += byteCount;
	if(pstart)*pstart = 0;
	return sub;
}
size_t String_search(const String* self, const String* token, size_t start, Memory* memory) {
	if (!self || !token || self->length <= token->length + start) return -1;
	size_t selfIndex = 0;
	byte_t* selfBuffer = (byte_t*)(self + 1);
	byte_t* tokenBuffer = (byte_t*)(self + 1);
	lchar_t selfCh = 0; lchar_t tokenCh;
	for (size_t i = 0, j = self->length - token->length; i < j; i++) {
		size_t selfCost = String_convertUtf8To32(selfBuffer, (lchar_t* const)&selfCh);
		if (selfCost == 0) return -1;
		byte_t* selfp = (selfBuffer += selfCost); byte_t* tokenp = tokenBuffer;
		for (size_t k = 0; k < token->length; k++) {
			size_t tokenCost = String_convertUtf8To32(tokenp, (lchar_t* const)&tokenCh);
			if (tokenCh != selfCh) {
				selfp = 0; break;
			}
			selfCost = String_convertUtf8To32(selfp, (lchar_t* const)&selfCh);
			selfp += selfCost;
		}
		if (selfp) return selfIndex;
		selfIndex++;

	}
	return selfIndex;
}

size_t Sunday(String* search,String* pattern,Memory* mm) {
	
	size_t n = search->length;
	size_t m = pattern->length;
	byte_t* sbuffer = (byte_t*)(search + 1);
	byte_t* pbuffer = (byte_t*)(pattern+1);

	// ģʽ����ʼλ��������������
	size_t s = 0;
	size_t index = 0;
	// ģʽ���Ѿ�ƥ�䵽��λ��
	size_t j;
	while (s <= n - m) {
		j = 0;
		while (sbuffer[s + j] == pbuffer[j]) {
			j++;
			// ƥ��ɹ�
			if (j >= m) {
				return s;// return s;������utf8����Ҫת��һ��index
			}
		}
		// �ҵ������е�ǰ��ģʽ��ƥ�����ĩ�ַ�����һ���ַ�
		// ��ģʽ���г�������λ��
		// ����Ҫ��(ģʽ��ĩβ+1)�ƶ�����λ�õĲ���
		//Ĭ����m+1
		size_t shiftCount = m+1;
		byte_t c = sbuffer[s + m];
		for (size_t k = 0,q= pattern->bytes; k <q; k++) {
			if (pbuffer[k] == c) {
				shiftCount = m-k;
				break;
			}
		}

		s += shiftCount;
	}
	return -1;
}


void getKMPNext(size_t* next, const char_t* p, size_t length) {

	next[0] = -1;
	size_t j = 0;
	size_t k = -1;
	while (j < length - 1) {
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

int String_searchKMP(const String* search, const String* pattern, size_t start,Memory* memory) {
	const size_t slength = search->length;
	if (pattern == 0 || slength == 0 || start >= slength) return -1;
	const size_t plength = pattern->length;

	if (plength + start > slength) return -1;
	if (plength == 0) return start;
	const char_t* s = (char_t*)((char*)search + sizeof(String)) + start;
	const char_t* p = (char_t*)((char*)pattern + sizeof(String));

	// KMP�㷨
	//����next�ռ�
	if (!memory) memory = Memory_default();
	size_t* next = (size_t*)memory->require(memory, plength * sizeof(size_t), 0);
	getKMPNext(next, p, plength);
	size_t i = 0; // ������λ��
	size_t j = 0; // ģʽ����λ��

	while (i < slength && (j == -1 || j < plength)) {
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
	memory->release(memory, next);
	return j == plength ? i - j + start : -1;

}
void String_cout(const String* self) {
	char_t* buffer = (wchar_t*)((char*)self + sizeof(String));
	for (size_t i = 0, j = self->length; i < j; i++) putwchar(*buffer++);
}
void String_coutln(const String* self) {
	char_t* buffer = (wchar_t*)((char*)self + sizeof(String));
	for (size_t i = 0, j = self->length; i < j; i++) putwchar(*buffer++);
	putwchar(L'\n');
}


int String_compare(const String* left, const String* right) {
	if (left == right) return 0;
	if (left) {
		if (right) {
			size_t c = left->length;
			if (c > right->length) c = right->length;
			char_t* leftBuffer = (char_t*)((char*)left + sizeof(String));
			char_t* rightBuffer = (char_t*)((char*)right + sizeof(String));
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



