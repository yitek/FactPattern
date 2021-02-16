#pragma once
/*****************************************************
*
* author:yiy
*
* description: �ڴ�������
* �����ڴ�������ĳ���ṹ
* ʵ�ֻ���malloc����򵥵��ڴ������
*
******************************************************/

#pragma once
#include "../loggers/Logger.h"
#ifndef __MEMORY_INCLUDED__ 
#define __MEMORY_INCLUDED__
// c�������Ͻ�cpp�ı�׼�÷�,ָ������ķ��Ű���c���Է�ʽ����
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stMemoryLogger {
		void(*allocated)(size_t size,const char_t* reason,const char_t* message);
		void(*free)(size_t size, const char_t* reason, const char_t* message);
	}MemoryLogger;
	struct stMemory;
	/// <summary>
	/// �ڴ���������麯����ṹ(������)
	/// </summary>
	typedef struct stMemoryVTBL {
		/// <summary>
		/// ����һƬ�ڴ棬
		/// </summary>
		void* (*require)(struct stMemory* self,size_t size,void* type);
		void* (*require1)(struct stMemory* self, size_t size, void* type);
		/// <summary>
		/// (ǿ)�ͷ�һƬ�ڴ�
		/// ֱ���ͷţ��ڴ��Ѿ����ظ����ڴ������
		/// </summary>
		bool_t (*release)(struct stMemory* self, void* p);

		/// <summary>
		/// ��������
		/// ��mallocʱ���Ǹ��պ���
		/// </summary>
		bool_t(*increase)(void* p);
		/// <summary>
		/// ��������
		/// ��mallocʱ���Ǹ��պ���
		/// </summary>
		bool_t (*decrease)(void* p);
		/// <summary>
		///  �����������ڴ����������ʱ����
		/// </summary>
		void (*destruct)(struct stMemory* allocator);
	}MemoryVTBL;

	typedef struct stMemory {
		/// <summary>
		/// �麯��ָ�룬ֱ��ָ������vtbl�ṹ����c++�Ķ��󲼾ֱ���һ��
		/// </summary>
		struct stMemoryVTBL* vptr;
		/// <summary>
		/// ����һƬ�ڴ棬
		/// </summary>
		void* (*require)(struct stMemory* self, size_t size,void* type);
		void* (*require1)(struct stMemory* self, size_t size, void* type);
		/// <summary>
		/// (ǿ)�ͷ�һƬ�ڴ�
		/// ֱ���ͷţ��ڴ��Ѿ����ظ����ڴ������
		/// </summary>
		bool_t(*release)(struct stMemory* self, void* p);
		/// <summary>
		/// ��������
		/// ��mallocʱ���Ǹ��պ���
		/// </summary>
		bool_t(*increase)(struct stMemory* self, void* p);
		/// <summary>
		/// ���ͷ��ڴ�
		/// ��mallocʱ����ǿ�ͷ�release��һ�µ�
		/// �������������ڴ����ʱ�����ͷ�ֻ�Ǽ��������ü���
		/// </summary>
		bool_t(*decrease)(struct stMemory* self, void* p);
		/// <summary>
		///  �����������ڴ����������ʱ����
		/// </summary>
		void (*destruct)(struct stMemory* memory,bool_t autoFree);
		Logger* logger;
	} Memory;

	/// <summary>
	/// Ψһ��Ĭ���ڴ��������
	/// </summary>
	extern Memory* Memory_defaultInstance;
	/// <summary>
	/// ����������ڴ�������Ĺ��캯��
	/// ��Ҫ���stMemory�ĸ����麯��ָ��
	/// </summary>
	/// <param name="self"></param>
	/// <returns></returns>
	Memory* Memory_construct(Memory* self,Logger* logger);
	void* Memory_require(Memory* self, size_t size, void* type);
	bool_t Memory_release(Memory* self, void* obj);
	inline bool_t Memory_increase(Memory* self, void* obj) { return 1; }
	inline bool_t Memory_decrease(Memory* self, void* obj) { return 1; }
	void Memory_destruct(Memory* self, bool_t existed);


	inline bool_t Memory_copy(void* dest, const void* src, size_t size) {

		if (dest && src && size) {
			if (size == sizeof(word_t)) {
				*((word_t*)dest) = *(word_t*)src;
				return 1;
			}
			size_t wordc = size / sizeof(word_t);
			size_t bytec = size % sizeof(word_t);

			if (wordc)for (size_t i = 0; i < wordc; i++) {
				*((word_t*)dest) = *((word_t*)src);
				dest =((word_t*)dest)+1; src = ((word_t*)src)+1;
			}
			if (bytec) for (size_t i = 0; i < bytec; i++) {
				*((byte_t*)dest) = *((byte_t*)src);
				dest =((byte_t*)dest)+1; src = ((byte_t*)src)+1;
			}
			return 1;
		}
		return 0;

	}

	inline bool_t Memory_equal(void* dest, const void* src, size_t size) {

		if (dest && src && size) {
			if (size == sizeof(word_t)) {
				return *((word_t*)dest) == *(word_t*)src;
			}
			if (size == sizeof(char_t)) {
				return *((char_t*)dest) == *(char_t*)src;
			}
			size_t wordc = size / sizeof(word_t);
			size_t bytec = size % sizeof(word_t);

			if (wordc)for (size_t i = 0; i < wordc; i++) {
				if (*((word_t*)dest) != *((word_t*)src))return 0;
				dest = ((word_t*)dest) + 1; src = ((word_t*)src) + 1;
			}
			if (bytec) for (size_t i = 0; i < bytec; i++) {
				if (*((byte_t*)dest) != *((byte_t*)src)) return 0;
				dest = ((byte_t*)dest) + 1; src = ((byte_t*)src) + 1;
			}
			return 1;
		}
		return 0;

	}

	inline bool_t Memory_clear(void* dest,  size_t size) {

		if (dest && size) {
			if (size == sizeof(word_t)) {
				*((word_t*)dest) = (word_t)0;
				return 1;
			}
			size_t wordc = size / sizeof(word_t);
			size_t bytec = size % sizeof(word_t);

			if (wordc)for (size_t i = 0; i < wordc; i++) {
				*((word_t*)dest) = (word_t)0;
				dest = ((word_t*)dest) + 1;
			}
			if (bytec) for (size_t i = 0; i < bytec; i++) {
				*((byte_t*)dest) = (byte_t)0;
				dest = ((byte_t*)dest) + 1;
			}
			return 1;
		}
		return 0;

	}

	inline Memory* Memory_default() {
		return Memory_defaultInstance ? Memory_defaultInstance : (Memory_defaultInstance = Memory_construct(Memory_defaultInstance,0));

	}
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __MEMORY_INCLUDED__



