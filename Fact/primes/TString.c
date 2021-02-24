#include "TString.h"
#define _ALLOC_(size,arg,kind,mm) m_alloc(size,arg,kind,mm)
#define _ALLOC1_(size,arg,kind,mm) m_alloc1(size,arg,kind,mm)
#define _FREE_(p,mm) m_free(p,mm)



const TString* TString__construct__(TString* self, const byte_t* buffer, usize_t byteCount, TMemory* memory, MemoryKinds mkind) {
	usize_t charCount = -1;

	if (byteCount == 0) {
		if (buffer) {
			EncodingCountResult c = UTF8_count((const byte_t* const)buffer);
			byteCount = c.byteCount;
			charCount = c.charCount;
		}
	}
	if (!self) {
		if (!memory) memory = TMemory_default;
		self = m_alloc(sizeof(String) + byteCount + sizeof(unichar_t), 0, mkind, memory);
	}
	if (buffer && byteCount) {
		m_copy(self + 1, buffer, byteCount + sizeof(unichar_t));

	}
	else {
		m_clear(self + 1, byteCount);
	}
	self->bytes = byteCount;
	utf8_t* p = (((utf8_t*)(self + 1)) + byteCount);
	*(unichar_t*)p = 0;
	if (charCount != -1) self->length = charCount;
	return self;
}

#undef _ALLOC_
#undef _ALLOC1_
#undef _FREE_