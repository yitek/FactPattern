#include "TestMemory.h"
#include "../../loggers/Logger.h"

void testMemory() {
	memories_module(0,0,0);
	Logger_sectionBegin(0,"Memory","Test start...");
	Memory* mm = Memory__construct__(0,0,Logger_default);
	log_assert("Memory.__construct__", mm!=0, "构造内存管理器:%p\n", mm);

	int* obj4 = (int*)Memory_alloc(mm,4, MemoryKind_normal);
	*obj4 = 88;
	log_assert("Memory.require", obj4!=0,"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long_t* obj8 = (long_t*)Memory_alloc__virtual__(mm,  8, MemoryKind_normal);
	*obj8 = 9876543210;
	log_assert("Memory.require1", obj8 != 0,"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = Memory_alloc(mm,12, MemoryKind_readonly);
	log_assert("Memory.require", obj12 != 0,"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = Memory_alloc__virtual__(mm, 12, MemoryKind_normal);
	log_assert("Memory.require", obj16 != 0,"请求16bytes内存obj16[%p]\n", obj16);
	

	Memory_free(mm, obj4);
	log_assert("Memory.release", obj4 != 0, "释放内存obj4[%p]=%d\n", obj4);

	Memory_free(mm, obj8);
	log_assert("Memory.release", obj8 != 0, "释放内存obj8[%p]=%ld\n", obj8,*obj8);

	Memory_free__virtual__(mm, obj12);
	log_assert("Memory.release", obj12!=0, "释放内存obj12[%p]\n", obj12);

	Memory_free__virtual__(mm, obj16);
	log_assert("Memory.release", obj16 != 0, "释放内存obj12[%p]\n", obj16);

	Memory__destruct__(mm,0);
	log_assert("Memory.__destruct__", mm != 0, "析构内存管理器[%p]\n", mm);

	mm = Memory_default;
	log_assert("Memory.default", mm && mm== Memory_default, "获取到默认内存管理器[%p]\n", mm);
	Memory__destruct__(mm, 0);
	log_assert("Memory.__destruct__", Memory_default!=0, "不能释放默认的内存管理器\n", mm);

	Logger_sectionEnd(0, "Memory", "Test done!");
}

#if defined(__64BITS__)
void testAlignedMemory() {
	memories_module(0,0,0);
	Logger_sectionBegin(0, "AlignedMemory", "Test start（64bits）...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_link;

	AlignedMemory* mm = AlignedMemory__construct__(0, &opts, Logger_default);
	log_assert("Memory.__construct__", mm && mm->allocatedBytes == sizeof(AlignedMemory), "构造对齐的内存管理器:[%p]%d\n", mm,mm->allocatedBytes);

	void* obj1 = AlignedMemory_alloc(mm,6, MemoryKind_none);
	log_assert("Memory.alloc"
		// 在第一个插槽中
		, mm->chunks[0]!=0 
		// 分配了一个页面
		&& mm->chunks[0]->page!=0 
		// 只有一个内存页
		&& mm->chunks[0]->page->next==0 
		// 处在最后一个位置
		&& obj1==(byte_t*)(mm->chunks[0]->page)+sizeof(AlignedMemoryPage)+8
		, "分配6个字节的单元[%p](total:%d)，会对齐到8个字节的单元\n", obj1,mm->allocatedBytes);

	void* obj2 = AlignedMemory_alloc(mm,7, MemoryKind_none);
	log_assert("Memory.alloc"
		// 在前面那个单元的前面
		, obj2==((byte_t*)obj1)-sizeof(addr_t)
		, "分配7个字节的单元[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2,mm->allocatedBytes);

	void* obj3 = AlignedMemory_alloc(mm,6, MemoryKind_none);
	log_assert("Memory.alloc"
		// 翻页，有第二页
		, mm->chunks[0]->page->next!=0 
		// 第二页没有后继页面
		&& mm->chunks[0]->page->next->next==0
		// 分配的内存在第二个页面上(后申请的页面在链的前面)
		&& obj3==(byte_t*)(mm->chunks[0]->page)+sizeof(AlignedMemoryPage)+8 
		// 将原先的内存页移到了第二位
		&& (((byte_t*)mm->chunks[0]->page->next) + sizeof(AlignedMemoryPage)) == obj2
		, "分配6个字节的单元[%p](total:%ld)，超过了一页的范围，内存翻页\n"
		, obj3,mm->allocatedBytes);

	AlignedMemory_free(mm,obj1);
	AlignedMemory_free(mm, obj3);

	void* obj4 = AlignedMemory_alloc(mm, 8, MemoryKind_none);
	log_assert("Memory.re-alloc",obj4==obj3,"应该在被释放的内存上obj4==obj3");
	void* obj5 = AlignedMemory_alloc(mm, 8, MemoryKind_none);
	log_assert("Memory.re-alloc", obj5 == obj4-8, "内存是连续的obj5==obj4-8");
	void* obj6 = AlignedMemory_alloc(mm,6, MemoryKind_none);
	log_assert("Memory.re-alloc", obj6 == obj1, "应该在被释放的内存上obj6==obj1");
	AlignedMemoryReleaseInfo rs = AlignedMemory_collectGarbages(mm,1 ,0);
	log_assert("Memory.collectGarbages", rs.bytes==0, "垃圾回收，所有页面都没有空闲，无法回收页面");
	AlignedMemory_free(mm, obj6);
	AlignedMemory_free(mm, obj2);
	rs = AlignedMemory_collectGarbages(mm, 1,0);
	log_assert("Memory.collectGarbages", rs.bytes == opts.pageSize && rs.pages==1, "释放obj6,obj2,让page1页面空闲，回收一个页面");

	Logger_sectionEnd(0, "AlignedMemory", "Test done!");
}
#endif


#if defined(__32BITS__)
void testAlignedLinkUnitMemory() {
	Logger_sectionBegin(0, "AlignedMemory(LINK unit,32bits)", "Test start...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_link;
	AlignedMemory* mm = AlignedMemory__construct__(0, &opts, Logger_default);
	log_assert("Memory.__construct__", mm && mm->allocatedBytes == sizeof(AlignedMemory), "构造对齐的内存管理器:[%p]\r\n共分配%d字节\r\n", mm, mm->allocatedBytes);

	void* obj1 = AlignedMemory_alloc(mm, 6, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj1+1);
	AlignedMemoryChunk* chunk = mm->chunks[1];
	log_assert("Memory.alloc"
		// 在第一个插槽中
		, chunk != 0
		// 分配了一个页面
		&& chunk->page != 0
		// 只有一个内存页
		&& chunk->page->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == sizeof(AlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize
		, "分配6个字节的单元obj1[%p]，会对齐到8个字节的单元\r\n构建一个插槽[%p]\r\n分配一个内存页[%p]\r\n共分配%ld字节\n", obj1, chunk,chunk->page,mm->allocatedBytes);

	log_assert("Memory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page) + sizeof(AlignedMemoryPage) + 16)
		, "分配6个字节的单元obj1[%p](total:%d)，会对齐到8个字节的单元\n", obj1, mm->allocatedBytes);

	void* obj2 = AlignedMemory_alloc(mm, 7, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj2 + 1);
	log_assert("Memory.alloc"
		// 在前面那个单元的前面
		, obj2 == ((byte_t*)obj1) - 8
		, "分配7个字节的单元obj2[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	void* obj3 = AlignedMemory_alloc(mm, 8, MemoryKind_normal);
	ref_increase((MemoryRefUnit*)obj3 + 1);
	log_assert("Memory.alloc"
		// 在前面那个单元的前面
		, obj3 == ((byte_t*)obj2) - 8
		, "分配7个字节的单元obj3[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj3, mm->allocatedBytes);

	
	void* obj4 = AlignedMemory_alloc(mm, 6, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj4 + 1);
	usize_t allocatedSize = sizeof(AlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize * 2;
	log_assert("Memory.alloc"
		, chunk->page != 0
		// 有2个内存页
		&& chunk->page->next != 0
		&& chunk->page->next->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == allocatedSize
		, "分配6个字节的单元obj4[%p]，内存翻页\r\n分配更多的内存页[%p]\r\n共分配%ld字节\n", obj4, chunk->page, mm->allocatedBytes);
	log_assert("Memory.alloc"
		// 处在最后一个位置
		, obj4 == (((byte_t*)chunk->page) + sizeof(AlignedMemoryPage) + 16)
		, "新分配的单元在第二个页上(链上是首页)[%p](total:%d)，会对齐到8个字节的单元\n", obj4, mm->allocatedBytes);

	log_assert("Memory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page->next) + sizeof(AlignedMemoryPage) + 16)
		, "原先的页被移动到了第二页\n");
	
	AlignedMemory_free(mm, obj2);
	log_assert("Memory.free" , obj2 == chunk->page->next->free, "归还内存obj2[%p]，page上放着空闲块链\n",obj2);
	AlignedMemory_free(mm, obj4);
	log_assert("Memory.free"
		// 处在最后一个位置
		, obj4 == chunk->page->free
		, "归还内存obj4[%p]，page上放着空闲块链\n", obj4);
	AlignedMemory_free(mm, obj3);
	log_assert("Memory.free", obj3 == chunk->page->next->free && obj2==chunk->page->next->free->next, "归还内存obj3[%p]，page上放着空闲块链", obj3);
	AlignedMemory_free(mm, obj1);
	log_assert("Memory.free", obj1 == chunk->page->next->free , "归还内存obj1[%p]，page上放着空闲块链", obj1);

	void* obj11 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj11 + 1);
	log_assert("Memory.alloc", obj11 == obj4, "再次分配内存，使用空闲单元obj4==obj11[%p]", obj11);

	void* obj12 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj12 + 1);
	log_assert("Memory.alloc", obj12 == (byte_t*)obj4 - 8, "分配内存obj12，占据page2[%p]", obj12);
	void* obj13 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj13 + 1);
	log_assert("Memory.alloc", obj13 == (byte_t*)obj12 - 8, "分配内存obj13，占据page2[%p]", obj13);
	void* obj14 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	log_assert("Memory.alloc", obj14 == obj1, "分配内存obj14，重复使用pag1上的空闲块obj14==obj1[%p]", obj14);
	ref_inc((MemoryRefUnit*)obj14 + 1);

	AlignedMemoryReleaseInfo rs = AlignedMemory_collectGarbages(mm,1, 0);
	log_assert("Memory.collectGarbages", rs.bytes==0, "垃圾回收，所有页面都没有空闲，无法回收页面\r\n");
	AlignedMemory_free(mm, obj14);
	rs = AlignedMemory_collectGarbages(mm, 1,0);
	log_assert("Memory.collectGarbages", rs.bytes == opts.pageSize && rs.pages==1, "释放obj14,让page1页面空闲，回收一个页面\r");
	log_assert("Memory.collectGarbages", chunk->page && chunk->page->next==0 && chunk->page+1== obj13, "只有一个页面\r");

	bool_t sfreeRs = AlignedMemory_sfree(mm,obj13);
	log_assert("Memory.sfree", sfreeRs && obj13==0, "安全释放宏，由于原先的内存放着空链指针，不可以再使用，释放后原先的指针应该不能被使用\t");

	Logger_sectionEnd(0, "AlignedMemory(Link unit,32bits)", "Test done!");
	
}


void testAlignedRefUnitMemory() {
	Logger_sectionBegin(0, "AlignedMemory(REF unit,32bits)", "Test start...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_ref;
	AlignedMemory* mm = AlignedMemory__construct__(0, &opts, Logger_default);
	log_assert("Memory.__construct__", mm && mm->allocatedBytes == sizeof(AlignedMemory), "构造对齐的内存管理器:[%p]\r\n共分配%d字节\r\n", mm, mm->allocatedBytes);

	void* obj1 = AlignedMemory_alloc(mm, 6, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj1 + 1);
	AlignedMemoryChunk* chunk = mm->chunks[1];
	log_assert("Memory.alloc"
		// 在第一个插槽中
		, chunk != 0
		// 分配了一个页面
		&& chunk->page != 0
		// 只有一个内存页
		&& chunk->page->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == sizeof(AlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize
		, "分配6个字节的单元obj1[%p]，会对齐到8个字节的单元\r\n构建一个插槽[%p]\r\n分配一个内存页[%p]\r\n共分配%ld字节\n", obj1, chunk, chunk->page, mm->allocatedBytes);

	log_assert("Memory.alloc"
		// 处在第一个位置
		, obj1 == (((byte_t*)&chunk->page->free))
		// 引用计数为1
		&& ((MemoryRefUnit*)obj1)->ref==1
		, "分配6个字节的单元obj1[%p](total:%d)，会对齐到8个字节的单元\n", obj1, mm->allocatedBytes);

	void* obj2 = AlignedMemory_alloc(mm, 7, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj2 + 1);
	log_assert("Memory.alloc"
		// 在前面那个单元的前面
		, obj2 == ((byte_t*)obj1) + 8
		, "分配7个字节的单元obj2[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	void* obj3 = AlignedMemory_alloc(mm, 8, MemoryKind_normal);
	ref_increase((MemoryRefUnit*)obj3 + 1);
	log_assert("Memory.alloc"
		// 在前面那个单元的前面
		, obj3 == ((byte_t*)obj2) + 8
		, "分配7个字节的单元obj3[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj3, mm->allocatedBytes);


	void* obj4 = AlignedMemory_alloc(mm, 6, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj4 + 1);
	usize_t allocatedSize = sizeof(AlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize * 2;
	log_assert("Memory.alloc"
		, chunk->page != 0
		// 有2个内存页
		&& chunk->page->next != 0
		&& chunk->page->next->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == allocatedSize
		, "分配6个字节的单元obj4[%p]，内存翻页\r\n分配更多的内存页[%p]\r\n共分配%ld字节\n", obj4, chunk->page, mm->allocatedBytes);
	log_assert("Memory.alloc"
		// 处在最后一个位置
		, obj4 == (((byte_t*)&chunk->page->free))
		, "新分配的单元在第二个页上(链上是首页)[%p](total:%d)，会对齐到8个字节的单元\n", obj4, mm->allocatedBytes);

	log_assert("Memory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)&chunk->page->next->free))
		, "原先的页被移动到了第二页\n");

	AlignedMemory_free(mm, obj2);
	log_assert("Memory.free", ((MemoryRefUnit*)obj2)->ref==0 , "归还内存obj2[%p]，引用计数置为0\n", obj2);
	AlignedMemory_free(mm, obj4);
	log_assert("Memory.free"
		// 处在最后一个位置
		, ((MemoryRefUnit*)obj4)->ref == 0
		, "归还内存obj4[%p]，引用计数置为0\n", obj4);
	void* obj11 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj11 + 1);
	log_assert("Memory.alloc", obj11 == obj4, "再次分配内存obj11，使用空闲单元obj4==obj11[%p]", obj11);

	void* obj12 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj12 + 1);
	log_assert("Memory.alloc", obj12 == (byte_t*)obj11+8, "分配内存obj12，占用obj11后面的空间[%p]", obj12);

	void* obj13 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj13 + 1);
	log_assert("Memory.alloc", obj13 == (byte_t*)obj12 + 8, "分配内存obj13，占用obj12后面的空间[%p]", obj12);

	void* obj14 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj14 + 1);
	log_assert("Memory.alloc", obj14 == obj2, "分配内存obj13，占用obj2原先的空间[%p]", obj12);

	AlignedMemory_free(mm, obj3);
	log_assert("Memory.free", ((MemoryRefUnit*)obj3)->ref == 0, "归还内存obj3[%p]，引用计数置0", obj3);
	AlignedMemory_free(mm, obj1);
	log_assert("Memory.free", ((MemoryRefUnit*)obj3)->ref == 0, "归还内存obj1[%p]，引用计数置0", obj1);

	void* obj15 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj15 + 1);
	log_assert("Memory.alloc", obj15 == obj1, "分配内存obj15，obj15 == obj1[%p]", obj15);
	void* obj16 = AlignedMemory_alloc(mm, 5, MemoryKind_normal);
	ref_inc((MemoryRefUnit*)obj16 + 1);
	log_assert("Memory.alloc", obj16 == obj3, "分配内存obj14，重复使用pag1上的空闲块obj16==obj3[%p]", obj16);
	
	
	AlignedMemoryReleaseInfo rs = AlignedMemory_collectGarbages(mm, 1, 0);
	log_assert("Memory.collectGarbages", rs.bytes == 0, "垃圾回收，所有页面都没有空闲，无法回收页面\r\n");
	AlignedMemory_free(mm, obj14);
	AlignedMemory_free(mm, obj15);
	AlignedMemory_free(mm, obj16);
	rs = AlignedMemory_collectGarbages(mm, 1, 0);
	log_assert("Memory.collectGarbages", rs.bytes == opts.pageSize && rs.pages == 1, "释放obj14,让page1页面空闲，回收一个页面\r");
	log_assert("Memory.collectGarbages", chunk->page && chunk->page->next == 0 && (&chunk->page->free == obj11), "只有一个页面\r");

	bool_t sfreeRs = AlignedMemory_sfree(mm, obj13);
	log_assert("Memory.sfree", sfreeRs && obj13 == 0, "安全释放宏，由于原先的内存放着空链指针，不可以再使用，释放后原先的指针应该不能被使用\t");

	Logger_sectionEnd(0, "AlignedMemory(REF unit,32bits)", "Test done!");
	
}

void testAlignedMemory() {
	memories_module(0, 0, 0);

	//testAlignedLinkUnitMemory();
	testAlignedRefUnitMemory();
}
#endif