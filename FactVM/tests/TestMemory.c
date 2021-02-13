#include "TestMemory.h"
#include <stdio.h>

void testMemory() {
	printf_s("\n== START TEST: Memory.h ==\n");
	Memory* mm = Memory___construct__(0);
	void* obj4 = mm->require(mm,4,0);
	printf_s("请求4bytes内存%p\n", obj4);

	void* obj8 = mm->require(mm, 8,0);
	printf_s("请求8bytes内存%p\n", obj8);

	void* obj12 = mm->require(mm, 12,0);
	printf_s("请求12bytes内存%p\n", obj12);

	void* obj16 = mm->require(mm, 16,0);
	printf_s("请求16bytes内存%p\n", obj16);


	mm->release(mm, obj4);
	printf_s("强释放4bytes内存\n");

	mm->release(mm, obj8);
	printf_s("弱释放8bytes内存\n");

	mm->release(mm, obj12);
	printf_s("弱释放12bytes内存\n");

	mm->decrease(obj16);
	printf_s("弱释放16bytes内存\n");

	Memory___destruct__(mm);
	printf_s("析构内存管理器\n");

	mm = Memory_default();
	printf_s("获取到默认的内存管理器%p\n",mm);
	mm->destruct(mm);
	printf_s("析构内存管理器\n");

	printf_s("== END TEST: Memory.h ==\n");
}