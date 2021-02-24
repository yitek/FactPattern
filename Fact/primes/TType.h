/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../runtime.h"
#ifndef __TTYPE_INCLUDED__ 
#define __TTYPE_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTType TType;
	typedef struct stTMethod{
		struct stTMember;
		void* entry;
	}TMethod;

	typedef struct stTProperty {
		struct stTMember;
		void* getter;
		void* setter;
	}TProperty;

	

	
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TTYPE_INCLUDED__
