#include "String.h"


struct {
	struct stString;
	unichar_t endl;
} String_emptyInstance = {
	.bytes=0,.length=0,.endl=0
};
const String*const String_empty =(const String*const)&String_emptyInstance;
const String* String__construct__(String* self, const byte_t* buffer, usize_t byteCount, TMemory* memory, void* type, MemoryKinds mkind) {
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
		self = m_alloc( sizeof(String) + byteCount + sizeof(unichar_t), type,mkind, memory);
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

const String* String_concat(const String* left,const String* right, TMemory* mm, void* type, MemoryKinds mkind) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	usize_t leftLen = left->length, rightLen = right->length;
	// 总长度等于左边长度+右边长度
	usize_t count = leftLen + rightLen;
	if (count == 0 ) return String_empty;
	// 内存大小等于长度乘单元大小

	// 构造一个新数组
	if (!mm)mm = TMemory_default;
	String* concatedArray = m_alloc(sizeof(String) + left->bytes + right->bytes + sizeof(unichar_t), type, mkind,mm);
	concatedArray->length = count;
	concatedArray->bytes = left->bytes + right->bytes;
	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(String);
	//拷贝左边的数组的元素
	TMemory_copy(buffer, (char*)left + sizeof(String), left->bytes);
	// 移动缓存指针到尾部
	buffer += left->bytes;
	// 拷贝右边的元素
	TMemory_copy(buffer, (char*)right + sizeof(String), right->bytes);
	buffer += right->bytes;
	*(unichar_t*)buffer = 0;
	// 返回连接后的数组
	return (const String*)concatedArray;
}

const String* String_clip(const String* arr, const usize_t start, usize_t length, TMemory* memory, void* type, MemoryKinds mkind) {
	if (arr && arr->length) {
		if (length == -1) length = arr->length - start;
		else if (start >= arr->length) return String_empty;
		else if (start + length > arr->length) length = arr->length - start;
		const utf8_t* startp = UTF8_skip((const utf8_t*)(arr + 1), start);
		const utf8_t* endp = UTF8_skip(startp, length);
		usize_t bytes = endp - startp;
		if (!memory) memory = TMemory_default;
		String* substr = m_alloc(sizeof(String) + bytes + sizeof(unichar_t), type, mkind, memory);
		m_copy((substr + 1), startp, bytes);
		*(unichar_t*)((byte_t*)(substr + 1) + bytes) = 0;
		substr->length = length;
		substr->bytes = bytes;
		m_look(substr,8,4);
		return substr;
	} return String_empty;

}