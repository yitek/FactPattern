#include "TArray.h"

#define _ALLOC_(size,type,mind,mm) m_alloc(size,type,mind,mm);
#define _FREE_(p,mm) m_free(p,mm);

const TArray* TArray__construct__(TArray* self, const void* buffer, const usize_t count,  TMemory* mm, struct stTType* type, MemoryKinds mkind) {
	//if (unitSize == 0) log_exit(ExitCode_argument, "Array.__construct__", "unitSize is required.");
	//如果未分配，就自己申请一片内存
	
	if (self == 0) {
		if (!mm)mm = TMemory_default;
		if (!self) {
			self = _ALLOC_(sizeof(TArray) + count * type->size, type, mkind, mm);
		}
	}
	// 写入长度
	self->length = count;
	if (count && buffer)m_copy(self + 1, buffer, count * type->size);

	return self;
}



usize_t TArray_index(const TArray* self, void* item, usize_t unitSize, usize_t start) {
	if (!self || self->length == 0 || self->length <= start) return -1;

	if (unitSize == sizeof(word_t)) {
		word_t k = *((word_t*)item);
		word_t* buffer = (word_t*)((byte_t*)self + sizeof(TArray)) + start;
		for (usize_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}
	else if (unitSize == sizeof(byte_t)) {
		byte_t k = *((byte_t*)item);
		byte_t* buffer = (byte_t*)((byte_t*)self + sizeof(TArray)) + start;
		for (usize_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}
	else {
		byte_t* buffer = (byte_t*)((byte_t*)self + sizeof(TArray)) + start * unitSize;
		for (usize_t i = start; i < self->length; i++) {
			if (TMemory_equal(buffer, item, unitSize)) return i;
			buffer += unitSize;
		}
	}
	return -1;
}


const TArray* TArray_concat(const TArray* left, const TArray* right, TMemory* mm,MemoryKinds mkind) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	usize_t leftLen = left->length, rightLen = right->length;

	//这里应该有类型检查代码
	struct stTType* arrType = get_type((struct stTObject*)left);
	usize_t unitSize = get_genericArgument((struct stTObject*)left, 0)->size;
	// 总长度等于左边长度+右边长度
	usize_t count = leftLen + rightLen;
	//if (count == 0 ) return empty;
	usize_t concatedSize = count * unitSize + sizeof(TArray);
	// 内存大小等于长度乘单元大小

	// 构造一个新数组
	TArray* concatedArray = _ALLOC_(concatedSize, arrType, mkind, mm);
	concatedArray->length = count;

	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(TArray);
	//拷贝左边的数组的元素
	TMemory_copy(buffer, (char*)left + sizeof(TArray), leftLen * unitSize);
	// 移动缓存指针到尾部
	buffer += leftLen * unitSize;
	// 拷贝右边的元素
	TMemory_copy(buffer, (char*)right + sizeof(TArray), rightLen * unitSize);
	// 返回连接后的数组
	return (const TArray*)concatedArray;
}

const TArray* TArray_clip(const TArray* arr, const usize_t start, const usize_t count,  TMemory* mm,  MemoryKinds mkind) {
	if (!arr)return 0;
	usize_t len = arr->length;
	//开始位置比最后一个位置还靠后,什么都无法截取
	if (start >= len) return 0;
	// 如果没有指定截取个数，默认为截取剩余部分
	usize_t clipCount;
	if (count == -1) clipCount = len - start;
	else {
		// 截取的长度超出了最大范围，截取到尾部
		if (start + count >= len) clipCount = len - start;
		else clipCount = count;
	}
	if (clipCount == 0 ) return 0;
	// 构造子数组
	struct stTType* arrType = get_type((struct stTObject*)arr);
	usize_t unitSize = get_genericArgument((struct stTObject*)arr, 0)->size;
	usize_t size = sizeof(TArray) + clipCount * unitSize;
	if (!mm)mm = TMemory_default;
	TArray* subArray = (TArray*)_ALLOC_(size, arrType, mkind, mm);
	subArray->length = clipCount;
	const void* src = ((char*)arr + sizeof(TArray)) + start * unitSize;
	byte_t* dest = (char*)subArray + sizeof(TArray);
	TMemory_copy(dest, src, clipCount * unitSize);

	return (TArray*)subArray;
}

void TArray__destruct__(TArray* self, bool_t existed, TMemory* mm) {
	if (!existed) {
		_FREE_(self, mm ? mm : TMemory_default);
	}
}

#undef _ALLOC_
#undef _FREE_
