/*****************************************************
*
* author:yiy
*
* description: ¿‡–Õ¿‡

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

	typedef enum {
		AccessLevel_Private = 0,
		AccessLevel_Protected=1,
		AccessLevel_Internal = 1<<1,
		AccessLevel_Mixin = 1 << 2,
		AccessLevel_Public = 1<<3 | AccessLevel_Internal | AccessLevel_Protected
	}AccessLevels;


	typedef struct stType {
		Memory* memory;
		TypeKinds kind;
		size_t size;
		AccessLevels accessLevel;
		Array* generics;
	}Type;

	


	inline Type* TType_getGenericType(Type* self, size_t index) {
		return (Type*)Array___INDEXGETER__(self->generics, index, sizeof(Type*));
	}




#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TTYPE_INCLUDED__



