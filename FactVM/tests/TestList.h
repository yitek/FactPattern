
/*****************************************************
*
* author:yiy
*
* description: ������Ĳ��Դ���
*
******************************************************/

#pragma once
#include "../types/Array.h"
#ifndef __TESTARRAY_INCLUDED__ 
#define __TESTARRAY_INCLUDED__
// c�������Ͻ�cpp�ı�׼�÷�,ָ������ķ��Ű���c���Է�ʽ����
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct {
		int no;
		char_t* name;
		int age;
	}ArrayStudent;

	void testArray();

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TESTARRAY_INCLUDED__



