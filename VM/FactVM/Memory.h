#pragma once
#include "FactVM.h"
#include "List.h"
typedef struct TMemory {
	struct TMMU;
	HOST_ADDR pageSize;
	unsigned int pageCount;
	unsigned int maxPageCount;
	List sections;
} Memory;

struct TMemorySection {
	HOST_ADDR sectionDescriptor;
	List pages;
};
struct TMemoryPage {
	HOST_ADDR vAddressBegin;
	HOST_ADDR vAddressEnd;
};
Memory* Memory_construct(Memory* self,HOST_ADDR pageSize,unsigned int maxPageCount);
HOST_DWORD Memory_read(Memory* self , HOST_ADDR vAddress,int size, HOST_ADDR scp);
HOST_DWORD Memory_write(Memory* self, HOST_ADDR vAddress, int size,HOST_DWORD value, HOST_ADDR scp);
