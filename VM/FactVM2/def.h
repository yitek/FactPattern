#pragma once
#ifndef __DEF_INCLUDED__ 
#define __DEF_INCLUDED__


typedef char* addr_t;
typedef char byte_t;
typedef unsigned short char_t;
#ifdef _WIN64
typedef long word_t;
typedef unsigned long size_t;
typedef long pointer_t;
#else
typedef int word_t;
typedef unsigned int size_t;
typedef int pointer_t;
#endif


#endif
