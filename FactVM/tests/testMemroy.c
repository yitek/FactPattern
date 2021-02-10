#include "TestMemory.h"
#include <stdio.h>

void testMemory() {
	printf_s("== START TEST: Memory.h ==\n");
	Memory* mm = Memory___construct__(0);
	void* obj4 = mm->require(mm,4);
	printf_s("����4bytes�ڴ�%p\n", obj4);

	void* obj8 = Memory_require(mm,8);
	printf_s("����8bytes�ڴ�%p\n", obj8);

	void* obj12 = Memory_require(mm, 12);
	printf_s("����12bytes�ڴ�%p\n", obj12);

	void* obj16 = Memory_require(mm, 16);
	printf_s("����16bytes�ڴ�%p\n", obj16);


	Memory_release(mm,obj4);
	printf_s("ǿ�ͷ�4bytes�ڴ�\n");

	mm->release(mm, obj8);
	printf_s("���ͷ�8bytes�ڴ�\n");

	Memory_weekRelease(mm, obj12);
	printf_s("���ͷ�12bytes�ڴ�\n");

	mm->weekRelease(mm, obj16);
	printf_s("���ͷ�16bytes�ڴ�\n");

	Memory___destruct__(mm);
	printf_s("�����ڴ������\n");

	mm = Memory_default();
	printf_s("��ȡ��Ĭ�ϵ��ڴ������%p\n",mm);
	mm->destruct(mm);
	printf_s("�����ڴ������\n");

	printf_s("== END TEST: Memory.h ==\n");
}