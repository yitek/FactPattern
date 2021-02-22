/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../memories/TMemory.h"
#ifndef __TTYPE_INCLUDED__ 
#define __TTYPE_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTMethod{
		struct stTMember;
		void* entry;
	}TMethod;

	typedef struct stTProperty {
		struct stTMember;
		void* getter;
		void* setter;
	}TProperty;

	//typedef struct stTTypeBuilder

	TType* TType__create(const char* name, usize_t mixins, usize_t interfaces, usize_t fields, usize_t methods, usize_t properties, TMemory* mm);
	TType* TType__setField(TType* type,usize_t index,const char* name,TType* fieldType,uword_t decorators,usize_t offset);
	
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TTYPE_INCLUDED__
