
#pragma once
/*****************************************************
*
* author:yiy
*
* description: 内存管理器基类的测试代码
*
******************************************************/

#pragma once
#include "../../memories/memories_module.h"
#include "../../Test.h"
#ifndef __TESTMEMORY_INCLUDED__ 
#define __TESTMEMORY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif

	struct stUserForTest;
	struct stUserProfileForTest;
	typedef struct stUserForTest {
		struct stTObject;
		int id;
		struct stUserProfileForTest* profile;

	}TUserForTest;

	struct stUserForTestMeta {
		struct stClazzMeta;
	};
	struct stUserForTestMeta UserForTest__meta__;
	
	

	typedef struct stUserProfileForTest {
		struct stTObject;
		int age;
		struct stUserForTest* user;
	}TUserProfileForTest;



	struct stUserProfileForTestMeta {
		struct stClazzMeta;
	};



	struct stUserProfileForTestMeta UserProfileForTest__meta__;
	

	void testMemory();
	void testAlignedMemory();

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TESTMEMORY_INCLUDED__



