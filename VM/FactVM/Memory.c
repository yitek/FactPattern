#include "Memory.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
Memory* memory_construct(Memory* self,HOST_ADDR pageSize,unsigned int pageCount, unsigned int maxPageCount) {
	if(!self) self = (Memory*)malloc(sizeof(Memory));
	if (!self) {
		printf_s("无法分配内存");
		exit(0);
	} 
	self->pageSize = pageSize;
	self->pageCount = pageCount;
	self->maxPageCount = maxPageCount;
	list_construct(&self->sections, sizeof(struct TMemorySection),0,0);
	return self;
}
void memory_destruct(Memory* self) {
	
}

int lookupPage(struct TMemoryPage* page, HOST_ADDR vAddress) {
	if (page->vAddressBegin <= vAddress && page->vAddressEnd >= vAddress)return 1;
	return 0;
}
HOST_ADDR memory_getPhysicalAddress(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	struct TMemorySection* section = list_find(&self->sections, (void*)scp, 0);
	if (!section) {
		section = list_add(&self->sections);
		section->sectionDescriptor = scp;
		list_construct(&section->pages, (unsigned int)self->pageSize, 0, 0);
	}

	struct TMemoryPage* page = list_find(&section->pages, lookupPage, vAddress);
	if (!page) {
		// 触发中断，但这是虚拟机，直接请求内存
		page = list_add(&section->pages);
		// 计算开始与结束地址
		HOST_ADDR startAddr = (HOST_ADDR)(((size_t)vAddress / (size_t)self->pageSize) * (size_t)self->pageSize);
		HOST_ADDR endAddr = (HOST_ADDR)(startAddr + (size_t)self->pageSize);
		page->vAddressBegin = startAddr;
		page->vAddressEnd = endAddr;
	}
	size_t offset = (size_t)vAddress - (size_t)page->vAddressBegin;
	return (HOST_ADDR)((char*)page + sizeof(struct TMemoryPage) + offset);
	
}
HOST_DWORD memory_read(Memory* self, HOST_ADDR vAddress, int size, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self,vAddress,scp);
	if (size == 1) return *((char*)(fAddress));
	if (size == 2) return *((short*)(fAddress));
	if (size == 4) return *((int*)(fAddress));
	if (size == 8) return *((long*)(fAddress));
	if (size == 16) return *((long long*)(fAddress));
	printf_s("memory_read的参数size不正确，只能是1，2，4，8，16");
	return 0;
}

HOST_DWORD memory_write(Memory* self, HOST_ADDR vAddress, int size, HOST_DWORD value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	if (size == 1) *((char*)(fAddress))=(char)value;
	if (size == 2) *((short*)(fAddress)) = (short)value;
	if (size == 4) *((int*)(fAddress)) = (int)value;
	if (size == 8) *((long*)(fAddress)) = (long)value;
	if (size == 16) *((long long*)(fAddress)) = (long long)value;
	printf_s("memory_read的参数size不正确，只能是1，2，4，8，16");
	return 0;
}