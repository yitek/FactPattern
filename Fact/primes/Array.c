#include "Array.h"
const size_t Array_EndlMask = 1 << (sizeof(size_t) * 8 - 1);

const Array* Array_construct(Array* self, const void* buffer, const size_t count, size_t unitSize, void* mmArgs, Memory* memory) {
	//如果未分配，就自己申请一片内存
	bool_t hasEndl = unitSize & Array_EndlMask;
	if (self == 0) {
		if (!memory)memory = Memory_default();
		if (!self) {
			if(hasEndl)self = (Array*)memory->require(memory, sizeof(Array) + (count+1) * unitSize, mmArgs);
			else self = (Array*)memory->require(memory, sizeof(Array) + count * unitSize, mmArgs);
		}
	}
	// 写入长度
	self->length = count;
	if (hasEndl) {
		unitSize = unitSize - Array_EndlMask;
		char* p = (char*)self + sizeof(Array);
		if (buffer) Memory_copy(p, buffer, unitSize * count);
		p += unitSize * count;
		if (sizeof(word_t) == unitSize) *((word_t*)p) = 0;
		else if (sizeof(char_t) == unitSize) *((char_t*)p) = 0;
		else Memory_clear(p, unitSize);
	}
	else {
		// 拷贝原始数据到目标内存
		if (count && buffer) {
			Memory_copy((char*)self + sizeof(Array), buffer, unitSize * count);
		}
	}
	
	return self;
}



size_t Array_index(const Array* self, void* item, size_t unitSize,size_t start) {
	if (!self || self->length == 0 || self->length<=start) return -1;
	
	if (unitSize == sizeof(word_t)) {
		word_t k = *((word_t*)item);
		word_t* buffer = (word_t*)((byte_t*)self + sizeof(Array)) + start ;
		for (size_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}else if (unitSize == sizeof(char_t)) {
		char_t k = *((char_t*)item);
		char_t* buffer = (char_t*)((byte_t*)self + sizeof(Array)) + start;
		for (size_t i = start; i < self->length; i++) if (*buffer++ == k) return i;
	}
	else {
		byte_t* buffer = (byte_t*)((byte_t*)self + sizeof(Array)) + start* unitSize;
		for (size_t i = start; i < self->length; i++) {
			if (Memory_equal(buffer, item, unitSize)) return i;
			buffer += unitSize;
		}
	}
	return -1;
}


const Array* Array_concat(const Array* left, const Array* right, const size_t unitSize,const Array* empty,void* mmArgs, Memory* memory) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	size_t leftLen = left->length, rightLen = right->length;
	// 总长度等于左边长度+右边长度
	size_t count = leftLen + rightLen ;
	if (count == 0 && empty) return empty;
	bool_t hasEndl = unitSize & Array_EndlMask;
	size_t concatedSize;
	if (hasEndl) {
		concatedSize = (count + 1) * (unitSize - Array_EndlMask) + sizeof(Array);
	}else concatedSize = count  * unitSize + sizeof(Array);
	// 内存大小等于长度乘单元大小

	// 构造一个新数组
	if (!memory)memory = Memory_default();
	Array* concatedArray = memory->require(memory, concatedSize, mmArgs);
	concatedArray->length = count;

	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//拷贝左边的数组的元素
	Memory_copy(buffer, (char*)left + sizeof(Array), leftLen * unitSize);
	// 移动缓存指针到尾部
	buffer += leftLen * unitSize;
	// 拷贝右边的元素
	Memory_copy(buffer, (char*)right + sizeof(Array), rightLen * unitSize);
	if (hasEndl) {
		buffer += rightLen * unitSize;
		if (unitSize == sizeof(char_t)) {
			*((char_t*)buffer) = (char_t)0;
		}
		else {
			Memory_clear(buffer, unitSize);
		}
	}
	// 返回连接后的数组
	return (const Array*)concatedArray;
}

const Array* Array_clip(const Array* arr, const size_t start, const size_t count, const size_t unitSize,const Array* empty, void* mmArgs, Memory* memory) {
	if (!arr)return 0;
	size_t len = arr->length;
	//开始位置比最后一个位置还靠后,什么都无法截取
	if (start >= len) return 0;
	// 如果没有指定截取个数，默认为截取剩余部分
	size_t clipCount;
	if (count == -1) clipCount = len - start;
	else {
		// 截取的长度超出了最大范围，截取到尾部
		if (start + count >= len) clipCount = len - start;
		else clipCount = count;
	}
	if (clipCount == 0 && empty) return empty;
	// 构造子数组
	bool_t hasEndl = unitSize & Array_EndlMask;
	size_t size;
	if (hasEndl) {
		size = (clipCount + 1) * (unitSize - Array_EndlMask);
	}
	else size = sizeof(Array) + clipCount * unitSize;
	if (!memory)memory = Memory_default();
	Array* subArray = (Array*)memory->require(memory, size, mmArgs);
	subArray->length = clipCount;
	const void* src = ((char*)arr + sizeof(Array)) + start * unitSize;
	byte_t* dest = (char*)subArray + sizeof(Array);
	Memory_copy(dest, src, clipCount * unitSize);
	if (hasEndl) {
		dest += clipCount * unitSize;
		if (unitSize == sizeof(char_t)) {
			*((char_t*)dest) = (char_t)0;
		}
		else {
			Memory_clear(dest, unitSize);
		}
	}
	return (Array*)subArray;
}

void Array_destruct(Array* self, bool_t existed) {
	if (!existed) {
		Memory* mm = Memory_default();
		mm->release(mm,self);
	}
}
