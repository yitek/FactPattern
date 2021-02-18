/*****************************************************
* 
* author:yiy
* 
* description: 公用定义文件
* 定义一些常用数据类型
* 32位或64位编译能够用相同的源代码
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

typedef long long word_t;
typedef unsigned long long uword_t;
typedef long long dword_t[2];
typedef unsigned long long udword_t[2];
typedef unsigned long long usize_t;

#elif defined(__32BITS__)
typedef long long long_t;
typedef unsigned long long ulong_t;
typedef unsigned int addr_t;

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

typedef short word_t;
typedef unsigned short uword_t;
typedef int dword_t;
typedef unsigned int udword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#endif // endif _WIN64

#if defined(__WIN__)
typedef unsigned short lchar_t;
#elif defined(__LINUX__)
typedef unsigned int lchar_t;
#endif

typedef char byte_t;
typedef unsigned char ubyte_t;
typedef short short_t;
typedef unsigned short ushort_t;
typedef int int_t;
typedef unsigned int uint_t;
typedef unsigned int unichar_t;
typedef word_t bool_t;

typedef struct stVTBLLayout {
	usize_t offset;
}TVTBLLayout;
typedef struct stVTBL {
	struct stVTBLLayout;
	void* vfp0;
}TVTBL;

typedef TVTBL* vftptr_t;

typedef struct stVirtStructLayout {
	vftptr_t vftptr;
}TVirtStructLayout;
static inline void* VirtStructLayout_getvf(const TVirtStructLayout* const self, usize_t index) {
	return ((void**)(&self->vftptr->vfp0))[index];
}

typedef struct stGCUnitLayout {
	void* type;
	usize_t ref;
}TGCUnitLayout;

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
	}else if (size == sizeof(word_t) * 2) {
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

#endif // end ifndef __DEF_INCLUDED__
