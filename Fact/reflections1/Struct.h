/*****************************************************
*
* author:yiy
*
* description: 结构体类

*
******************************************************/
#pragma once

#include "Function.h"


#ifndef __STRUCT_INCLUDED__ 
#define __STRUCT_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif


	typedef enum {
		/// <summary>
		/// 0 0 0 01
		/// </summary>
		MemberKind_Field = 1,
		/// <summary>
		///0 0 0 10
		/// </summary>
		MemberKind_Method = 2,
		/// <summary>
		///0 0 0 11
		/// </summary>
		MemberKind_Property = 3,
		/// <summary>
		///0 0 1 10
		/// </summary>
		MemberKind_Constructor = MemberKind_Method | 1 << 2,
		/// <summary>
		///0 1 0 00
		/// </summary>
		MemberKind_Virtual = 1 << 3,
		MemberKind_VirtualMethod = MemberKind_Virtual | MemberKind_Method,

		MemberKind_VirtualProperty = MemberKind_Property | MemberKind_Virtual,
		/// <summary>
		/// 1 0 0 00
		/// </summary>
		MemberKind_Static = 1 << 4,
		MemberKind_StaticField = MemberKind_Static | MemberKind_Field,
		MemberKind_StaticMethod = MemberKind_Static | MemberKind_Method,
		MemberKind_StaticProperty = MemberKind_Static | MemberKind_Property,
	}MemberKinds;


	typedef struct stMember {
		String* name;
		MemberKinds kind;
		AccessLevels accessLevel;
		Type* memberType;
		union {
			size_t offset;
			Function* getter;
		};

		Function* setter;
	}Member;

	typedef struct stStruct {
		struct stType;
		Array* members;
		Type* baseType;
	}Struct;


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __STRUCT_INCLUDED__



