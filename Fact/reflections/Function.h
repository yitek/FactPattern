/*****************************************************
*
* author:yiy
*
* description: 函数类

*
******************************************************/
#pragma once
#include "../types/TType.h"
#include "../types/TString.h"


#ifndef __FUNCTION_INCLUDED__ 
#define __FUNCTION_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif


	typedef enum {
		ParameterDirection_In = 1,
		ParameterDirection_Out = 2,
		ParameterDirection_Ref = 3,
		ParameterDirection_HasDefault = 4,
		ParameterDirection_InWithDefault = 5
	}ParameterDirections;

	typedef struct stParameter {
		String* name;
		Type* parameterType;
		ParameterDirections direction;
		void* defaultValue;

	}Parameter;

	typedef struct stTParameter {
		struct stGCObject;
		struct stTypeParameter;
	}TParameter;

	typedef enum {
		FunctionKind_Sync = 0,
		FunctionKind_VM = 1,		// 00 01
		FunctionKind_C = 2,			// 00 10
		FunctionKind_Cpp = 3,		// 00 11
		FunctionKind_Async = 4,		// 01 00
		FunctionKind_AsyncVM = FunctionKind_VM | FunctionKind_Async,
		FunctionKind_AsyncC = FunctionKind_C | FunctionKind_Async,
		FunctionKind_AsyncCpp = FunctionKind_Cpp | FunctionKind_Async,

	}FunctionKinds;

	typedef struct stFunction {
		struct stType;
		FunctionKinds funcKind;
		Type* returnType;
		Array* parameters;

		/// <summary>
		/// 汇编代码入口
		/// </summary>
		void* asmEntry;
		size_t vmEntry;
	}Function;


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __FUNCTION_INCLUDED__



