/*****************************************************
*
* author:yiy
*
* description: 数组基类
* 定义了 连接concat,切片clip操作
* 定义了属性 length
* 定义了下标访问器
*
******************************************************/

#pragma once
#include "Set.h"
#include "String.h"
#include "../memories/GCMemory.h"
#ifndef __TYPE_INCLUDED__ 
#define __TYPE_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	
	typedef struct stTString {
		struct stGCObject;
		struct stString;
	}TString;
	typedef struct stTList {
		struct stGCObject;
		struct stList;
	} TList;

	typedef struct stTSet {
		struct stGCObject;
		struct stSet;
	}TSet;

	

	typedef struct stTType {
		struct stGCObject;
		struct stType;
	}TType;


	typedef enum {
		ParameterDirection_In = 1,
		ParameterDirection_Out = 2,
		ParameterDirection_Ref = 3,
		ParameterDirection_HasDefault = 4,
		ParameterDirection_InWithDefault = 5
	}ParameterDirections;

	typedef struct stParameter {
		TString* name;
		TType* parameterType;
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
		FunctionKind_Async =4,		// 01 00
		FunctionKind_AsyncVM = FunctionKind_VM| FunctionKind_Async,
		FunctionKind_AsyncC = FunctionKind_C | FunctionKind_Async,
		FunctionKind_AsyncCpp = FunctionKind_Cpp | FunctionKind_Async,

	}FunctionKinds;
	typedef struct stFunction {
		struct stType;
		FunctionKinds funcKind;
		TType* returnType;
		TArray* parameters;
		
		/// <summary>
		/// 汇编代码入口
		/// </summary>
		void* asmEntry;
		size_t vmEntry;
	}Function;

	typedef struct stTFunction {
		struct stGCObject;
		struct stFunction;
	}TFunction;

	typedef struct stFunctionBuilder {
		struct stType;
		FunctionKinds funcKind;
		TType* returnType;
		List* parameters;
	}FunctionBuilder;



	typedef enum {
		/// <summary>
		/// 00 0 01
		/// </summary>
		MemberKind_Field = 1,
		/// <summary>
		/// 0 0 10
		/// </summary>
		MemberKind_Method = 2,
		/// <summary>
		/// 0 0 11
		/// </summary>
		MemberKind_Property = 3,
		/// <summary>
		/// 0 1 10
		/// </summary>
		MemberKind_Constructor = MemberKind_Method | 1 << 2,
		/// <summary>
		/// 1 0 00
		/// </summary>
		MemberKind_Virtual = 1 << 3,
		MemberKind_VirtualMethod = MemberKind_Virtual | MemberKind_Method,

		MemberKind_VirtualProperty = MemberKind_Property | MemberKind_Virtual
	}MemberKinds;


	typedef struct stMember {
		TString* name;
		MemberKinds kind;
		TType* memberType;
		union {
			size_t offset;
			Function* getter;
		};

		Function* setter;
	}Member;

	typedef struct stTMember {
		struct stGCObject;
		struct stMember;
	}TMember;

	typedef struct stStruct {
		struct stType;
		TArray* members;
	}Struct;
	typedef struct stTStruct {
		struct stGCObject;
		struct stStruct;
	}TStruct;

	typedef struct stStructBuilder {
		struct stType;
		TList* members;
	}TStruct;


	typedef struct stClass {
		struct stStruct;
		TArray* statics;//Array<Member>
	}Class;
	typedef struct stTClass {
		struct stGCObject;
		struct stClass;
	}TClass;

	typedef struct stModule {
		TString* name;
		TString* file;
		TArray* imports;
		TArray* exports;
		TArray* variables;
		TArray* types;
	}Module;

	typedef struct stModuleBuilder {
		TString* name;
		TString* file;
		TList* exports;
		TList* variables;
		TList* types;
	}ModuleBuilder; 


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TYPE_INCLUDED__



