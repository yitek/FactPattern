/*****************************************************
*
* author:yiy
*
* description: ÀàÐÍÀà

*
******************************************************/
#pragma once
#include "../memories/GCMemory.h"
#include "../ads/Array.h"

#ifndef __TTYPE_INCLUDED__ 
#define __TTYPE_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum {
		TypeKind_Func,
		TypeKind_Struct,
		TypeKind_Class,
		TypeKind_Interface
	}TypeKinds;


	typedef struct stType {
		Memory* memory;
		TypeKinds kind;
		size_t size;
		Array* generics;
	}Type;




	inline Type* TType_getGenericType(Type* self, size_t index) {
		return (Type*)Array___INDEXGETER__(self->generics, index, sizeof(Type*));
	}




#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TTYPE_INCLUDED__