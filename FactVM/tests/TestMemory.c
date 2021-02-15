#include "TestMemory.h"
#include "../loggers/Logger.h"

void testMemory() {
	Logger_sectionBegin(0,L"Memory",L"Test start...");
	Memory* mm = Memory___construct__(0,0);
	assert(L"Memory.__construct__", (bool_t)mm, L"构造内存管理器:%p\n", mm);

	int* obj4 = (int*)mm->require(mm,4,0);
	*obj4 = 88;
	assert(L"Memory.require", (bool_t)obj4,L"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long* obj8 = (long*)mm->require1(mm, 8,0);
	*obj8 = 1234567890123456789;
	assert(L"Memory.require1", (bool_t)obj8,L"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = mm->require(mm, 12,0);
	assert(L"Memory.require", (bool_t)obj12,L"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = mm->require(mm, 16,0);
	assert(L"Memory.require", (bool_t)obj16,L"请求16bytes内存obj16[%p]\n", obj16);

	mm->increase(mm, obj4);
	assert(L"Memory.increase", (bool_t)*obj4, L"引用增加obj4[%p]=%d\n", obj4,*obj4);

	mm->decrease(mm, obj4);
	assert(L"Memory.decrease", (bool_t)*obj4, L"引用增加obj4[%p]=%d\n", obj4,*obj4);

	mm->release(mm, obj4);
	assert(L"Memory.release", (bool_t)obj4, L"释放内存obj4[%p]=%d\n", obj4);

	mm->release(mm, obj8);
	assert(L"Memory.release", (bool_t)obj8, L"释放内存obj8[%p]=%ld\n", obj8,*obj8);

	mm->release(mm, obj12);
	assert(L"Memory.release", (bool_t)obj12, L"释放内存obj12[%p]\n", obj12);

	mm->release(mm, obj16);
	assert(L"Memory.release", (bool_t)obj16, L"释放内存obj12[%p]\n", obj16);

	Memory___destruct__(mm,0);
	assert(L"Memory.__destruct__", (bool_t)mm, L"析构内存管理器\n", mm);

	mm = Memory_default();
	assert(L"Memory.default", mm && mm== Memory_defaultInstance, L"默认内存管理器%p\n", mm);
	Memory___destruct__(mm, 0);
	assert(L"Memory.__destruct__", Memory_defaultInstance==0, L"析构默认的内存管理器\n", mm);

	Logger_sectionEnd(0, L"Memory", L"Test done!");
}