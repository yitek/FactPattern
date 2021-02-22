#include "TestMemory.h"

void testMemory() {
	memories_module(0,0,0);
	Test_begin("TMemory","Test start...");
	TMemory* mm = TMemory__construct__(0);
	Test_assert("TMemory.__construct__", mm!=0, "构造内存管理器:%p\n", mm);

	int* obj4 = (int*)TMemory_alloc(mm,4,0, MemoryKind_normal);
	*obj4 = 88;
	Test_assert("TMemory.require", obj4!=0,"请求4bytes内存obj4[%p]=%d\n", obj4,*obj4);

	long_t* obj8 = (long_t*)m_alloc(mm,  8,0, MemoryKind_normal);
	*obj8 = 9876543210;
	Test_assert("TMemory.require1", obj8 != 0,"请求8bytes内存obj8[%p]=%ld\n", obj8,*obj8);

	void* obj12 = TMemory_alloc(mm,12, 0, MemoryKind_readonly);
	Test_assert("Memory.require", obj12 != 0,"请求12bytes内存obj12[%p]\n", obj12);

	void* obj16 = m_alloc(mm, 12, 0,MemoryKind_normal);
	Test_assert("TMemory.require", obj16 != 0,"请求16bytes内存obj16[%p]\n", obj16);
	

	TMemory_free(mm, obj4);
	Test_assert("Memory.release", obj4 != 0, "释放内存obj4[%p]=%d\n", obj4);

	TMemory_free(mm, obj8);
	Test_assert("TMemory.release", obj8 != 0, "释放内存obj8[%p]=%ld\n", obj8,*obj8);

	m_free(mm, obj12);
	Test_assert("TMemory.release", obj12!=0, "释放内存obj12[%p]\n", obj12);

	m_free(mm, obj16);
	Test_assert("TMemory.release", obj16 != 0, "释放内存obj12[%p]\n", obj16);

	TMemory__destruct__(mm,0);
	Test_assert("Memory.__destruct__", mm != 0, "析构内存管理器[%p]\n", mm);

	mm = TMemory_default;
	Test_assert("TMemory.default", mm && mm== TMemory_default, "获取到默认内存管理器[%p]\n", mm);
	TMemory__destruct__(mm, 0);
	Test_assert("TMemory.__destruct__", TMemory_default!=0, "不能释放默认的内存管理器\n", mm);

	Test_end();
}

