#include "TestMemory.h"
#include "../loggers/Logger.h"

void testMemory() {
	Logger_sectionBegin(0,L"Memory",L"Test start...");
	Memory* mm = Memory___construct__(0,0);
	assert(L"Memory.__construct__", (bool_t)mm, L"�����ڴ������:%p\n", mm);

	int* obj4 = (int*)mm->require(mm,4,0);
	*obj4 = 88;
	assert(L"Memory.require", (bool_t)obj4,L"����4bytes�ڴ�obj4[%p]=%d\n", obj4,*obj4);

	long* obj8 = (long*)mm->require1(mm, 8,0);
	*obj8 = 1234567890123456789;
	assert(L"Memory.require1", (bool_t)obj8,L"����8bytes�ڴ�obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = mm->require(mm, 12,0);
	assert(L"Memory.require", (bool_t)obj12,L"����12bytes�ڴ�obj12[%p]\n", obj12);

	void* obj16 = mm->require(mm, 16,0);
	assert(L"Memory.require", (bool_t)obj16,L"����16bytes�ڴ�obj16[%p]\n", obj16);

	mm->increase(mm, obj4);
	assert(L"Memory.increase", (bool_t)*obj4, L"��������obj4[%p]=%d\n", obj4,*obj4);

	mm->decrease(mm, obj4);
	assert(L"Memory.decrease", (bool_t)*obj4, L"��������obj4[%p]=%d\n", obj4,*obj4);

	mm->release(mm, obj4);
	assert(L"Memory.release", (bool_t)obj4, L"�ͷ��ڴ�obj4[%p]=%d\n", obj4);

	mm->release(mm, obj8);
	assert(L"Memory.release", (bool_t)obj8, L"�ͷ��ڴ�obj8[%p]=%ld\n", obj8,*obj8);

	mm->release(mm, obj12);
	assert(L"Memory.release", (bool_t)obj12, L"�ͷ��ڴ�obj12[%p]\n", obj12);

	mm->release(mm, obj16);
	assert(L"Memory.release", (bool_t)obj16, L"�ͷ��ڴ�obj12[%p]\n", obj16);

	Memory___destruct__(mm,0);
	assert(L"Memory.__destruct__", (bool_t)mm, L"�����ڴ������\n", mm);

	mm = Memory_default();
	assert(L"Memory.default", mm && mm== Memory_defaultInstance, L"Ĭ���ڴ������%p\n", mm);
	Memory___destruct__(mm, 0);
	assert(L"Memory.__destruct__", Memory_defaultInstance==0, L"����Ĭ�ϵ��ڴ������\n", mm);

	Logger_sectionEnd(0, L"Memory", L"Test done!");
}