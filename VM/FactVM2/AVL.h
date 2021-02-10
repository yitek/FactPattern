#pragma once
#pragma once
#include "List.h"
#ifndef __AVL_INCLUDED__ 
#define __AVL_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stAVLNode {
		struct stAVLNode left;
		struct stAVLNode right;

	} AVLNode;

	

#ifdef __cplusplus 
}//extern "C" 
#endif

#endif
