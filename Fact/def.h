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

//是否是开发过程
#define __DEVALOPMENT__

typedef char byte_t;
typedef unsigned char ubyte_t;
typedef short short_t;
typedef unsigned short ushort_t;
typedef int int_t;
typedef unsigned int uint_t;
typedef long long_t;
typedef unsigned int ulong_t;
typedef long long large_t;
typedef unsigned long long ularge_t;
typedef unsigned int lchar_t;
typedef int bool_t;

#if defined(_WIN64) || defined(__LP64__) || defined(__amd64)  || defined(__IA64__) || __SIZEOF_POINTER__ == 8
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
typedef long word_t;
typedef unsigned long uword_t;
typedef long long dword_t;
typedef unsigned long long udword_t;
typedef long long  lword_t[2];
typedef unsigned long usize_t;
typedef unsigned long addr_t;
#elif defined(__32BITS__)
typedef int word_t;
typedef unsigned int uword_t;
typedef long dword_t;
typedef unsigned long udword_t;
typedef long long lword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#else // else16
typedef short word_t;
typedef unsigned short uword_t;
typedef int dword_t;
typedef unsigned int udword_t;
typedef long  lword_t;
typedef unsigned int usize_t;
typedef unsigned int addr_t;
#endif // endif _WIN64

typedef struct stVTBL {
	usize_t offset;
	void* vfp0;
}TVTBL;

typedef TVTBL* vftptr_t;

typedef struct stVirtStructLayout {
	vftptr_t vftptr;
}VirtStructLayout;
static inline void* VirtStructLayout_getvf(const VirtStructLayout* const self, usize_t index) {
	return ((void**)(&self->vftptr->vfp0))[index];
}

#endif // end ifndef __DEF_INCLUDED__
