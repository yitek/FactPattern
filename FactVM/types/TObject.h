/*****************************************************
*
* author:yiy
*
* description: ¶ÔÏóÀà

*
******************************************************/
#pragma once
#include "TType.h"

#ifndef __TOBJECT_INCLUDED__ 
#define __TOBJECT_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif



	inline Type* TObject_getType(void* obj) {
		return obj?(Type*)getGCUnit(obj)->type:0;
	}

	inline Memory* TObject_getMemory(void* obj) {
		return obj ? ((Type*)getGCUnit(obj)->type)->memory : 0;
	}


	inline Type* TObject_getIndexType(void* obj,size_t index) {
		return obj?(index==-1?TObject_getType(obj):(Type*)Array_get(TObject_getType(obj)->generics, index, sizeof(Type*))):0;
	}




#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TOBJECT_INCLUDED__



