/*****************************************************
*
* author:yiy
*
* description: 运行期
* 代表与底层系统的隔断，其他类都包含该头文件，不再需要标准库头文件支持
* 
* 封装了控制台输出
* 封装了内存操作,定义了内存分配器TMemory
* 定义了日志类TLogger
*
******************************************************/

#pragma once
#include "def.h"

#ifndef __RUNTIME_INCLUDED__ 
#define __RUNTIME_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif


	typedef enum {
		OutColor_orign = 0,
		OutColor_blue = 1,
		OutColor_green = 2,
		OutColor_red = 4,
		OutColor_highline = 8,
		OutForeColor_blue = 1,
		OutForeColor_green = 2,
		OutForeColor_red = 4,
		OutForeColor_highline = 8,
		OutBackColor_blue = 1 << 4,
		OutBackColor_green = 2 << 4,
		OutBackColor_red = 4 << 4,
		OutBackColor_highline = 8 << 4

	}OutColors;
	void out(byte_t ch);
	void outs(const byte_t* str);
	void outln(const byte_t* str);

	void outc(OutColors color, byte_t ch);
	void outcs(OutColors color, const byte_t* str);
	void outcln(OutColors color, const byte_t* str);


	void outu(uint_t n, favor_t width);
	void outd(int_t n, favor_t width);
	void outx(uint_t n, favor_t width);
	void outb(uint_t n, favor_t width);
	void outlu(ulong_t n, favor_t width);
	void outld(long_t n, favor_t width);
	void outlx(ulong_t n, favor_t width);
	void outlb(ulong_t n, favor_t width);
	void outf(double n, favor_t i, favor_t f);
	void outt(const byte_t* fmt);
	static inline void out_p(void* p, int width) {
		outlx((ulong_t)p, width);
	}
	void outs_fmt(const byte_t* str, ...);
	void outcs_fmt(OutColors color, const byte_t* str, ...);
	void outcs_fmtln(OutColors color, const byte_t* str, ...);
	void outs_format(const byte_t* p, bool_t ignoreEndRet, void* args);
	void outcs_format(OutColors color, const byte_t* str, bool_t ignoreEndRet, void* args);



	typedef enum {
		MemoryUnitKind_link,
		MemoryUnitKind_ref
	} MemoryUnitKinds;

	struct stMLnkUnit {
		struct stMLnkUnit* next;
	};

	struct stMRefUnit {
		usize_t __ref__;
	};

	struct stTType;
	struct stTObject;


	typedef struct stClazzMeta {
		struct stVFTL;
		struct stTType* (*__gettype__)(struct stTObject* self);
		struct stTString* (*__toString__)(struct stTObject* self);
		favor_t(*__compare__)(struct stTObject* left, struct stTObject* right);
	} ClazzMeta;

	struct stTObject {
		/// <summary>
			/// 对象信息指针(结构跟虚函数表一样，只是第一个虚函数固定成get_type函数)
			/// </summary>
		const struct stClazzMeta*const __meta__;

	};

	struct stTObjectMeta {
		struct stClazzMeta;
		void(*__endle__)();
	} ;

	typedef struct stMTObjUnit {
		struct stMRefUnit;
		struct stTObject;
	} MTObjUnit;

	extern const struct stTType* const KeyValuePair__type__;
	
	struct stIIteratorMeta {
		struct stTObjectMeta;
		void (*__reset__)();
		void* (*__current__)();
		bool_t(*__next__)();
	};


	struct stISetMeta {
		struct stTObjectMeta;
		struct stIIterator* (*__iterator__)();
	};
	
	struct stISet {
		struct stTObject;
	};

	struct stArray {
		usize_t length;
	};


	struct stTArray {
		struct stISet;
		struct stArray;
	};

	struct stString {
		usize_t length;
		usize_t bytes;
	};

	struct stTString {
		struct stISet;
		struct stString;
	};
	struct _stTString {
		struct stISet __ob__;
		struct stString __str__;
	};

	struct stLink {
		struct stLink* next;
	};
	struct stList {
		usize_t length;
		struct stLink* head;
		struct stLink* tail;
	};
	struct stTList {
		struct stISet;
		struct stList;
	};

	struct _stTList {
		struct stISet __ob__;
		struct stList __list__;
	};

	typedef enum {
		TypeKind_func = 0b0000001,
		TypeKind_struct = 0b0000010,
		TypeKind_class = 0b0000100,
		TypeKind_interface = 0b0001000
	}TypeKinds;

	struct stTType {
		struct stTObject;
		const struct stTString* name;
		ufavor_t size;
		TypeKinds kind;
		struct stVFTL* vftptr;
		struct stTArray* genericRestricts;
		struct stTType* base;
		struct stTArray* mixins;
		struct stTArray* interfaces;
		struct stTArray* genericArguments;
		struct stTArray* fields;
		struct stTArray* methods;
		struct stTArray* properties;
		void* extras;
	};
	typedef enum {
		MemberType_field = 0b01,
		MemberType_method = 0b10,
		MemberType_property = 0b11
	} MemberTypes;

	typedef enum {
		AccessLevel_private = 0,
		AccessLevel_protected = 0b00100000000,
		AccessLevel_internal = 0b01000000000,
		AccessLevel_protected_internal = 0b01100000000,
		AccessLevel_public = 0b11100000000
	} AccessLevels;

	struct stTMember {
		struct stTObject;
		struct stTString* name;
		uword_t decorators;
		struct stTType* type;
	};

	struct stTField {
		struct stTMember;
		usize_t offset;
	};

	struct stGenericRestrict {
		const struct stTType* defaultType;
		const struct stTType* restrictType;
	};
	


	typedef struct {
		struct stArray;
		union {
			struct stGenericRestrict infos[2];
			struct {
				struct stGenericRestrict valueRestrict;
				struct stGenericRestrict keyRestrict;
			};
		};

	} KeyValuePairRestricts;

	extern const KeyValuePairRestricts KeyValuePair_genericRestrictsInstance;

	extern const struct stTObjectMeta TObject__metaInstance;
	extern const struct stTObjectMeta KeyValuePair__metaInstance;
	extern const struct stIIteratorMeta IIterator__metaInstance;
	extern const struct stISetMeta ISet__metaInstance;
	extern const struct stTObjectMeta TType__metaInstance;
	extern const struct stTObjectMeta TString__metaInstance;

	extern const struct stTType*const TObject__type__;
	extern const struct stTType* const KeyValuePair__type__;
	extern const struct stTType* const IIterator__type__;
	extern const struct stTType* const ISet__type__;
	extern const struct stTType*const TType__type__;
	extern const struct stTType*const TString__type__;
	inline static const struct stTType* TObject__gettype__(struct stTObject* self) { return TObject__type__; };
	inline static const struct stTType* KeyValuePair__gettype__(struct stTObject* self) { return KeyValuePair__type__; };
	inline static const struct stTType* IIterator__gettype__(struct stTObject* self) { return IIterator__type__; };
	inline static const struct stTType* ISet__gettype__(struct stTObject* self) { return ISet__type__; }
	inline static const struct stTType* TType__gettype__(struct stTObject* self) { return TType__type__; };
	inline static const struct stTType* TString__gettype__(struct stTObject* self) { return TString__type__; };

	inline static favor_t TObject__compare__(struct stTObject* left, struct stTObject* right) { return left == right ? 0 : ((left > right) ? 1 : -1); }
	
	inline static const struct stTString* TObject__toString__(struct stTObject* self) {return self->__meta__->__gettype__(self)->name;}
	inline static const struct stTString* TString__toString__(struct stTObject* self) { return (struct stTString*)self; };
	const struct stTString* KeyValuePair__toString__(struct stTObject* self);
	static inline struct stTType* get_type(struct stTObject* obj) {
		return (struct stTType*)obj->__meta__->__gettype__(obj);
	}
	static inline struct stTType* get_genericArgument(struct stTObject* obj, ufavor_t index) {
		return ((struct stTType*)(obj->__meta__->__gettype__(obj)->genericArguments + 1)) + index;
	}
	static inline void* ref_increase(void* obj) { return (((struct stMRefUnit*)obj - 1)->__ref__++, obj); }
