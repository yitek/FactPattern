/*****************************************************
*
* author:yiy
*
* description: 类型构造器
*
******************************************************/

#pragma once
#include "../primes/TString.h"
#include "../primes/TList.h"
#ifndef __TTYPEBUILDER_INCLUDED__ 
#define __TTYPEBUILDER_INCLUDED__


#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stTTypeBuilder {
		struct stTObject;
		TMemory* mm;
		MemoryKinds mkind;
		const struct TString* name;
		ufavor_t size;
		TypeKinds kind;
		struct stVFTL* vftptr;
		struct TList* genericRestricts;
		struct stTType* base;
		struct TList* mixins;
		struct TList* interfaces;
		struct TList* genericArguments;
		struct TList* fields;
		struct TList* methods;
		struct TList* properties;
		void* extras;
	}TTypeBuilder;

	TTypeBuilder* TTypeBuilder__construct__(TList* self, TMemory* memory, void* listType, MemoryKinds mkind);
#ifdef __cplusplus 
}//extern "C" 
#endif

#endif // __TTYPEBUILDER_INCLUDED__
