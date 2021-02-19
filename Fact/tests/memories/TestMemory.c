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

#if defined(__64BITS__)
void testAlignedMemory() {
	memories_module(0);
	TLogger_sectionBegin(0, "TAlignedMemory", "Test start（64bits）...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.allocating = 0;
	opts.initPage = 0;
	opts.totalBytes = 0;
	opts.lookupUnit = 0;

	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	log_assert("TMemory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]%d\n", mm,mm->allocatedBytes);

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

	TLogger_sectionEnd(0, "TAlignedMemory", "Test done!");
}
#endif


#if defined(__32BITS__)
void testAlignedMemory() {
	memories_module(0);
	TLogger_sectionBegin(0, "TAlignedMemory", "Test start（32bits）...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.allocating = 0;
	opts.initPage = 0;
	opts.totalBytes = 0;
	opts.lookupUnit = 0;

	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	log_assert("TMemory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]\r\n共分配%d字节\r\n", mm, mm->allocatedBytes);

	void* obj1 = TAlignedMemory_alloc(mm, 6);
	TAlignedMemoryChunk* chunk = mm->chunks[1];
	log_assert("TMemory.alloc"
		// 在第一个插槽中
		, chunk != 0
		// 分配了一个页面
		&& chunk->page != 0
		// 只有一个内存页
		&& chunk->page->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == sizeof(TAlignedMemory) + sizeof(TAlignedMemoryChunk) + opts.pageSize
		, "分配6个字节的单元obj1[%p]，会对齐到8个字节的单元\r\n构建一个插槽[%p]\r\n分配一个内存页[%p]\r\n共分配%ld字节\n", obj1, chunk,chunk->page,mm->totalBytes);

	log_assert("TMemory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page) + sizeof(TAlignedMemoryPage) + 16)
		, "分配6个字节的单元obj1[%p](total:%d)，会对齐到8个字节的单元\n", obj1, mm->allocatedBytes);

	void* obj2 = TAlignedMemory_alloc(mm, 7);
	log_assert("TMemory.alloc"
		// 在前面那个单元的前面
		, obj2 == ((byte_t*)obj1) - 8
		, "分配7个字节的单元obj2[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	void* obj3 = TAlignedMemory_alloc(mm, 8);
	log_assert("TMemory.alloc"
		// 在前面那个单元的前面
		, obj3 == ((byte_t*)obj2) - 8
		, "分配7个字节的单元obj3[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	
	void* obj4 = TAlignedMemory_alloc(mm, 6);
	usize_t allocatedSize = sizeof(TAlignedMemory) + sizeof(TAlignedMemoryChunk) + opts.pageSize * 2;
	log_assert("TMemory.alloc"
		, chunk->page != 0
		// 有2个内存页
		&& chunk->page->next != 0
		&& chunk->page->next->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == allocatedSize
		, "分配6个字节的单元obj4[%p]，内存翻页\r\n分配更多的内存页[%p]\r\n共分配%ld字节\n", obj4, chunk->page, mm->totalBytes);
	log_assert("TMemory.alloc"
		// 处在最后一个位置
		, obj4 == (((byte_t*)chunk->page) + sizeof(TAlignedMemoryPage) + 16)
		, "新分配的单元在第二个页上(链上是首页)[%p](total:%d)，会对齐到8个字节的单元\n", obj4, mm->allocatedBytes);

	log_assert("TMemory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page->next) + sizeof(TAlignedMemoryPage) + 16)
		, "原先的页被移动到了第二页\n");
	
	TAlignedMemory_free(mm, obj2);
	log_assert("TMemory.free" , obj2 == chunk->page->next->free, "归还内存obj2[%p]，page上放着空闲块链\n",obj2);
	TAlignedMemory_free(mm, obj4);
	log_assert("TMemory.free"
		// 处在最后一个位置
		, obj4 == chunk->page->free
		, "归还内存obj4[%p]，page上放着空闲块链\n", obj4);
	TAlignedMemory_free(mm, obj3);
	log_assert("TMemory.free", obj3 == chunk->page->next->free && obj2==chunk->page->next->free->next, "归还内存obj3[%p]，page上放着空闲块链", obj3);
	TAlignedMemory_free(mm, obj1);
	log_assert("TMemory.free", obj1 == chunk->page->next->free , "归还内存obj1[%p]，page上放着空闲块链", obj1);

	void* obj11 = TAlignedMemory_alloc(mm, 5);
	log_assert("TMemory.alloc", obj11 == obj4, "再次分配内存，使用空闲单元obj4==obj11[%p]", obj11);

	void* obj12 = TAlignedMemory_alloc(mm, 5);
	log_assert("TMemory.alloc", obj12 == (byte_t*)obj4 - 8, "分配内存obj12，占据page2[%p]", obj12);
	void* obj13 = TAlignedMemory_alloc(mm, 5);
	log_assert("TMemory.alloc", obj13 == (byte_t*)obj12 - 8, "分配内存obj13，占据page2[%p]", obj13);
	void* obj14 = TAlignedMemory_alloc(mm, 5);
	log_assert("TMemory.alloc", obj14 == obj1, "分配内存obj14，重复使用pag1上的空闲块obj14==obj1[%p]", obj14);

	AlignedMemoryReleaseInfo rs = TAlignedMemory_collectGarbages(mm, 0);
	log_assert("TMemory.collectGarbages", rs.bytes==0, "垃圾回收，所有页面都没有空闲，无法回收页面\r\n");
	TAlignedMemory_free(mm, obj14);
	rs = TAlignedMemory_collectGarbages(mm, 0);
	log_assert("TMemory.collectGarbages", rs.bytes == opts.pageSize && rs.pages==1, "释放obj14,让page1页面空闲，回收一个页面\r");
	
	TLogger_sectionEnd(0, "TAlignedMemory", "Test done!");
}
#endif