/*****************************************************
*
* author:yiy
*
* description: Ä£¿éÀà

*
******************************************************/
#pragma once

#include "Class.h"
#include "../types/TList.h"


#ifndef __MODULE_INCLUDED__ 
#define __MODULE_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct stModule {
		String* name;
		String* filename;
		List* types;
	}stModule;


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __MODULE_INCLUDED__



