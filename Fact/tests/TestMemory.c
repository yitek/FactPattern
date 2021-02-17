#include "TestMemory.h"
#include "../loggers/TLogger.h"

void testMemory() {
	TLogger_sectionBegin(0,"Memory","Test start...");
	Memory* mm = Memory_construct(0,0);
	assert("Memory.__construct__", (bool_t)mm, "构造内存管理器:%p\n", mm);

	int* obj4 = (int*)mm->require(mm,4,0);
	*obj4 = 88;
	assert("Memory.require", (bool_t)obj4,"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long* obj8 = (long*)mm->require1(mm, 8,0);
	*obj8 = 9876543210;
	assert("Memory.require1", (bool_t)obj8,"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = mm->require(mm, 12,0);
	assert("Memory.require", (bool_t)obj12,"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = mm->require(mm, 16,0);
	assert("Memory.require", (bool_t)obj16,"请求16bytes内存obj16[%p]\n", obj16);

	mm->increase(mm, obj4);
	assert("Memory.increase", (bool_t)*obj4, "引用增加obj4[%p]=%d\n", obj4,*obj4);

	mm->decrease(mm, obj4);
	assert("Memory.decrease", (bool_t)*obj4, "引用增加obj4[%p]=%d\n", obj4,*obj4);

	mm->release(mm, obj4);
	assert("Memory.release", (bool_t)obj4, "释放内存obj4[%p]=%d\n", obj4);

	mm->release(mm, obj8);
	assert("Memory.release", (bool_t)obj8, "释放内存obj8[%p]=%ld\n", obj8,*obj8);

	mm->release(mm, obj12);
	assert("Memory.release", (bool_t)obj12, "释放内存obj12[%p]\n", obj12);

	mm->release(mm, obj16);
	assert("Memory.release", (bool_t)obj16, "释放内存obj12[%p]\n", obj16);

	Memory_destruct(mm,0);
	assert("Memory.__destruct__", (bool_t)mm, "析构内存管理器\n", mm);

	mm = Memory_default();
	assert("Memory.default", mm && mm== Memory_defaultInstance, "默认内存管理器%p\n", mm);
	Memory_destruct(mm, 0);
	assert("Memory.__destruct__", Memory_defaultInstance==0, "析构默认的内存管理器\n", mm);

	TLogger_sectionEnd(0, "Memory", "Test done!");
}