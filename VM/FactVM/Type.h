#pragma once
#include <stddef.h>
struct TType;
struct TTypeMember;
struct TAny {
	struct TType* type;
};
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
	struct TAny;
} Type;
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
	struct TAny;
} TypeMember;





