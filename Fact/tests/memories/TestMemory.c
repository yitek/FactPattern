#include "TestMemory.h"
#include "../../loggers/TLogger.h"

void testMemory() {
	memories_module(0);
	TLogger_sectionBegin(0,"TMemory","Test start...");
	TMemory* mm = TMemory__construct__(0,0,TLogger_default);
	log_assert("TMemory.__construct__", (bool_t)mm, "构造内存管理器:%p\n", mm);

	int* obj4 = (int*)TMemory_alloc(mm,4);
	*obj4 = 88;
	log_assert("TMemory.require", (bool_t)obj4,"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long_t* obj8 = (long_t*)TMemory_alloc__virtual__(mm, 8);
	*obj8 = 9876543210;
	log_assert("TMemory.require1", (bool_t)obj8,"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = TMemory_alloc1(mm,12);
	log_assert("TMemory.require", (bool_t)obj12,"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = TMemory_alloc1__virtual__(mm, 12);
	log_assert("TMemory.require", (bool_t)obj16,"请求16bytes内存obj16[%p]\n", obj16);
	

	TMemory_free(mm, obj4);
	log_assert("TMemory.release", (bool_t)obj4, "释放内存obj4[%p]=%d\n", obj4);

	TMemory_free(mm, obj8);
	log_assert("TMemory.release", (bool_t)obj8, "释放内存obj8[%p]=%ld\n", obj8,*obj8);

	TMemory_free__virtual__(mm, obj12);
	log_assert("TMemory.release", (bool_t)obj12, "释放内存obj12[%p]\n", obj12);

	TMemory_free__virtual__(mm, obj16);
	log_assert("TMemory.release", (bool_t)obj16, "释放内存obj12[%p]\n", obj16);

	TMemory__destruct__(mm,0);
	log_assert("TMemory.__destruct__", (bool_t)mm, "析构内存管理器[%p]\n", mm);

	mm = TMemory_default;
	log_assert("TMemory.default", mm && mm== TMemory_default, "获取到默认内存管理器[%p]\n", mm);
	TMemory__destruct__(mm, 0);
	log_assert("TMemory.__destruct__", (bool_t)TMemory_default, "不能释放默认的内存管理器\n", mm);

	TLogger_sectionEnd(0, "TMemory", "Test done!");
}