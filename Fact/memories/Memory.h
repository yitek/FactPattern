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
#include "../loggers/Logger.h"
#ifndef __MEMORY_INCLUDED__ 
#define __MEMORY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif

	struct stMemory;
	typedef word_t (*MemoryAllocating)(struct stMemory* self,usize_t t,void* param);
	typedef struct stMemoryOptions {
		MemoryAllocating allocating;
	}MemoryOptions;

	typedef struct stMemory {
		/// <summary>
		/// 虚函数指针，直接指向后面的vtbl结构，跟c++的对象布局保持一致
		/// </summary>
		struct stVirtStructLayout;
		struct stMemoryOptions;
		Logger* logger;
	} Memory;

	typedef struct stMemoryVTBL {
		struct stVTBLLayout;
		void* (*alloc)(Memory* self,usize_t size);
		void* (*alloc1)(Memory* self, usize_t size);
		bool_t (*free)(Memory* self,void* p);
		void(*__destruct__)(Memory* self,bool_t existed);
	} MemoryVTBL;

	extern MemoryVTBL memoryVTBL;

	/// <summary>
	/// 唯一的默认内存管理器，
	/// </summary>
	extern Memory* Memory_default;
	typedef struct stMemoryGCLayout{
		ObjectLayout __GC__;
		struct stMemory;
	} MemoryGCLayout;
	extern MemoryGCLayout Memory_defaultInstance;
	/// <summary>
	/// 最近基础的内存管理器的构造函数
	/// 需要填充stMemory的各个虚函数指针
	/// </summary>
	/// <param name="self"></param>
	/// <returns></returns>
	Memory* Memory__construct__(Memory* self, const MemoryOptions* options, Logger* logger);
	void Memory__destruct__(Memory* self, bool_t existed);
	void* Memory_alloc(Memory* self, usize_t size);
	void* Memory_alloc1(Memory* self, usize_t size);
	bool_t Memory_free(Memory* self, void* obj);
	//inline bool_t Memory_inc_ref(Memory* self, void* obj) { return 1; }
	//inline bool_t Memory_dec_ref(Memory* self, void* obj) { return 1; }
	

	inline static void* Memory_alloc__virtual__(Memory* self, usize_t size) {return ((MemoryVTBL*)((VirtStructLayout*)self)->vftptr)->alloc(self,size);}
	inline static void* Memory_alloc1__virtual__(Memory* self, usize_t size) { return ((MemoryVTBL*)((VirtStructLayout*)self)->vftptr)->alloc1(self, size); }
	inline static bool_t Memory_free__virtual__(Memory* self, void* p) {return ((MemoryVTBL*)((VirtStructLayout*)self)->vftptr)->free(self, p);}
	inline static void Memory__destruct____virtual__(Memory* self, bool_t existed) { ((MemoryVTBL*)((VirtStructLayout*)self)->vftptr)->__destruct__(self, existed); }

	static inline bool_t Memory_copy(void* dest, const void* src, usize_t size) { return (dest && src && size) ? m_copy(dest, src, size), 1 : 0; }
	static inline bool_t Memory_repeat(void* dest, usize_t times, void* value, usize_t size) { return (dest&&times&&value&&size)?m_repeat(dest,times,value,size),1:0; }
	static inline bool_t Memory_clear(void* dest, usize_t size) { return dest&&size? m_clear(dest,size),1:0; }
	static inline bool_t Memory_equal(void* dest, const void* src, usize_t size) {return  m_equal(dest, src, size);}

	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __MEMORY_INCLUDED__



