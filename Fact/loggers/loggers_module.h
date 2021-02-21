/*****************************************************
*
* author:yiy
*
* description: 日志模块初始化
*
******************************************************/

#pragma once

#include "TLogger.h"
#ifndef __LOGGERS_INCLUDED__ 
#define __LOGGERS_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	void loggers_module(void* memory, void* vm, void* param);
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __LOGGERS_INCLUDED__
