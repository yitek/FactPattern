#pragma once
/*****************************************************
*
* author:yiy
*
* description: 内存管理基类
* 定义内存管理器的抽象结构
* 实现基于malloc的最简单的内存管理器
*
******************************************************/

#pragma once
#include "../runtime.h"
#ifndef __TMEMORY_INCLUDED__ 
#define __TMEMORY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif

	struct stTMemory;
	typedef enum {
		MemoryAllocatingDirective_fail = 0,
		MemoryAllocatingDirective_lookup = -1,
		MemoryAllocatingDirective_new = 1,
		MemoryAllocatingDirective_lookupOrNew = 2,
	} MemoryAllocatingDirectives;

	

	
	typedef struct stMemoryOptions {
		usize_t totalBytes;
	}MemoryOptions;

	typedef struct stTMemory {
		/// <summary>
		/// 虚函数指针，直接指向后面的vtbl结构，跟c++的对象布局保持一致
		/// </summary>
		struct stTObject;
		struct stMemoryOptions;
		Allocator allocator;
		TLogger* logger;
		
	} TMemory;

	typedef struct stMemoryMeta {
		struct stClazzMeta;
		void* (*alloc)(TMemory* self,usize_t size,uword_t masks);
		bool_t (*free)(TMemory* self,void* p);
		void(*__destruct__)(TMemory* self,bool_t existed);
		MemoryAllocatingDirectives(*allocating)(TMemory* memory,usize_t size,uword_t masks,void* param);
	} MemoryMeta;

	extern MemoryMeta TMemory__meta__;

	/// <summary>
	/// 唯一的默认内存管理器，
	/// </summary>
	extern TMemory* TMemory_default;
	;
	extern struct stMemoryGCLayout TMemory_defaultInstance;
	/// <summary>
	/// 最近基础的内存管理器的构造函数
	/// 需要填充stMemory的各个虚函数指针
	/// </summary>
	/// <param name="self"></param>
	/// <returns></returns>
	TMemory* TMemory__construct__(TMemory* self, const MemoryOptions* options, TLogger* logger);
	void TMemory__destruct__(TMemory* self, bool_t existed);
	void* TMemory_alloc(TMemory* self, usize_t size,uword_t mask);
	bool_t TMemory_free(TMemory* self, void* obj);
	//MemoryAllocatingDirectives Memory__allocating(Memory* memory, usize_t size, void* param);
	

	inline static void* TMemory_alloc__virtual__(TMemory* self, usize_t size,uword_t masks) {return ((MemoryMeta*)((TObject*)self)->__meta__)->alloc(self,size,masks);}
	inline static bool_t TMemory_free__virtual__(TMemory* self, void* p) {return ((MemoryMeta*)((TObject*)self)->__meta__)->free(self, p);}
	inline static void TMemory__destruct____virtual__(TMemory* self, bool_t existed) { ((MemoryMeta*)((TObject*)self)->__meta__)->__destruct__(self, existed); }

	static inline bool_t TMemory_copy(void* dest, const void* src, usize_t size) { return (dest && src && size) ? m_copy(dest, src, size), 1 : 0; }
	static inline bool_t TMemory_repeat(void* dest, usize_t times, void* value, usize_t size) { return (dest&&times&&value&&size)?m_repeat(dest,times,value,size),1:0; }
	static inline bool_t TMemory_clear(void* dest, usize_t size) { return dest&&size? m_clear(dest,size),1:0; }
	static inline bool_t TMemory_equal(void* dest, const void* src, usize_t size) {return  m_equal(dest, src, size);}

	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TMEMORY_INCLUDED__



