#include "Memory.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>


int lookupPage(struct TMemoryPage* page, HOST_ADDR vAddress) {
	if (page->vAddressBegin <= vAddress && page->vAddressEnd >= vAddress)return 1;
	return 0;
}
HOST_ADDR Memory_getPhysicalAddress(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	struct TMemorySection* section = List_find(&self->sections, (void*)scp, 0);
	if (!section) {
		section = List_add(&self->sections);
		section->sectionDescriptor = scp;
		List_construct(&section->pages, (unsigned int)self->pageSize, 0, 0);
	}

	struct TMemoryPage* page = List_find(&section->pages, lookupPage, vAddress);
	if (!page) {
		// 触发中断，但这是虚拟机，直接请求内存
		page = List_add(&section->pages);
		// 计算开始与结束地址
		HOST_ADDR startAddr = (HOST_ADDR)(((size_t)vAddress / (size_t)self->pageSize) * (size_t)self->pageSize);
		HOST_ADDR endAddr = (HOST_ADDR)(startAddr + (size_t)self->pageSize);
		page->vAddressBegin = startAddr;
		page->vAddressEnd = endAddr;
	}
	size_t offset = (size_t)vAddress - (size_t)page->vAddressBegin;
	return (HOST_ADDR)((char*)page + sizeof(struct TMemoryPage) + offset);

}
HOST_DWORD Memory_read(Memory* self, HOST_ADDR vAddress, int size, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	if (size == 2) return *((short*)(fAddress));
	if (size == 4) return *((int*)(fAddress));
	if (size == 8) return *((long*)(fAddress));
	if (size == 16) return *((long long*)(fAddress));
	printf_s("memory_read的参数size不正确，只能是2，4，8，16");
	return 0;
}



HOST_DWORD Memory_write(Memory* self, HOST_ADDR vAddress, int size, HOST_DWORD value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	if (size == 1) *((char*)(fAddress)) = (char)value;
	if (size == 2) *((short*)(fAddress)) = (short)value;
	if (size == 4) *((int*)(fAddress)) = (int)value;
	if (size == 8) *((long*)(fAddress)) = (long)value;
	if (size == 16) *((long long*)(fAddress)) = (long long)value;
	printf_s("memory_read的参数size不正确，只能是1，2，4，8，16");
	return 0;
}

BITS08 Memory_read01(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	return ((BITS08*)memory_getPhysicalAddress(self, vAddress, scp));

	return 0;
}

void Memory_write01(Memory* self, HOST_ADDR vAddress, BITS08 value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	return *((BITS08*)fAddress) = value;
}


BITS16 Memory_read02(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	return ((BITS16*)memory_getPhysicalAddress(self, vAddress, scp));

	return 0;
}

void Memory_write02(Memory* self, HOST_ADDR vAddress, BITS16 value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	return *((BITS16*)fAddress) = value;
}

BITS32 Memory_read04(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	return ((BITS32*)memory_getPhysicalAddress(self, vAddress, scp));

	return 0;
}

void Memory_write04(Memory* self, HOST_ADDR vAddress, BITS32 value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	return *((BITS32*)fAddress) = value;
}


BITS64 Memory_read08(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	return ((BITS64*)memory_getPhysicalAddress(self, vAddress, scp));

	return 0;
}

void Memory_write08(Memory* self, HOST_ADDR vAddress, BITS64 value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	return *((BITS64*)fAddress) = value;
}

BITS128 Memory_read16(Memory* self, HOST_ADDR vAddress, HOST_ADDR scp) {
	return ((BITS128*)memory_getPhysicalAddress(self, vAddress, scp));

	return 0;
}

void Memory_write16(Memory* self, HOST_ADDR vAddress, BITS128 value, HOST_ADDR scp) {
	HOST_ADDR fAddress = memory_getPhysicalAddress(self, vAddress, scp);
	return *((BITS32*)fAddress) = value;
}

Memory* Memory_construct(Memory* self,HOST_ADDR pageSize,unsigned int maxPageCount) {
	if(!self) self = (Memory*)malloc(sizeof(Memory));
	if (!self) {
		printf_s("无法分配内存");
		exit(0);
	} 
	self->pageSize = pageSize;
	self->maxPageCount = maxPageCount;
	List_construct(&self->sections, sizeof(struct TMemorySection),0,0);
	self->readByte = Memory_read01;
	self->writeByte = Memory_write01;
	if (sizeof(short) == 2) {
		self->readByte2 = Memory_read02;
		self->writeByte2 = Memory_write02;
	}
	
	return self;
}
void Memory_destruct(Memory* self) {
	struct TListNode* sectionNode = self->sections.head;
	while (sectionNode) {
		
		struct TMemorySection* section = &sectionNode->value;
		List_destruct(&section->pages);
	}
	List_destruct(&self->sections);
}



