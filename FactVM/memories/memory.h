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
#include "../def.h"
#ifndef __MEMORY_INCLUDED__ 
#define __MEMORY_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	struct stMemory;
	/// <summary>
	/// 内存管理器的虚函数表结构(命名表)
	/// </summary>
	typedef struct stMemoryVTBL {
		/// <summary>
		/// 请求一片内存，
		/// </summary>
		void* (*require)(struct stMemory* self, size_t size,void* type);
		/// <summary>
		/// (强)释放一片内存
		/// 直接释放，内存已经返回给了内存管理器
		/// </summary>
		bool_t (*release)(struct stMemory* self, void* p);
		/// <summary>
		/// 弱释放内存
		/// 在malloc时，跟强释放release是一致的
		/// 但在垃圾回收内存分配时，弱释放只是减少了引用计数
		/// </summary>
		bool_t (*weekRelease)(struct stMemory* self, void* p);
		/// <summary>
		///  析构函数，内存管理器撤销时调用
		/// </summary>
		void (*destruct)(struct stMemory* allocator);
	}MemoryVTBL;

	typedef struct stMemory {
		/// <summary>
		/// 虚函数指针，直接指向后面的vtbl结构，跟c++的对象布局保持一致
		/// </summary>
		struct stMemoryVTBL* vptr;
		/// <summary>
		/// 请求一片内存，
		/// </summary>
		void* (*require)(struct stMemory* self, size_t size,void* type);
		/// <summary>
		/// (强)释放一片内存
		/// 直接释放，内存已经返回给了内存管理器
		/// </summary>
		bool_t(*release)(struct stMemory* self, void* p);
		/// <summary>
		/// 弱释放内存
		/// 在malloc时，跟强释放release是一致的
		/// 但在垃圾回收内存分配时，弱释放只是减少了引用计数
		/// </summary>
		bool_t(*weekRelease)(struct stMemory* self, void* p);
		/// <summary>
		///  析构函数，内存管理器撤销时调用
		/// </summary>
		void (*destruct)(struct stMemory* allocator);
	} Memory;

	/// <summary>
	/// 唯一的默认内存管理器，
	/// </summary>
	extern Memory* Memory_defaultInstance;
	/// <summary>
	/// 最近基础的内存管理器的构造函数
	/// 需要填充stMemory的各个虚函数指针
	/// </summary>
	/// <param name="self"></param>
	/// <returns></returns>
	Memory* Memory___construct__(Memory* self);
	/// <summary>
	/// 析构函数
	/// </summary>
	/// <param name="self"></param>
	inline void Memory___destruct__(Memory* self) { if (self->destruct) self->destruct(self); }

	inline void* Memory_require(Memory* self, size_t size,void* type) { return self->require(self, size,type); }
	inline bool_t Memory_release(Memory* self, void* obj) { return self->release(self, obj); }
	inline bool_t Memory_weekRelease(Memory* self, void* obj) { return self->release(self, obj); }

	bool_t Memory_copy(void* dest, const void* src, size_t size);

	inline Memory* Memory_default() {
		return Memory_defaultInstance ? Memory_defaultInstance : (Memory_defaultInstance = Memory___construct__(Memory_defaultInstance));

	}
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __MEMORY_INCLUDED__



