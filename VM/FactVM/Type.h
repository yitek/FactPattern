#pragma once
#include "List.h"
#include "String.h"

#ifndef __TYPE_INCLUDED__ 
#define __TYPE_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif

	struct TType;
	struct TTypeMember;
	typedef struct TGCObject {
		struct TType* type;
		size_t ref;
	} GCObject;



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
	typedef struct  TType {
		void* name;
		TypeKinds kind;
		size_t size;
		size_t memberCount;
		size_t methodCount;
		//接 methodCount个指针
		// 后面接sizeof(MemberObject)*memberCount;
		// 后面接字符串
	} Type;
	typedef struct TTypeObject {
		struct TGCObject;
		struct TType;
	} TypeObject;
	typedef enum {
		MemberType_Field,
		MemberType_Property,
		MemberType_Method,
		MemberType_Constructor,
		MemberType_Type
	} TypeMemberKinds;
	typedef struct TTypeMember {
		void* name;
		size_t offset;
		TypeMemberKinds kind;
		struct TType* memberType;
	} TypeMember;
	typedef struct TTypeMemberObject {
		struct TGCObject;
		struct TTypeMember;
	}TypeMemberObject;

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif







