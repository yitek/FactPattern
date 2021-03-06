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
	typedef struct stObjectInstance {
		struct stType* type;
		size_t ref;
	} ObjectInstance;

	inline ObjectInstance* objectInstance(const void* obj) {return ((ObjectInstance*)obj - 1);}



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
		Memory* memory;
		Array* generics;
		Array* members;
	} Type;
	typedef struct stTypeInstance {
		struct stObjectInstance;
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
	typedef struct stTypeMemberInstance {
		struct stObjectInstance;
		struct stTypeMember;
	}TypeMemberObject;

	typedef union {
		byte_t bytes[1];
		byte_t value;
	}Byte;

	typedef union {
		char_t value;
		byte_t bytes[2];
	}Char;

	typedef union unInt {
		byte_t bytes[4];
		int value;
	}Int;

	typedef union {
		union unInt;
	}Boolean;

	typedef union {
		union unInt;
	}Enum;

	typedef union {
		byte_t bytes[8];
		long value;
	}Long;

	typedef union {
		byte_t bytes[16];
		long long value;
	}Large;

	typedef union {
		byte_t bytes[8];
		double value;
	} Float;

	typedef struct {
		struct stArray;
	} TArray;
	const TArray* TArray___construct__(TArray* self, const void* buffer, const size_t count);
	const TArray* TArray_concat(const TArray* left, const TArray* right);
	const TArray* TArray_clip(const TArray* arr, const size_t start, const size_t length);
	inline const void* TArray___INDEXGETER__(const TArray* self, size_t index);
	inline const void* TArray_buffer(const TArray* self) { return (char*)self + sizeof(TArray); }

	typedef struct{
		struct stString;
	} TString;

	const TString* TString___construct__(TString* self, const char_t* buffer, size_t count);
	const TString* TString_concat(const TString* left, const TString* right);
	const TString* TString_clip(const TString* arr, const size_t start, const size_t length);
	inline const TString* TString_substr(const TString* arr, const size_t start, const size_t length) { return TString_clip(arr,start,length); }
	inline int TString_search(const TString* self, const TString* token, size_t at) { return String_search((const String*)self,(const String*)token,at); }
	const char_t* TString___INDEXGETER__(const TString* self, size_t index);
	inline const char_t* TString_buffer(const TString* self) { return (const char_t*)((char*)self + sizeof(String)); }

	typedef struct {
		struct stList;
	} TList;
	TList* TList___construct__(List* self);

	void* TList_append(TList* self);

	int TList_remove(TList* self, LinkPredicate predicate, void* param);

	TArray* TList_toArray(TList* self);
	

#ifdef __cplusplus 
}//extern "C" {
#endif
#endif







