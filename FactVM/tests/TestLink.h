
#pragma once
/*****************************************************
*
* author:yiy
*
* description: �ڴ����������Ĳ��Դ���
*
******************************************************/

#pragma once
#include "../types/Link.h"
#ifndef __TESTLINK_INCLUDED__ 
#define __TESTLINK_INCLUDED__
// c�������Ͻ�cpp�ı�׼�÷�,ָ������ķ��Ű���c���Է�ʽ����
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



