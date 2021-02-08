#pragma once
#include "List.h"
#include "String.h"

#ifndef __TYPE_INCLUDED__ 
#define __TYPE_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif

	struct stType;
	struct stTypeMember;
	typedef struct stObject {
		struct stType* type;
		size_t ref;
	} Object;



	typedef enum {
		AssignType_Unknwon,
		AssignType_ByVal,
		AssignType_GCRef,
		AssignType_ConstRef
	}AssignTypes;
	typedef enum {
		TypeKind_Struct,
		TypeKind_Class,
		TypeKind_Interface,
		TypeKind_Enum,
		TypeKind_Delegate
	} TypeKinds;
	typedef struct  stType {
		void* name;
		TypeKinds kind;
		size_t size;
		Array* members;
	} Type;
	typedef struct stTypeObject {
		struct stObject;
		struct stType;
	} TypeObject;
	typedef enum {
		MemberType_Field,
		MemberType_Property,
		MemberType_Method,
		MemberType_Constructor,
		MemberType_Type
	} TypeMemberKinds;
	typedef struct stTypeMember {
		void* name;
		size_t offset;
		TypeMemberKinds kind;
		struct stType* memberType;
	} TypeMember;
	typedef struct stTypeMemberObject {
		struct stObject;
		struct stTypeMember;
	}TypeMemberObject;

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif







