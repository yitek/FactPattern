
#pragma once
/*****************************************************
*
* author:yiy
*
* description: 内存管理器基类的测试代码
*
******************************************************/

#pragma once
#include "../types/Link.h"
#ifndef __TESTLINK_INCLUDED__ 
#define __TESTLINK_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct {
		struct stLink;
		int no;
		char_t* name;
		int age;
	}LinkStudent;

	void testLink();

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TESTLINK_INCLUDED__



