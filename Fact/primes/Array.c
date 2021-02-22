#include "Array.h"

#define ALLOC(mm,size,arg,kind) m_alloc(mm,size,arg,kind)
#define FREE(mm,p) m_free(mm,p)


const Array* Array__construct__(Array* self, const void* buffer, const usize_t count, usize_t unitSize, TMemory* mm, void* mInitArgs, MemoryKinds mkind) {
	if (unitSize == 0) log_exit(ExitCode_argument, "Array.__construct__", "unitSize is required.");
	//如果未分配，就自己申请一片内存
	if (self == 0) {
		if (!mm)mm = TMemory_default;
		if (!self) {
			self = ALLOC(mm, sizeof(Array) + count * unitSize,mInitArgs,mkind);
		}
	}
	// 写入长度
	self->length = count;
	if(count && buffer)m_copy(self + 1, buffer, count * unitSize);
	
	return self;
}



usize_t Array_index(const Array* self, void* item, usize_t unitSize, usize_t start) {
	if (!self || self->length == 0 || self->length<=start) return -1;
	
	if (unitSize == sizeof(word_t)) {
		word_t k = *((word_t*)item);
		word_t* buffer = (word_t*)((byte_t*)self + sizeof(Array)) + start ;
		for (usize_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}else if (unitSize == sizeof(byte_t)) {
		byte_t k = *((byte_t*)item);
		byte_t* buffer = (byte_t*)((byte_t*)self + sizeof(Array)) + start;
		for (usize_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}
	else {
		byte_t* buffer = (byte_t*)((byte_t*)self + sizeof(Array)) + start* unitSize;
		for (usize_t i = start; i < self->length; i++) {
			if (TMemory_equal(buffer, item, unitSize)) return i;
			buffer += unitSize;
		}
	}
	return -1;
}


const Array* Array_concat(const Array* left, const Array* right, const usize_t unitSize,const Array* empty, TMemory* mm, void* mInitArgs, MemoryKinds mkind) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	usize_t leftLen = left->length, rightLen = right->length;
	// 总长度等于左边长度+右边长度
	usize_t count = leftLen + rightLen ;
	if (count == 0 && empty) return empty;
	usize_t concatedSize = count  * unitSize + sizeof(Array);
	// 内存大小等于长度乘单元大小

	// 构造一个新数组
	if (!mm)mm = TMemory_default;
	Array* concatedArray = ALLOC(mm, concatedSize, mInitArgs,mkind);
	concatedArray->length = count;

	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//拷贝左边的数组的元素
	TMemory_copy(buffer, (char*)left + sizeof(Array), leftLen * unitSize);
	// 移动缓存指针到尾部
	buffer += leftLen * unitSize;
	// 拷贝右边的元素
	TMemory_copy(buffer, (char*)right + sizeof(Array), rightLen * unitSize);
	// 返回连接后的数组
	return (const Array*)concatedArray;
}

const Array* Array_clip(const Array* arr, const usize_t start, const usize_t count, const usize_t unitSize,const Array* empty, TMemory* mm, void* mInitArgs, MemoryKinds mkind) {
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
	if (clipCount == 0 && empty) return empty;
	// 构造子数组
	
	usize_t size = sizeof(Array) + clipCount * unitSize;
	if (!mm)mm = TMemory_default;
	Array* subArray = (Array*)ALLOC(mm, size, mInitArgs,mkind);
	subArray->length = clipCount;
	const void* src = ((char*)arr + sizeof(Array)) + start * unitSize;
	byte_t* dest = (char*)subArray + sizeof(Array);
	TMemory_copy(dest, src, clipCount * unitSize);
	
	return (Array*)subArray;
}

void Array__destruct__(Array* self, bool_t existed) {
	if (!existed) {
		FREE(TMemory_default,self);
	}
}

#undef ALLOC
#undef FREE
