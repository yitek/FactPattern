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
	const char_t* s = (char_t*)((char*)search + sizeof(String)) + start;
	const char_t* p = (char_t*)((char*)pattern + sizeof(String));
	
	// KMP算法
	//分配next空间
	size_t* next = (int*)Memory_require(Memory_default(), plength * sizeof(size_t),0);
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

