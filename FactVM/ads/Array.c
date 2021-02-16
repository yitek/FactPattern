#include "Array.h"
const size_t Array_EndlMask = 1 << (sizeof(size_t) * 8 - 1);

const Array* Array_construct(Array* self, const void* buffer, const size_t count, size_t unitSize, void* mmArgs, Memory* memory) {
	//���δ���䣬���Լ�����һƬ�ڴ�
	bool_t hasEndl = unitSize & Array_EndlMask;
	if (self == 0) {
		if (!memory)memory = Memory_default();
		if (!self) {
			if(hasEndl)self = (Array*)memory->require(memory, sizeof(Array) + (count+1) * unitSize, mmArgs);
			else self = (Array*)memory->require(memory, sizeof(Array) + count * unitSize, mmArgs);
		}
	}
	// д�볤��
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
		// ����ԭʼ���ݵ�Ŀ���ڴ�
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
	// �ұߵ�û�У�������ߵ�
	if (right == 0 || right->length == 0) return left;
	// ��ߵ�û�У������ұߵ�
	if (!left || !left->length) return right;
	size_t leftLen = left->length, rightLen = right->length;
	// �ܳ��ȵ�����߳���+�ұ߳���
	size_t count = leftLen + rightLen ;
	if (count == 0 && empty) return empty;
	bool_t hasEndl = unitSize & Array_EndlMask;
	size_t concatedSize;
	if (hasEndl) {
		concatedSize = (count + 1) * (unitSize - Array_EndlMask) + sizeof(Array);
	}else concatedSize = count  * unitSize + sizeof(Array);
	// �ڴ��С���ڳ��ȳ˵�Ԫ��С

	// ����һ��������
	if (!memory)memory = Memory_default();
	Array* concatedArray = memory->require(memory, concatedSize, mmArgs);
	concatedArray->length = count;

	// ��ȡ������Ԫ�صĿ�ʼλ��
	char* buffer = ((char*)concatedArray) + sizeof(Array);
	//������ߵ������Ԫ��
	Memory_copy(buffer, (char*)left + sizeof(Array), leftLen * unitSize);
	// �ƶ�����ָ�뵽β��
	buffer += leftLen * unitSize;
	// �����ұߵ�Ԫ��
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
	// �������Ӻ������
	return (const Array*)concatedArray;
}

const Array* Array_clip(const Array* arr, const size_t start, const size_t count, const size_t unitSize,const Array* empty, void* mmArgs, Memory* memory) {
	if (!arr)return 0;
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
	if (clipCount == 0 && empty) return empty;
	// ����������
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
