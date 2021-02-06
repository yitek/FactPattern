#pragma once
#include <stddef.h>

#include "List.h"
#include "String.h"

struct TType;
struct TTypeMember;
struct TObject {
	struct TType* type;
	size_t ref;
} Object;



typedef enum  {
	AssignType_Unknwon,
	AssignType_ByVal,
	AssignType_GCRef,
	AssignType_ConstRef
}AssignTypes;
typedef enum  {
	Type_Struct,
	Type_Class,
	Type_Interface,
	Type_Enum,
	Type_Delegate
} TypeKinds;
typedef struct  TType {
	void* name;
	TypeKinds kind;
	AssignTypes assignType;
	size_t memberCount;
	size_t size;

	// 后面接sizeof(MemberObject)*memberCount;
	// 后面接字符串
} Type;
typedef struct TTypeObject {
	struct TObject;
	struct TType;
} TypeObject;
typedef enum  {
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
	struct TObject;
	struct TTypeMember;
}TypeMemberObject;







