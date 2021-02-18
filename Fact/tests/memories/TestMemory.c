#include "TestMemory.h"
#include "../../loggers/TLogger.h"

void testMemory() {
	memories_module(0);
	TLogger_sectionBegin(0,"TMemory","Test start...");
	TMemory* mm = TMemory__construct__(0,0,TLogger_default);
	log_assert("TMemory.__construct__", mm!=0, "构造内存管理器:%p\n", mm);

	int* obj4 = (int*)TMemory_alloc(mm,4);
	*obj4 = 88;
	log_assert("TMemory.require", obj4!=0,"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long_t* obj8 = (long_t*)TMemory_alloc__virtual__(mm, 8);
	*obj8 = 9876543210;
	log_assert("TMemory.require1", obj8 != 0,"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = TMemory_alloc1(mm,12);
	log_assert("TMemory.require", obj12 != 0,"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = TMemory_alloc1__virtual__(mm, 12);
	log_assert("TMemory.require", obj16 != 0,"请求16bytes内存obj16[%p]\n", obj16);
	

	TMemory_free(mm, obj4);
	log_assert("TMemory.release", obj4 != 0, "释放内存obj4[%p]=%d\n", obj4);

	TMemory_free(mm, obj8);
	log_assert("TMemory.release", obj8 != 0, "释放内存obj8[%p]=%ld\n", obj8,*obj8);

	TMemory_free__virtual__(mm, obj12);
	log_assert("TMemory.release", obj12!=0, "释放内存obj12[%p]\n", obj12);

	TMemory_free__virtual__(mm, obj16);
	log_assert("TMemory.release", obj16 != 0, "释放内存obj12[%p]\n", obj16);

	TMemory__destruct__(mm,0);
	log_assert("TMemory.__destruct__", mm != 0, "析构内存管理器[%p]\n", mm);

	mm = TMemory_default;
	log_assert("TMemory.default", mm && mm== TMemory_default, "获取到默认内存管理器[%p]\n", mm);
	TMemory__destruct__(mm, 0);
	log_assert("TMemory.__destruct__", TMemory_default!=0, "不能释放默认的内存管理器\n", mm);

	TLogger_sectionEnd(0, "TMemory", "Test done!");
}
void testAlignedMemory() {
	memories_module(0);
	TLogger_sectionBegin(0, "TAlignedMemory", "Test start...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.allocating = 0;
	opts.initPage = 0;
	opts.totalBytes = 0;
	opts.lookupUnit = 0;

	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	log_assert("TMemory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]%d\n", mm,mm->allocatedBytes);
#if defined(__64BITS__)
	void* obj1 = TAlignedMemory_alloc(mm,6);
	log_assert("TMemory.alloc"
		// 在第一个插槽中
		, mm->chunks[0]!=0 
		// 分配了一个页面
		&& mm->chunks[0]->page!=0 
		// 只有一个内存页
		&& mm->chunks[0]->page->next==0 
		// 处在最后一个位置
		&& obj1==(byte_t*)(mm->chunks[0]->page)+sizeof(TAlignedMemoryPage)+8
		, "分配6个字节的单元[%p](total:%d)，会对齐到8个字节的单元\n", obj1,mm->allocatedBytes);

	void* obj2 = TAlignedMemory_alloc(mm,7);
	log_assert("TMemory.alloc"
		// 在前面那个单元的前面
		, obj2==((byte_t*)obj1)-sizeof(addr_t)
		, "分配7个字节的单元[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2,mm->allocatedBytes);

	void* obj3 = TAlignedMemory_alloc(mm,6);
	log_assert("TMemory.alloc"
		// 翻页，有第二页
		, mm->chunks[0]->page->next!=0 
		// 第二页没有后继页面
		&& mm->chunks[0]->page->next->next==0
		// 分配的内存在第二个页面上(后申请的页面在链的前面)
		&& obj3==(byte_t*)(mm->chunks[0]->page)+sizeof(TAlignedMemoryPage)+8 
		// 将原先的内存页移到了第二位
		&& (((byte_t*)mm->chunks[0]->page->next) + sizeof(TAlignedMemoryPage)) == obj2
		, "分配6个字节的单元[%p](total:%ld)，超过了一页的范围，内存翻页\n"
		, obj3,mm->allocatedBytes);

	TAlignedMemory_free(mm,obj1);
#endif
	TLogger_sectionEnd(0, "TAlignedMemory", "Test done!");
}