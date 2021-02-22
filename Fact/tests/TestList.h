
/*****************************************************
*
* author:yiy
*
* description: 链表类的测试代码
*
******************************************************/

#pragma once
#include "../ads/List.h"
#ifndef __TESTLIST_INCLUDED__ 
#define __TESTLIST_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct {
		word_t no;
		char* name;
		int age;
	}ListStudent;

	void testList();

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TESTLIST_INCLUDED__



