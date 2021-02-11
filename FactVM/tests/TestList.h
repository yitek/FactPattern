
/*****************************************************
*
* author:yiy
*
* description: 链表类的测试代码
*
******************************************************/

#pragma once
#include "../types/Array.h"
#ifndef __TESTARRAY_INCLUDED__ 
#define __TESTARRAY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
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



