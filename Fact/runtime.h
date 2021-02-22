/*****************************************************
*
* author:yiy
*
* description: 运行期
* 定义了TObject（对象） TType(类型)与虚表等的内存布局
* 封装了内存操作,定义了内存分配器Allocator
* 定义了一个类TLogger ,日志类
*
******************************************************/

#pragma once
#include "def.h"

#ifndef __RUNTIME_INCLUDED__ 
#define __RUNTIME_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif
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

typedef struct stMLnkUnit {
	struct stMLnkUnit* next;
}MLnkUnit;

typedef struct stMRefUnit {
	usize_t ref;
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
static inline void* ref_increase(void* obj) { return (((MRefUnit*)obj-1)->ref++,obj); }
#define ref_inc(obj) (((MRefUnit*)obj-1)->ref++,obj)
static inline void* ref_decrease(void* obj) { return (--(((MRefUnit*)obj - 1)->ref) >= 0 ? obj : ((((MRefUnit*)obj - 1)->ref=0,obj))); }
#define ref_dec(obj) (--(((MRefUnit*)obj - 1)->ref) >= 0 ? obj : ((((MRefUnit*)obj - 1)->ref=0,obj)))

#define Object_param(obj) ( (*((MTObjUnit*)obj-1)).ref++,obj )
#define Object_assign(dest,obj) ( (*((MTObjUnit*)(dest=obj)-1)).ref++,obj )
#define Object_release(obj) ( --(*((MTObjUnit*)obj-1)).ref<=0?(obj=0):obj )


void* m_alloc(usize_t size, uword_t mkinds);
bool_t m_free(void* p);
#define m_allocate(T,mmkinds) ((T*)m_alloc(sizeof(T),mmkinds));



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

static inline usize_t m_strlen(const char* str) {
	usize_t len = 0;
	while (*str++)++len;
	return len;
}

static inline const char* m_cstr(const char* str) {
	usize_t len = m_strlen(str);
	char* p = (char*)m_alloc(len+1,0);
	m_copy(p,str,len);
	*(p + len) = 0;
	return (const char*)p;
}
typedef void* (*AllocatorAlloc)(void* mm, usize_t size, uword_t mkinds);
typedef bool_t(*AllocatorFree)(void* mm, void* p);
typedef struct stAllocator {
	void* __mm__;
	 AllocatorAlloc alloc;
	 AllocatorFree free;
}Allocator;
#define allocate(T,allocator,...) ((T*)allocator->alloc(allocator->__mm__,sizeof(T),0##__VA_ARGS__))
#define withdraw(allocator,p) (allocator->free(__mm__->__mm__,p))
extern Allocator allocator;


typedef enum {
	LogLevel_None = 0,
	LogLevel_SectionBegin = 1,
	LogLevel_SectionEnd = 1 << 1,
	LogLevel_Trace = 1 << 8,
	LogLevel_Message = 1 << 9,
	LogLevel_Info = 1 << 10,
	LogLevel_Success = 1 << 11,
	LogLevel_Notice = 1 << 12,
	LogLevel_Warn = 1 << 13,
	LogLevel_Exception = 1 << 14,
	LogLevel_Error = 1 << 15,


}LogLevels;

typedef struct stLoggerOptions {
	LogLevels level;
}LoggerOptions;

typedef struct stTest {
	const char* category;
	usize_t assertCount;
	usize_t failCount;
	usize_t childCount;
	usize_t failedChildCount;
	struct stTest* parent;
}Test;

typedef struct stTLogger {
	struct stTObject;
	struct stLoggerOptions;
} TLogger;

typedef void (*TLoggerOutput)(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);

typedef struct stLoggerMeta {
	struct stClazzMeta;
	TLoggerOutput output;
}LoggerMeta;

extern LoggerMeta TLogger__meta__;


extern TLogger* TLogger_default;

TLogger* TLogger__construct__(TLogger* self, LoggerOptions* opts);
void TLogger__destruct__(TLogger* self, bool_t existed);
void TLogger_trace(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_message(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_info(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_success(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_notice(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_warn(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_exception(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_error(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_sectionBegin(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_sectionEnd(TLogger* self, const byte_t* category, const byte_t* message, ...);
void TLogger_log(TLogger* self, LogLevels level, const byte_t* category, const byte_t* message, ...);

void TLogger__output(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
static inline void TLogger_output__virtual__(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
	((LoggerMeta*)(((TObject*)self)->__meta__))->output(self, lv, category, message, args);
}


void TLogger__printf(const byte_t* p, void* args);

typedef enum {

	ExitCode_memory = 1,
	ExitCode_critical = 2,
	ExitCode_argument = 3
} ExitCodes;
word_t log_exit(word_t code, const byte_t* category, const byte_t* message, ...);



// 基础内存类




#ifdef __cplusplus 
} //extern "C" 
#endif
#endif // end ifndef __LAYOUT_INCLUDED__
