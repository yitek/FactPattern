
/*****************************************************
*
* author:yiy
*
* description: 链表类的测试代码
*
******************************************************/

#pragma once
#include "../types/List.h"
#ifndef __TESTLIST_INCLUDED__ 
#define __TESTLIST_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct {
		int no;
		char_t* name;
		int age;
	}ListStudent;

	void testList();

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TESTLIST_INCLUDED__