#if defined(__64BITS__)
void testAlignedMemory() {
	memories_module(0,0,0);
	Test_begin("TAlignedMemory", "Test start（64bits）...");
	AlignedMemoryOptions opts;
	opts.pageSize = 42;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_link;

	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	Test_assert("Memory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]%d\n", mm,mm->allocatedBytes);

	void* obj1 = TAlignedMemory_alloc(mm,6, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.alloc"
		// 在第一个插槽中
		, mm->chunks[0]!=0 
		// 分配了一个页面
		&& mm->chunks[0]->page!=0 
		// 只有一个内存页
		&& mm->chunks[0]->page->next==0 
		// 处在最后一个位置
		&& obj1==(byte_t*)(mm->chunks[0]->page)+sizeof(AlignedMemoryPage)+8
		, "分配6个字节的单元[%p](total:%d)，会对齐到8个字节的单元\n", obj1,mm->allocatedBytes);

	void* obj2 = TAlignedMemory_alloc(mm,7, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.alloc"
		// 在前面那个单元的前面
		, obj2==((byte_t*)obj1)-sizeof(addr_t)
		, "分配7个字节的单元[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2,mm->allocatedBytes);

	void* obj3 = TAlignedMemory_alloc(mm,6, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.alloc"
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

	TAlignedMemory_free(mm,obj1);
	TAlignedMemory_free(mm, obj3);

	void* obj4 = TAlignedMemory_alloc(mm, 8, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.re-alloc",obj4==obj3,"应该在被释放的内存上obj4==obj3");
	void* obj5 = TAlignedMemory_alloc(mm, 8, 0, MemoryKind_normal);
	Test_assert("Memory.re-alloc", obj5 == obj4-8, "内存是连续的obj5==obj4-8");
	void* obj6 = TAlignedMemory_alloc(mm,6, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.re-alloc", obj6 == obj1, "应该在被释放的内存上obj6==obj1");
	AlignedMemoryReleaseInfo rs = TAlignedMemory_collectGarbages(mm,1 ,0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes==0, "垃圾回收，所有页面都没有空闲，无法回收页面");
	TAlignedMemory_free(mm, obj6);
	TAlignedMemory_free(mm, obj2);
	rs = TAlignedMemory_collectGarbages(mm, 1,0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes == opts.pageSize && rs.pages==1, "释放obj6,obj2,让page1页面空闲，回收一个页面");

	Test_end();
}
#endif


#if defined(__32BITS__)
void testAlignedLinkUnitMemory() {
	Test_begin("TAlignedMemory(LINK unit,32bits)", "Test start...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_link;
	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	Test_assert("TAlignedMemory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]\r\n共分配%d字节\r\n", mm, mm->allocatedBytes);

	void* obj1 = TAlignedMemory_alloc(mm, 6,0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj1+1);
	AlignedMemoryChunk* chunk = mm->chunks[1];
	Test_assert("TAlignedMemory.alloc"
		// 在第一个插槽中
		, chunk != 0
		// 分配了一个页面
		&& chunk->page != 0
		// 只有一个内存页
		&& chunk->page->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == sizeof(TAlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize
		, "分配6个字节的单元obj1[%p]，会对齐到8个字节的单元\r\n构建一个插槽[%p]\r\n分配一个内存页[%p]\r\n共分配%ld字节\n", obj1, chunk,chunk->page,mm->allocatedBytes);

	Test_assert("TAlignedMemory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page) + sizeof(AlignedMemoryPage) + 16)
		, "分配6个字节的单元obj1[%p](total:%d)，会对齐到8个字节的单元\n", obj1, mm->allocatedBytes);

	void* obj2 = TAlignedMemory_alloc(mm, 7,0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj2 + 1);
	Test_assert("TAlignedMemory.alloc"
		// 在前面那个单元的前面
		, obj2 == ((byte_t*)obj1) - 8
		, "分配7个字节的单元obj2[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	void* obj3 = TAlignedMemory_alloc(mm, 8, 0, MemoryKind_normal);
	ref_increase((MRefUnit*)obj3 + 1);
	Test_assert("TAlignedMemory.alloc"
		// 在前面那个单元的前面
		, obj3 == ((byte_t*)obj2) - 8
		, "分配7个字节的单元obj3[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj3, mm->allocatedBytes);

	
	void* obj4 = TAlignedMemory_alloc(mm, 6, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj4 + 1);
	usize_t allocatedSize = sizeof(TAlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize * 2;
	Test_assert("TAlignedMemory.alloc"
		, chunk->page != 0
		// 有2个内存页
		&& chunk->page->next != 0
		&& chunk->page->next->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == allocatedSize
		, "分配6个字节的单元obj4[%p]，内存翻页\r\n分配更多的内存页[%p]\r\n共分配%ld字节\n", obj4, chunk->page, mm->allocatedBytes);
	Test_assert("TAlignedMemory.alloc"
		// 处在最后一个位置
		, obj4 == (((byte_t*)chunk->page) + sizeof(AlignedMemoryPage) + 16)
		, "新分配的单元在第二个页上(链上是首页)[%p](total:%d)，会对齐到8个字节的单元\n", obj4, mm->allocatedBytes);

	Test_assert("TAlignedMemory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)chunk->page->next) + sizeof(AlignedMemoryPage) + 16)
		, "原先的页被移动到了第二页\n");
	
	TAlignedMemory_free(mm, obj2);
	Test_assert("TAlignedMemory.free" , obj2 == chunk->page->next->free, "归还内存obj2[%p]，page上放着空闲块链\n",obj2);
	TAlignedMemory_free(mm, obj4);
	Test_assert("Memory.free"
		// 处在最后一个位置
		, obj4 == chunk->page->free
		, "归还内存obj4[%p]，page上放着空闲块链\n", obj4);
	TAlignedMemory_free(mm, obj3);
	Test_assert("TAlignedMemory.free", obj3 == chunk->page->next->free && obj2==chunk->page->next->free->next, "归还内存obj3[%p]，page上放着空闲块链", obj3);
	TAlignedMemory_free(mm, obj1);
	Test_assert("TAlignedMemory.free", obj1 == chunk->page->next->free , "归还内存obj1[%p]，page上放着空闲块链", obj1);

	void* obj11 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj11 + 1);
	Test_assert("TAlignedMemory.alloc", obj11 == obj4, "再次分配内存，使用空闲单元obj4==obj11[%p]", obj11);

	void* obj12 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj12 + 1);
	Test_assert("TAlignedMemory.alloc", obj12 == (byte_t*)obj4 - 8, "分配内存obj12，占据page2[%p]", obj12);
	void* obj13 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj13 + 1);
	Test_assert("TAlignedMemory.alloc", obj13 == (byte_t*)obj12 - 8, "分配内存obj13，占据page2[%p]", obj13);
	void* obj14 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	Test_assert("TAlignedMemory.alloc", obj14 == obj1, "分配内存obj14，重复使用pag1上的空闲块obj14==obj1[%p]", obj14);
	ref_inc((MRefUnit*)obj14 + 1);

	AlignedMemoryReleaseInfo rs = TAlignedMemory_collectGarbages(mm,1, 0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes==0, "垃圾回收，所有页面都没有空闲，无法回收页面\r\n");
	TAlignedMemory_free(mm, obj14);
	rs = TAlignedMemory_collectGarbages(mm, 1,0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes == opts.pageSize && rs.pages==1, "释放obj14,让page1页面空闲，回收一个页面\r");
	Test_assert("TAlignedMemory.collectGarbages", chunk->page && chunk->page->next==0 && chunk->page+1== obj13, "只有一个页面\r");

	bool_t sfreeRs = AlignedMemory_sfree(mm,obj13);
	Test_assert("TAlignedMemory.sfree", sfreeRs && obj13==0, "安全释放宏，由于原先的内存放着空链指针，不可以再使用，释放后原先的指针应该不能被使用\t");

	Test_end();
	
}


void testAlignedRefUnitMemory() {
	Test_begin("AlignedMemory(REF unit,32bits)", "Test start...");
	AlignedMemoryOptions opts;
	opts.pageSize = 36;
	opts.totalBytes = 0;
	opts.gcBytes = 0;
	opts.unitKind = MemoryUnitKind_ref;
	TAlignedMemory* mm = TAlignedMemory__construct__(0, &opts, TLogger_default);
	Test_assert("TAlignedMemory.__construct__", mm && mm->allocatedBytes == sizeof(TAlignedMemory), "构造对齐的内存管理器:[%p]\r\n共分配%d字节\r\n", mm, mm->allocatedBytes);

	void* obj1 = TAlignedMemory_alloc(mm, 6, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj1 + 1);
	AlignedMemoryChunk* chunk = mm->chunks[1];
	Test_assert("TAlignedMemory.alloc"
		// 在第一个插槽中
		, chunk != 0
		// 分配了一个页面
		&& chunk->page != 0
		// 只有一个内存页
		&& chunk->page->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == sizeof(TAlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize
		, "分配6个字节的单元obj1[%p]，会对齐到8个字节的单元\r\n构建一个插槽[%p]\r\n分配一个内存页[%p]\r\n共分配%ld字节\n", obj1, chunk, chunk->page, mm->allocatedBytes);

	Test_assert("TAlignedMemory.alloc"
		// 处在第一个位置
		, obj1 == (((byte_t*)&chunk->page->free))
		// 引用计数为1
		&& ((MRefUnit*)obj1)->__ref__==1
		, "分配6个字节的单元obj1[%p](total:%d)，会对齐到8个字节的单元\n", obj1, mm->allocatedBytes);

	void* obj2 = TAlignedMemory_alloc(mm, 7, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj2 + 1);
	Test_assert("TAlignedMemory.alloc"
		// 在前面那个单元的前面
		, obj2 == ((byte_t*)obj1) + 8
		, "分配7个字节的单元obj2[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj2, mm->allocatedBytes);

	void* obj3 = TAlignedMemory_alloc(mm, 8, 0, MemoryKind_normal);
	ref_increase((MRefUnit*)obj3 + 1);
	Test_assert("TAlignedMemory.alloc"
		// 在前面那个单元的前面
		, obj3 == ((byte_t*)obj2) + 8
		, "分配7个字节的单元obj3[%p](total:%d)，会对齐到8个字节的单元,挨着第一个单元\n", obj3, mm->allocatedBytes);


	void* obj4 = TAlignedMemory_alloc(mm, 6, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj4 + 1);
	usize_t allocatedSize = sizeof(TAlignedMemory) + sizeof(AlignedMemoryChunk) + opts.pageSize * 2;
	Test_assert("TAlignedMemory.alloc"
		, chunk->page != 0
		// 有2个内存页
		&& chunk->page->next != 0
		&& chunk->page->next->next == 0
		// 总共分配的内存为 memory + chunk + page
		&& mm->allocatedBytes == allocatedSize
		, "分配6个字节的单元obj4[%p]，内存翻页\r\n分配更多的内存页[%p]\r\n共分配%ld字节\n", obj4, chunk->page, mm->allocatedBytes);
	Test_assert("TAlignedMemory.alloc"
		// 处在最后一个位置
		, obj4 == (((byte_t*)&chunk->page->free))
		, "新分配的单元在第二个页上(链上是首页)[%p](total:%d)，会对齐到8个字节的单元\n", obj4, mm->allocatedBytes);

	Test_assert("TAlignedMemory.alloc"
		// 处在最后一个位置
		, obj1 == (((byte_t*)&chunk->page->next->free))
		, "原先的页被移动到了第二页\n");

	TAlignedMemory_free(mm, obj2);
	Test_assert("TAlignedMemory.free", ((MRefUnit*)obj2)->__ref__==0 , "归还内存obj2[%p]，引用计数置为0\n", obj2);
	TAlignedMemory_free(mm, obj4);
	Test_assert("TAlignedMemory.free"
		// 处在最后一个位置
		, ((MRefUnit*)obj4)->__ref__ == 0
		, "归还内存obj4[%p]，引用计数置为0\n", obj4);
	void* obj11 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj11 + 1);
	Test_assert("TAlignedMemory.alloc", obj11 == obj4, "再次分配内存obj11，使用空闲单元obj4==obj11[%p]", obj11);

	void* obj12 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj12 + 1);
	Test_assert("TAlignedMemory.alloc", obj12 == (byte_t*)obj11+8, "分配内存obj12，占用obj11后面的空间[%p]", obj12);

	void* obj13 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj13 + 1);
	Test_assert("TAlignedMemory.alloc", obj13 == (byte_t*)obj12 + 8, "分配内存obj13，占用obj12后面的空间[%p]", obj12);

	void* obj14 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj14 + 1);
	Test_assert("TAlignedMemory.alloc", obj14 == obj2, "分配内存obj13，占用obj2原先的空间[%p]", obj12);

	TAlignedMemory_free(mm, obj3);
	Test_assert("TAlignedMemory.free", ((MRefUnit*)obj3)->__ref__ == 0, "归还内存obj3[%p]，引用计数置0", obj3);
	TAlignedMemory_free(mm, obj1);
	Test_assert("TAlignedMemory.free", ((MRefUnit*)obj3)->__ref__ == 0, "归还内存obj1[%p]，引用计数置0", obj1);

	void* obj15 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj15 + 1);
	Test_assert("TAlignedMemory.alloc", obj15 == obj1, "分配内存obj15，obj15 == obj1[%p]", obj15);
	void* obj16 = TAlignedMemory_alloc(mm, 5, 0, MemoryKind_normal);
	ref_inc((MRefUnit*)obj16 + 1);
	Test_assert("TAlignedMemory.alloc", obj16 == obj3, "分配内存obj14，重复使用pag1上的空闲块obj16==obj3[%p]", obj16);
	
	
	AlignedMemoryReleaseInfo rs = TAlignedMemory_collectGarbages(mm, 1, 0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes == 0, "垃圾回收，所有页面都没有空闲，无法回收页面\r\n");
	TAlignedMemory_free(mm, obj14);
	TAlignedMemory_free(mm, obj15);
	TAlignedMemory_free(mm, obj16);
	rs = TAlignedMemory_collectGarbages(mm, 1, 0);
	Test_assert("TAlignedMemory.collectGarbages", rs.bytes == opts.pageSize && rs.pages == 1, "释放obj14,让page1页面空闲，回收一个页面\r");
	Test_assert("TAlignedMemory.collectGarbages", chunk->page && chunk->page->next == 0 && (&chunk->page->free == obj11), "只有一个页面\r");

	bool_t sfreeRs = AlignedMemory_sfree(mm, obj13);
	Test_assert("TAlignedMemory.sfree", sfreeRs && obj13 == 0, "安全释放宏，由于原先的内存放着空链指针，不可以再使用，释放后原先的指针应该不能被使用\t");

	Test_end(0, "TAlignedMemory(REF unit,32bits)", "Test done!");
	
}

struct stTUserForTestType {
	struct stMRefUnit;
	struct stTType;
};

LoggerMeta TUserForTest__meta__ = {
	.offset = 0,
	.output = TLogger__output
};


void testAlignedMemory() {
	memories_module(0, 0, 0);
	Test_begin("TAlignedMemory","");
	testAlignedLinkUnitMemory();
	testAlignedRefUnitMemory();
	Test_end();
}
#endif