#include "Type.h"

inline const TArray* TArray___construct__(TArray* self, const void* buffer, const size_t count) {
	Type* arrType = objectInstance(self)->type;
	size_t itemSize = ((Type*)Array___INDEXGETER__(arrType->generics, 0, sizeof(void*)))->size;
	return (TArray*)Array___construct__((Array*)self, buffer, count, (const size_t)itemSize, arrType, arrType->memory);
}



const TArray* TArray_concat(const TArray* left, const TArray* right) {
	Type* leftItemType;
	Type* arrType;
	if (left) {
		arrType = objectInstance((void*)left)->type;
		leftItemType = (Type*)Array___INDEXGETER__(arrType->generics, 0, sizeof(void*));
	}
	else return right;
	Type* rightItemType;
	if (right) {
		arrType = objectInstance(left)->type;
		rightItemType = (Type*)Array___INDEXGETER__(arrType->generics, 0, sizeof(void*));
	}
	else return left;
	if (leftItemType != rightItemType) return 0;
	size_t unitSize = left ? left->length : right->length;
	return (TArray*)Array_concat((Array*)left, (Array*)right, unitSize, arrType, arrType->memory);


}

const TArray* TArray_clip(const TArray* arr, const size_t start, const size_t length) {
	Type* arrType = objectInstance(arr)->type;
	Type* itemType = ((Type*)Array___INDEXGETER__(arrType->generics, 0, sizeof(void*)));
	return (TArray*)Array_clip((Array*)arr,start,length,itemType->size,arrType,arrType->memory);
}

const void* TArray___INDEXGETER__(const TArray* self, size_t index) {
	Type* itemType = ((Type*)Array___INDEXGETER__(objectInstance(self)->type->generics, 0, sizeof(void*)));
	return (index >= self->length) ? 0 : ((char*)self + sizeof(TArray) + index * itemType->size);
}

const TString* TString___construct__(TString* self, const char_t* buffer, const size_t count) {
	Type* strType = objectInstance(self)->type;
	return (TString*)String___construct__((String*)self, buffer, count, strType,strType->memory);
}



const TString* TString_concat(const TString* left, const TString* right) {
	Type* arrType;
	if (left) {
		arrType = objectInstance((void*)left)->type;
	}
	else return right;
	if (right) {
		arrType = objectInstance(left)->type;
	}
	else return left;
	return (TString*)String_concat((const String*)left, (const String*)right, arrType, arrType->memory);


}

const TString* TString_clip(const TString* arr, const size_t start, const size_t length) {
	Type* arrType = objectInstance(arr)->type;
	return (TString*)String_clip((String*)arr, start, length, arrType, arrType->memory);
}

const char_t* TString___INDEXGETER__(const TString* self, size_t index) {
	return (char_t*)((index >= self->length) ? 0 : ((char*)self + sizeof(TString) + index * sizeof(char_t)));
}

inline TList* TList___construct__(List* self) {
	Type* listType = objectInstance(self)->type;
	return (TList*)List___construct__((List*)self, listType, listType->memory);
}

void* TList_append(TList* self) {
	const Type* itemType = (const Type*)Array___INDEXGETER__(objectInstance(self)->type->generics, 0, sizeof(void*));
	return List_append((List*)self, itemType->size, (void*)itemType, itemType->memory);
}

inline int TList_remove(TList* self, LinkPredicate predicate, void* param) {
	return List_remove((List*)self, predicate, param, objectInstance(self)->type->memory);
}

inline TArray* TList_toArray(TList* self) {
	Type* listType = objectInstance(self)->type;
	return 0;
}


