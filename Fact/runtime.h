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
static inline void out_p(void* p, favor_t width) { 
	outlx((ulong_t)p,width); 
}
void outs_fmt(const byte_t* str, ...);
void outcs_fmt(OutColors color, const byte_t* str, ...);
void outcs_fmtln(OutColors color, const byte_t* str, ...);
void outs_format(const byte_t* p, bool_t ignoreEndRet, void* args);
void outcs_format(OutColors color, const byte_t* str, bool_t ignoreEndRet, void* args);


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



struct stTMemoryLayout {
	struct stMRefUnit;
	struct stTMemory inst;
};

extern struct stTMemoryLayout TMemory_instance;
extern TMemory* TMemory_default;

TMemory* TMemory__construct__(TMemory* self);
void TMemory__destruct__(TMemory* self,bool_t exsited);
void* TMemory_alloc(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
void* TMemory_alloc1(TMemory* mm, usize_t size, void* mInitArgs, MemoryKinds mkinds);
bool_t TMemory_free(TMemory* mm,void*p);

static inline void* m_alloc(usize_t size, void* mInitArgs, MemoryKinds mkinds, TMemory* mm) {
	return ((TMemoryMeta*)((TObject*)mm)->__meta__)->alloc(mm, size, mInitArgs, mkinds);
}
static inline void* m_alloc1( usize_t size, void* mInitArgs, MemoryKinds mkinds, TMemory* mm) {
	return ((TMemoryMeta*)((TObject*)mm)->__meta__)->alloc1(mm, size, mInitArgs, mkinds);
}
static inline bool_t m_free(void* p, TMemory* mm) {
	return ((TMemoryMeta*)((TObject*)mm)->__meta__)->free(mm, p);
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

void m_look(const unsigned char* str, usize_t length, MLookTake take);


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

typedef enum {

	ExitCode_memory = 1,
	ExitCode_critical = 2,
	ExitCode_argument = 3
} ExitCodes;
favor_t log_exit(favor_t code, const byte_t* category, const byte_t* message, ...);



// 基础内存类




#ifdef __cplusplus 
} //extern "C" 
#endif
#endif // end ifndef __LAYOUT_INCLUDED__
