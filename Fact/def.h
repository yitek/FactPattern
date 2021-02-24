/*****************************************************
* 
* author:yiy
* 
* description: 公用定义文件
* 定义一些常用数据类型
* 32位或64位编译能够用相同的源代码
* 定义了TObject（对象） TType(类型)与虚表等的内存布局
* 
******************************************************/

#pragma once

// 用预处理标签来处理头文件的交叉引用
// 实现多次引用该头文件时，头文件的内容在经过预处理后的送编译器的源代码中只出现一次
// 其结构为 #ifndef __Some_Mark
//          #define __some_Mark
//         #endif
#ifndef __DEF_INCLUDED__ 
#define __DEF_INCLUDED__
// 排除 utf8警告
// #pragma warning(disable:4819)
// 排除enum class警告
// #pragma warning(disable:26812)
//是否是开发过程
#define __DEVALOPMENT__



#if defined(_WIN64) || __SIZEOF_POINTER__ == 8
#define __64BITS__
#elif defined(_WIN16) || defined(__MSDOS__)
#define __16BITS__
#else 
#define __32BITS__
#endif

#if defined(_WIN32)
#define __WIN__
#endif

#if defined(__linux__)
#define __LINUX__
#endif

#if defined(__64BITS__)
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef unsigned long long addr_t;

typedef int favor_t;
typedef unsigned int ufavor_t;
typedef long long word_t;
typedef unsigned long long uword_t;
typedef long long dword_t[2];
typedef unsigned long long udword_t[2];
typedef unsigned long long usize_t;

#elif defined(__32BITS__)
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef unsigned int addr_t;

typedef int favor_t;
typedef unsigned int ufavor_t;
typedef int word_t;
typedef unsigned int uword_t;
typedef long long dword_t;
typedef unsigned long long udword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#else // else16
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef long long large_t[2];
typedef unsigned long long ularge_t[2];
typedef unsigned int addr_t;
typedef short favor_t;
typedef unsigned short ufavor_t;
typedef short word_t;
typedef unsigned short uword_t;
typedef int dword_t;
typedef unsigned int udword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#endif // endif _WIN64

#if defined(__WIN__)
typedef unsigned short schar_t;
#elif defined(__LINUX__)
typedef unsigned int schar_t;
#endif
typedef char ch_t;
typedef char byte_t;
typedef char tiny_t;
typedef unsigned char utiny_t;
typedef short short_t;
typedef unsigned short ushort_t;
typedef int int_t;
typedef unsigned int uint_t;
typedef unsigned int unichar_t;
typedef word_t bool_t;



	
/// <summary>
/// 虚函数表
/// </summary>

typedef struct stVFTL {
	addr_t offset;
}VFTL;

typedef struct stVirtualObject {
	/// <summary>
		/// 虚函数表指针
		/// </summary>
	VFTL* __vftptr__;
} VirtualObject;

typedef enum {
	MemoryUnitKind_link,
	MemoryUnitKind_ref
} MemoryUnitKinds;

typedef struct stMLnkUnit {
	struct stMLnkUnit* next;
}MLnkUnit;

typedef struct stMRefUnit {
	usize_t __ref__;
}MRefUnit;

struct stTType;



typedef struct stClazzMeta {
	struct stVFTL;
	struct stTType* (*get_type)();

} ClazzMeta;

typedef struct stTObject {
	/// <summary>
		/// 对象信息指针(结构跟虚函数表一样，只是第一个虚函数固定成get_type函数)
		/// </summary>
	struct stClazzMeta* __meta__;

}TObject;

typedef struct stMTObjUnit {
	usize_t ref;
	struct stTObject;
} MTObjUnit;


typedef struct stTArray {
	struct stTObject;
	usize_t length;
}TArray;

typedef struct stTString {
	struct stTObject;
	usize_t length;
	usize_t bytes;
}TString;

typedef enum {
	TypeKind_func=			0b0000001,
	TypeKind_struct =		0b0000010,
	TypeKind_class=			0b0000100, 
	TypeKind_interface =	0b0001000
}TypeKinds;

typedef struct stTType {
	struct stTObject;
	TString* name;
	usize_t size;
	uword_t decorators;
	struct stTType* base;
	TArray* mixins;
	TArray* interfaces;
	TArray* fields;
	TArray* methods;
	TArray* properties;
	void* extras;
}TType;
typedef enum {
	MemberType_field =		0b01,
	MemberType_method =		0b10,
	MemberType_property =	0b11
} MemberTypes;

typedef enum {
	AccessLevel_private = 0,
	AccessLevel_protected =			0b00100000000,
	AccessLevel_internal =			0b01000000000,
	AccessLevel_protected_internal =0b01100000000,
	AccessLevel_public=				0b11100000000
} AccessLevels;

typedef struct stTMember {
	struct stTObject;
	TString* name;
	uword_t decorators;
	TType* type;
}TMember;

typedef struct stTField {
	struct stTMember;
	usize_t offset;
}TField;



static inline TType* get_type(TObject* obj) {
	return (TType*)obj->__meta__->get_type();
}
static inline void* ref_increase(void* obj) { return (((MRefUnit*)obj-1)->__ref__++,obj); }
#define ref_inc(obj) (((MRefUnit*)obj-1)->__ref__++,obj)
static inline void* ref_decrease(void* obj) { return (--(((MRefUnit*)obj - 1)->__ref__) >= 0 ? obj : ((((MRefUnit*)obj - 1)->__ref__=0,obj))); }
#define ref_dec(obj) (--(((MRefUnit*)obj - 1)->__ref__) >= 0 ? obj : ((((MRefUnit*)obj - 1)->__ref__=0,obj)))

#define Object_param(obj) ( (*((MTObjUnit*)obj-1)).__ref__++,obj )
#define Object_assign(dest,obj) ( (*((MTObjUnit*)(dest=obj)-1)).__ref__++,obj )
#define Object_release(obj) ( --(*((MTObjUnit*)obj-1)).__ref__<=0?(obj=0):obj )

#endif // end ifndef __DEF_INCLUDED__
