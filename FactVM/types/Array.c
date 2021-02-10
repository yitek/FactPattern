#include <string.h>
#include "Array.h"
const Array* Array___construct__(Array* self, const void* buffer, const size_t count, const size_t unitSize, void* mmArgs, Memory* memory) {
	//���δ���䣬���Լ�����һƬ�ڴ�
	if (self == 0) {
		if (!memory)memory = Memory_default();
		if (!self)self = (Array*)memory->require(memory, sizeof(Array) + count * unitSize, mmArgs);
	}
	// д�볤��
	self->length = count;
	// ����ԭʼ���ݵ�Ŀ���ڴ�
	if (count && buffer) {
		memcpy((char*)self + sizeof(Array), buffer, unitSize * count);
	}
	return self;
}


const Array* Array_concat(const Array* left, const Array* right, const size_t unitSize, void* mmArgs, Memory* memory) {
	// �ұߵ�û�У�������ߵ�
	if (right == 0 || right->length == 0) return left;
	// ��ߵ�û�У������ұߵ�
	if (!left || !left->length) return right;
	size_t leftLen = left->length, rightLen = right->length;
	// �ܳ��ȵ�����߳���+�ұ߳���
	size_t count = leftLen + rightLen;
	// �ڴ��С���ڳ��ȳ˵�Ԫ��С
	size_t concatedSize = count * unitSize;

	// ����һ��������
	if (!memory)memory = Memory_default();
	Array* concatedArray = memory->require(memory, concatedSize, mmArgs);
	concatedArray->length = count;

	// ��ȡ������Ԫ�صĿ�ʼλ��
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//������ߵ������Ԫ��
	memcpy(buffer, (char*)left + sizeof(Array), leftLen * unitSize);
	// �ƶ�����ָ�뵽β��
	buffer += leftLen * unitSize;
	// �����ұߵ�Ԫ��
	memcpy(buffer, (char*)right + sizeof(Array), rightLen * unitSize);
	// �������Ӻ������
	return (const Array*)concatedArray;
}

const Array* Array_clip(const Array* arr, const size_t start, const size_t count, const size_t unitSize, void* mmArgs, Memory* memory) {
	size_t len = arr->length;
	//��ʼλ�ñ����һ��λ�û�����,ʲô���޷���ȡ
	if (start >= len) return 0;
	// ���û��ָ����ȡ������Ĭ��Ϊ��ȡʣ�ಿ��
	size_t clipCount;
	if (count == -1) clipCount = len - start;
	else {
		// ��ȡ�ĳ��ȳ��������Χ����ȡ��β��
		if (start + count >= len) clipCount = len - start;
		else clipCount = count;
	}
	// ����������
	size_t size = sizeof(Array) + clipCount * unitSize;
	if (!memory)memory = Memory_default();
	Array* subArray = (Array*)memory->require(memory, size, mmArgs);
	subArray->length = clipCount;
	const void* src = ((char*)arr + sizeof(Array)) + start * unitSize;
	void* dest = (char*)subArray + sizeof(Array);
	memcpy(dest, src, clipCount * unitSize);
	return (Array*)subArray;
}

