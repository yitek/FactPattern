/*****************************************************
*
* author:yiy
*
* description: 内存模块初始化
*
******************************************************/

#pragma once

#include "TMemory.h"
#include "TAlignedMemory.h"
#include "TGCMemory.h"

#ifndef __MEMORIES_INCLUDED__ 
#define __MEMORIES_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	void memories_module(void* memory, void* vm, void* param);
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __MEMORIES_INCLUDED__
