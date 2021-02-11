#pragma once
#include "String.h"
#include "Memory.h"

#include <string.h>
#include <stdio.h>




const String* String_empty = 0;


void getKMPNext(size_t* next,const char_t* p,size_t length) {
	
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
	const char_t* s = (char_t*)((char*)search + sizeof(String)) + start;
	const char_t* p = (char_t*)((char*)pattern + sizeof(String));
	
	// KMP�㷨
	//����next�ռ�
	size_t* next = (int*)Memory_require(Memory_default(), plength * sizeof(size_t),0);
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
void String_cout(const String* self) {
	char_t* buffer = (wchar_t*)((char*)self + sizeof(String));
	for (size_t i = 0, j = self->length; i < j; i++) putwchar(*buffer++);
}

int String___EQ__(const String* left, const String* right) {
	if (left == right) return 1;
	if (left) {
		if (right) {
			if (left->length != right->length) return 0;
			if (left->length == 0) return 1;
			char_t* leftBuffer = (char_t)((char*)left + sizeof(String));
			char_t* rightBuffer = (char_t)((char*)right + sizeof(String));
			for (size_t i = 0, j = left->length; i < j; i++) {
				if (*leftBuffer != *rightBuffer) return 0;
				leftBuffer++; rightBuffer++;
			}
			return 1;
		}
		else return 0;
	}
	else {
		if (right) return 0;
		return 0;
	}
}

int String_GreaterThan(const String* left, const String* right,int eq) {
	if (left) {
		if (right) {
			if (eq && left == right) return 1;
			size_t c = left->length;
			if (c > right->length) c = right->length;
			char_t* leftBuffer = (char_t)((char*)left + sizeof(String));
			char_t* rightBuffer = (char_t)((char*)right + sizeof(String));
			for (int i = 0, j = c; i < j; i++) {
				if (*leftBuffer > *rightBuffer) return 1;
				if (*leftBuffer < *rightBuffer) return 0;
				leftBuffer++; rightBuffer++;
			}
			if (eq && left->length == right->length) return 1;
			return left->length>right->length;
		}
		else return 1;
	}
	else {
		if (eq) return left == right;
		return 0;
	}
}

int String_LessThan(const String* left, const String* right,int eq) {
	if (left) {
		if (right) {
			if (eq && left == right) return 1;
			size_t c = left->length;
			if (c > right->length) c = right->length;
			char_t* leftBuffer = (char_t)((char*)left + sizeof(String));
			char_t* rightBuffer = (char_t)((char*)right + sizeof(String));
			for (int i = 0, j = c; i < j; i++) {
				if (*leftBuffer < * rightBuffer) return 1;
				if (*leftBuffer > *rightBuffer) return 0;
				leftBuffer++; rightBuffer++;
			}
			if (eq && left->length == right->length) return 1;
			return left->length < right->length;
		}
		else return 0;
	}
	else {
		if (eq) return left == right;
		return right?1:0;
	}
}

int String___GT__(const String* left, const String* right) { return String_GreaterThan(left,right,0); }
int String___LT__(const String* left, const String* right) { return String_LessThan(left, right, 0); }
int String___GTE__(const String* left, const String* right) { return String_GreaterThan(left, right, 1); }
int String___LTE__(const String* left, const String* right) { return String_LessThan(left, right, 1); }
