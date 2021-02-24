#include "TString.h"
#define _ALLOC_(size,arg,kind,mm) m_alloc(size,arg,kind,mm)
#define _ALLOC1_(size,arg,kind,mm) m_alloc1(size,arg,kind,mm)
#define _FREE_(p,mm) m_free(p,mm)

const TString* const TString_empty = &String__emptyInstance.__ob__;

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

void TString__destruct__(TString* self, bool_t existed, TMemory* memory) {
	if (!existed) _FREE_(self, memory ? memory : TMemory_default);
}


const TString* TString_concat(const TString* left, const TString* right, TMemory* mm, void* type, MemoryKinds mkind) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	usize_t leftLen = left->length, rightLen = right->length;
	// 总长度等于左边长度+右边长度
	usize_t count = leftLen + rightLen;
	if (count == 0) return TString_empty;
	// 内存大小等于长度乘单元大小

	// 构造一个新数组
	if (!mm)mm = TMemory_default;
	TString* concatedArray = _ALLOC_(sizeof(TString) + left->bytes + right->bytes + sizeof(unichar_t), type, mkind, mm);
	concatedArray->length = count;
	concatedArray->bytes = left->bytes + right->bytes;
	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(TString);
	//拷贝左边的数组的元素
	TMemory_copy(buffer, (char*)left + sizeof(TString), left->bytes);
	// 移动缓存指针到尾部
	buffer += left->bytes;
	// 拷贝右边的元素
	TMemory_copy(buffer, (char*)right + sizeof(TString), right->bytes);
	buffer += right->bytes;
	*(unichar_t*)buffer = 0;
	// 返回连接后的数组
	return (const TString*)concatedArray;
}

const TString* TString_clip(const TString* arr, const usize_t start, usize_t length, TMemory* memory, void* type, MemoryKinds mkind) {
	if (arr && arr->length) {
		if (length == -1) length = arr->length - start;
		else if (start >= arr->length) return TString_empty;
		else if (start + length > arr->length) length = arr->length - start;
		const utf8_t* startp = UTF8_skip((const utf8_t*)(arr + 1), start);
		const utf8_t* endp = UTF8_skip(startp, length);
		usize_t bytes = endp - startp;
		if (!memory) memory = TMemory_default;
		TString* substr = m_alloc(sizeof(TString) + bytes + sizeof(unichar_t), type, mkind, memory);
		m_copy((substr + 1), startp, bytes);
		*(unichar_t*)((byte_t*)(substr + 1) + bytes) = 0;
		substr->length = length;
		substr->bytes = bytes;
		return substr;
	} return String_empty;

}

#undef _ALLOC_
#undef _ALLOC1_
#undef _FREE_