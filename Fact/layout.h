/*****************************************************
*
* author:yiy
*
* description: 内存布局文件
* 定义了TObject（对象） TType(类型)与虚表等的内存布局
*
******************************************************/

#pragma once
#include "def.h"

#ifndef __LAYOUT_INCLUDED__ 
#define __LAYOUT_INCLUDED__



typedef struct stVirtTableLayout {
	usize_t offset;
}VirtTableLayout;

typedef struct stVirtualLayout {
	/// <summary>
		/// 虚函数表指针
		/// </summary>
	VirtTableLayout* __virtp__;
} VirtualLayout;

struct stTType;

typedef struct stObjectMetaLayout {
	struct stVirtTableLayout;
	struct stTType*(*get_type)();
} ObjectMetaLayout;


typedef struct stTObject {
	/// <summary>
		/// 对象信息指针(结构跟虚函数表一样，只是第一个虚函数固定成get_type函数)
		/// </summary>
	struct stObjectMetaLayout* __meta__;
	
}TObject;

typedef struct stGCUnitLayout {
	usize_t ref;
}GCUnitLayout;

typedef struct stObjectLayout{
	usize_t ref;
	struct stObjectMetaLayout* __meta__;
} ObjectLayout;


typedef struct stTFieldList {
	struct stTObject;
	usize_t length;
}TFieldList;

typedef enum {
	TypeKind_func=			0b0000001,
	TypeKind_struct =		0b0000010,
	TypeKind_class=			0b0000100, 
	TypeKind_interface =	0b0001000
}TypeKinds;

typedef struct stTType {
	struct stTObject;
	usize_t size;
	uword_t decorators;
	TFieldList* fields;
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
	uword_t decorators;
	TType* type;
}TMember;

typedef struct stTField {
	struct stTMember;
	usize_t offset;
}TField;

static inline TType* get_type(TObject* obj) {
	return obj->__meta__->get_type();
}

#define Object_param(obj) ( (*((ObjectLayout*)obj-1)).ref++,obj )
#define Object_assign(dest,obj) ( (*((ObjectLayout*)(dest=obj)-1)).ref++,obj )
#define Object_release(obj) ( --(*((ObjectLayout*)obj-1)).ref<=0?(obj=0):obj )




static inline void m_copy(void* dest, const void* src, usize_t size) {

	if (size == sizeof(word_t)) {
		*((word_t*)dest) = *(word_t*)src;
		return;
	}
	usize_t wordc = size / sizeof(word_t);
	usize_t bytec = size % sizeof(word_t);

	if (wordc)for (usize_t i = 0; i < wordc; i++) {
		*((word_t*)dest) = *((word_t*)src);
		dest = ((word_t*)dest) + 1; src = ((word_t*)src) + 1;
	}
	if (bytec) for (usize_t i = 0; i < bytec; i++) {
		*((byte_t*)dest) = *((byte_t*)src);
		dest = ((byte_t*)dest) + 1; src = ((byte_t*)src) + 1;
	}

}

static inline void m_repeat(void* dest, usize_t times, void* value, usize_t size) {

	if (size == sizeof(word_t)) {
		word_t v = *((word_t*)value);
		word_t* p = ((word_t*)dest);
		for (usize_t i = 0; i < times; i++) { *p = v; ++p; }
	}
	else if (size == sizeof(word_t) * 2) {
		word_t v1 = *((word_t*)value);
		word_t v2 = *(((word_t*)value) + 1);
		word_t* p = ((word_t*)dest);
		for (usize_t i = 0; i < times; i++) { *p = v1; ++p; *p = v2; ++p; }
	}
	else if (size % sizeof(word_t)) {
		word_t* p = ((word_t*)dest);
		word_t* k = (word_t*)value;
		usize_t c = size / sizeof(word_t);
		for (usize_t i = 0; i < times; i++)
			for (usize_t j = 0; j < c; j++)*p = k[j];
	}
	else if (size == sizeof(short_t)) {
		short_t v = *((short_t*)value);
		short_t* p = ((short_t*)dest);
		for (usize_t i = 0; i < times; i++) { *p = v; ++p; }
	}
	else {
		byte_t* p = ((byte_t*)dest);
		for (usize_t i = 0; i < times; i++) { m_copy(p, value, size); p += size; }
	}

}


static inline void m_clear(void* dest, usize_t size) {
	if (size == sizeof(word_t)) {
		*((word_t*)dest) = (word_t)0;
	}
	else {
		usize_t wordc = size / sizeof(word_t);
		usize_t bytec = size % sizeof(word_t);

		if (wordc)for (usize_t i = 0; i < wordc; i++) {
			*((word_t*)dest) = (word_t)0;
			dest = ((word_t*)dest) + 1;
		}
		if (bytec) for (usize_t i = 0; i < bytec; i++) {
			*((byte_t*)dest) = (byte_t)0;
			dest = ((byte_t*)dest) + 1;
		}

	}

}

static inline bool_t m_equal(void* dest, const void* src, usize_t size) {
	if (dest) {
		if (src) {
			if (dest == src || !size) return 1;
			if (size == sizeof(word_t)) {
				return *((word_t*)dest) == *(word_t*)src;
			}
			if (size == sizeof(byte_t)) {
				return *((byte_t*)dest) == *(byte_t*)src;
			}
			usize_t wordc = size / sizeof(word_t);
			usize_t bytec = size % sizeof(word_t);

			if (wordc)for (usize_t i = 0; i < wordc; i++) {
				if (*((word_t*)dest) != *((word_t*)src))return 0;
				dest = ((word_t*)dest) + 1; src = ((word_t*)src) + 1;
			}
			if (bytec) for (usize_t i = 0; i < bytec; i++) {
				if (*((byte_t*)dest) != *((byte_t*)src)) return 0;
				dest = ((byte_t*)dest) + 1; src = ((byte_t*)src) + 1;
			}
			return 1;
		}
		else return 0;
	}
	else {
		return src ? 0 : 1;
	}
	


}

#endif // end ifndef __LAYOUT_INCLUDED__
