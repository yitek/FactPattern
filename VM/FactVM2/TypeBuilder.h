#pragma once
#include "type.h"
#include "Memory.h"

#ifndef __TYPEBUILDER_INCLUDED__ 
#define __TYPEBUILDER_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct {
		void* name;
		TypeKinds kind;
		size_t size;
		Memory* memory;
		List* generics;
		List* members;
	}TypeBuilder;
	TypeBuilder* TypeBuilder___construct__(TypeBuilder* self,void* mmArgs, Memory* memory) {
		if (!memory)memory = Memory_default();
		if (!self) self = (TypeBuilder*)memory->require(memory, sizeof(TypeBuilder), mmArgs);
		self->memory = memory;
		return self;
	}
	
#ifdef __cplusplus 
}//end extern c
#endif
#endif



