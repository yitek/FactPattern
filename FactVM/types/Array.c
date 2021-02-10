#include <string.h>
#include "Array.h"
const Array* Array___construct__(Array* self, const void* buffer, const size_t count, const size_t unitSize, void* mmArgs, Memory* memory) {
	//如果未分配，就自己申请一片内存
	if (self == 0) {
		if (!memory)memory = Memory_default();
		if (!self)self = (Array*)memory->require(memory, sizeof(Array) + count * unitSize, mmArgs);
	}
	// 写入长度
	self->length = count;
	// 拷贝原始数据到目标内存
	if (count && buffer) {
		memcpy((char*)self + sizeof(Array), buffer, unitSize * count);
	}
	return self;
}


const Array* Array_concat(const Array* left, const Array* right, const size_t unitSize, void* mmArgs, Memory* memory) {
	// 右边的没有，返回左边的
	if (right == 0 || right->length == 0) return left;
	// 左边的没有，返回右边的
	if (!left || !left->length) return right;
	size_t leftLen = left->length, rightLen = right->length;
	// 总长度等于左边长度+右边长度
	size_t count = leftLen + rightLen;
	// 内存大小等于长度乘单元大小
	size_t concatedSize = count * unitSize;

	// 构造一个新数组
	if (!memory)memory = Memory_default();
	Array* concatedArray = memory->require(memory, concatedSize, mmArgs);
	concatedArray->length = count;

	// 获取到数组元素的开始位置
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//拷贝左边的数组的元素
	memcpy(buffer, (char*)left + sizeof(Array), leftLen * unitSize);
	// 移动缓存指针到尾部
	buffer += leftLen * unitSize;
	// 拷贝右边的元素
	memcpy(buffer, (char*)right + sizeof(Array), rightLen * unitSize);
	// 返回连接后的数组
	return (const Array*)concatedArray;
}

const Array* Array_clip(const Array* arr, const size_t start, const size_t count, const size_t unitSize, void* mmArgs, Memory* memory) {
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
	// 构造子数组
	size_t size = sizeof(Array) + clipCount * unitSize;
	if (!memory)memory = Memory_default();
	Array* subArray = (Array*)memory->require(memory, size, mmArgs);
	subArray->length = clipCount;
	const void* src = ((char*)arr + sizeof(Array)) + start * unitSize;
	void* dest = (char*)subArray + sizeof(Array);
	memcpy(dest, src, clipCount * unitSize);
	return (Array*)subArray;
}