#define ref_inc(obj) (((struct stMRefUnit*)obj-1)->__ref__++,obj)
	static inline void* ref_decrease(void* obj) { return (--(((struct stMRefUnit*)obj - 1)->__ref__) >= 0 ? obj : ((((struct stMRefUnit*)obj - 1)->__ref__ = 0, obj))); }
#define ref_dec(obj) (--(((struct stMRefUnit*)obj - 1)->__ref__) >= 0 ? obj : ((((struct stMRefUnit*)obj - 1)->__ref__=0,obj)))

#define Object_param(obj) ( (*((struct stMTObjUnit*)obj-1)).__ref__++,obj )
#define Object_assign(dest,obj) ( (*((struct stMTObjUnit*)(dest=obj)-1)).__ref__++,obj )
#define Object_release(obj) ( --(*((struct stMTObjUnit*)obj-1)).__ref__<=0?(obj=0):obj )

typedef enum {
	MemoryKind_normal = 0,
	MemoryKind_disCollect = 1 << 1,
	MemoryKind_readonly = 1 << 2,
	MemoryKind_system = 1 << 3,
	MemoryKind_share = 1 << 4
} MemoryKinds;

typedef struct stTMemory {
	struct stTObject;
}TMemory;

typedef struct stTMemoryMeta {
	struct stClazzMeta;
	void* (*alloc)(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
	void* (*alloc1)(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
	bool_t(*free)(TMemory* mm, void* p);
	void (*__destruct__)(TMemory* mm, bool_t existed);
}TMemoryMeta;

extern const struct stTType*const TMemory__type__;

extern TMemory* TMemory_default;

TMemory* TMemory__construct__(TMemory* self);
void TMemory__destruct__(TMemory* self,bool_t exsited);
void* TMemory_alloc(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
void* TMemory_alloc1(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
bool_t TMemory_free(TMemory* mm,void*p);

static inline void* m_alloc(usize_t size, void* mInitArgs, MemoryKinds mkinds, TMemory* mm) {
	return ((TMemoryMeta*)((struct stTObject*)mm)->__meta__)->alloc(mm, size, mInitArgs, mkinds);
}
static inline void* m_alloc1( usize_t size, void* mInitArgs, MemoryKinds mkinds, TMemory* mm) {
	return ((TMemoryMeta*)((struct stTObject*)mm)->__meta__)->alloc1(mm, size, mInitArgs, mkinds);
}
static inline bool_t m_free(void* p, TMemory* mm) {
	return ((TMemoryMeta*)((struct stTObject*)mm)->__meta__)->free(mm, p);
}
#define m_allocate(T,mInitArgs,mkind, mm) ((T*)m_alloc(sizeof(T),mInitArgs, mkind,mm?mm:TMemory_default))
#define m_withdraw(mm,p) m_free(p,mm?mm:TMemory_default)


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
static inline  void TMemory_copy(void* dest, const void* src, usize_t size) { m_copy(dest,src,size); }

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

static inline void TMemory_repeat(void* dest, usize_t times, void* value, usize_t size) { m_repeat(dest,times,value,size); }


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

static inline void TMemory_clear(void* dest, usize_t size) {m_clear(dest,size);}

static inline bool_t m_equal(const void* dest, const void* src, usize_t size) {
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
static inline bool_t TMemory_equal(void* dest, const void* src, usize_t size) { return m_equal(dest,src,size); }

static inline int m_compare(const void* leftBuffer, const void* rightBuffer) {
	if (leftBuffer == rightBuffer) return 0;
	if (leftBuffer) {
		if (rightBuffer) {
			usize_t at = 0;
			for (;; at++) {
				utiny_t lc = *(utiny_t*)leftBuffer; utiny_t rc = *(utiny_t*)rightBuffer;
				if (lc > rc) return 1;
				if (lc < rc) return -1;
				if (lc == 0) return 0;
				leftBuffer =((utiny_t*)leftBuffer)+1; rightBuffer= ((utiny_t*)rightBuffer)+1;
			}
		}
		else return 1;
	}
	else {
		return -1;
	}
}

static inline bool_t TMemory_compare(const void* left, const void* right) { return m_compare(left, right); }


static inline usize_t m_strlen(const char* str) {
	usize_t len = 0;
	while (*str++)++len;
	return len;
}

static inline const char* m_cstr(const char* str) {
	usize_t len = m_strlen(str);
	char* p = (char*)m_alloc(len+1,0,MemoryKind_normal, TMemory_default);
	m_copy(p,str,len);
	*(p + len) = 0;
	return (const char*)p;
}

typedef usize_t(*MLookTake)(utiny_t b);

void m_look(const void* str, usize_t length, MLookTake take);

/// <summary>
/// TLogger
/// </summary>



typedef enum {
	LogLevel_none = 0,
	LogLevel_sectionBegin = 1,
	LogLevel_sectionEnd = 1 << 1,
	LogLevel_trace = 1 << 8,
	LogLevel_message = 1 << 9,
	LogLevel_info = 1 << 10,
	LogLevel_success = 1 << 11,
	LogLevel_notice = 1 << 12,
	LogLevel_warn = 1 << 13,
	LogLevel_exception = 1 << 14,
	LogLevel_error = 1 << 15,


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

typedef struct stTLoggerMeta {
	struct stClazzMeta;
	TLoggerOutput output;
}TLoggerMeta;

extern const struct stTType*const TLogger__type__;
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
	((TLoggerMeta*)(((struct stTObject*)self)->__meta__))->output(self, lv, category, message, args);
}

typedef enum {

	ExitCode_memory = 1,
	ExitCode_critical = 2,
	ExitCode_argument = 3
} ExitCodes;
favor_t log_exit(favor_t code, const byte_t* category, const byte_t* message, ...);





#ifdef __cplusplus 
} //extern "C" 
#endif
#endif // end ifndef __LAYOUT_INCLUDED__
