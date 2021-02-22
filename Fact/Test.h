/*****************************************************
*
* author:yiy
*
* description: 测试可u类
*
******************************************************/

#pragma once

#include "runtime.h"
#ifndef __TEST_INCLUDED__ 
#define __TEST_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	extern TLogger* Test__logger;
	void Test_begin(const byte_t* category, const byte_t* message, ...);
	void Test_end();
	void Test_assert(const byte_t* category, bool_t condition, const byte_t* message, ...);

#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TEST_INCLUDED__
