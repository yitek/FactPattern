/*****************************************************
*
* author:yiy
*
* description: 集合类

*
******************************************************/

#pragma once
#include "TObject.h"
#include "../ads/Set.h"
#ifndef __TSET_INCLUDED__ 
#define __TSET_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif





	inline Set* TSet___construct__(Set* self) {
		return Set___construct__(self,0,0);
	}


	inline size_t TSet_length(const Set* self) { return self ? self->length : 0; }

	inline void* TSet_put(Set* self, void* key, void* value) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self,0);
		Type* keyType = TType_getGenericType(nodeType, 0);
		Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_put(self, key, keyType->size, value, valueType->size, nodeType, nodeType->memory);
	}
	inline void* TSet_putValueByValue(Set* self, word_t key, word_t value) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		return Set_putValueByValue(self,key,value, nodeType,nodeType->memory);
	}
	inline void* TSet_putValue(Set* self, void* key,  word_t value) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		Type* keyType = TType_getGenericType(nodeType, 0);
		return Set_putValue(self,key,keyType->size,value,nodeType,nodeType->memory);
	}
	inline void* TSet_putByValue(Set* self, word_t key, void* value, size_t valueSize) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_putByValue(self,key,value,valueType->size,nodeType,nodeType->memory);
	}

	inline bool_t TSet_get(Set* self, void* key,  void* value) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		Type* keyType = TType_getGenericType(nodeType, 0);
		Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_get(self,key,keyType->size,value,valueType->size);
	}

	inline bool_t TSet_getByComparer(Set* self, void* key, KeyComparer comparer, void* value) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		//Type* keyType = TType_getGenericType(nodeType, 0);
		Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_getByComparer(self, key, comparer, value, valueType->size);
	}


	
	inline word_t TSet_getValue(Set* self, void* key) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		Type* keyType = TType_getGenericType(nodeType, 0);
		//Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_getValue(self, key,keyType->size);
	}
	inline bool_t TSet_getByValue(Set* self, word_t key, void* value, size_t valueSize) {
		if (!self) return 0;
		Type* nodeType = TObject_getIndexType(self, 0);
		//Type* keyType = TType_getGenericType(nodeType, 0);
		Type* valueType = TType_getGenericType(nodeType, 1);
		return Set_getByValue(self, key, value,valueType->size);
	}

#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __TSET_INCLUDED__



