/*****************************************************
*
* author:yiy
*
* description: key/value的基类
*
******************************************************/

#pragma once
#include "List.h"
#ifndef __SET_INCLUDED__ 
#define __SET_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	typedef int (*KeyComparer)(void* left, void* right);
	typedef struct stSet {
		size_t length;
		void* headOrRoot;
		void* tail;
		KeyComparer keyComparer;
	} Set;

	typedef struct stSetComparePredicate {
		void* key;
		KeyComparer keyComparer;
	}SetComparePredicate;

	inline bool_t Set_searchPredicate(void* item, size_t index, void* args) {
		return ((SetComparePredicate*)args)->keyComparer(item, ((SetComparePredicate*)args)->key);
	}

	


	Set* Set___construct__(Set* self, void* mmArgs, Memory* memory);


	inline size_t Set_length(const Set* self) { return self ? self->length : 0; }

	void* Set_put(Set* self, void* key, size_t keySize, void* value, size_t valueSize, void* mmArgs, Memory* memory);
	inline void* Set_putValueByValue(Set* self, word_t key, word_t value, void* mmArgs, Memory* memory) {
		if (!memory)memory = Memory_default();
		size_t* item = (size_t*)List_append((List*)self, sizeof(word_t) * 2, mmArgs, memory);
		*item = key;
		*(item + 1) = value;
		return item;
	}
	void* Set_putValue(Set* self, void* key, size_t keySize, word_t value, void* mmArgs, Memory* memory);
	void* Set_putByValue(Set* self, word_t key, void* value, size_t valueSize, void* mmArgs, Memory* memory);

	inline bool_t Set_get(Set* self, void* key, size_t keySize, void* value, size_t valueSize) {
		void* item = List_searchByItem((List*)self, key, keySize).item;
		if (item) {
			Memory_copy(value, ((byte_t*)item + keySize), valueSize);
			return 1;
		}
		return 0;
	}

	inline bool_t Set_getByComparer(Set* self, void* key, KeyComparer comparer,void* value, size_t valueSize) {
		SetComparePredicate cp;
		cp.key = key;
		cp.keyComparer = comparer;
		void* item = List_searchByPredicate((List*)self, Set_searchPredicate, &cp).item;
		if (item) {
			Memory_copy(value, ((byte_t*)item + sizeof(void*)), valueSize);
			return 1;
		}
		return 0;
	}


	inline word_t Set_getValueByValue(Set* self, word_t key) {
		void* item = List_searchByValue((List*)self, key).item;
		return(item) ? *((word_t*)item) : invalidWordValue();
	}
	inline word_t Set_getValueByComparer(Set* self, void* key, KeyComparer comparer) {
		SetComparePredicate cp;
		cp.key = key;
		cp.keyComparer = comparer;
		void* item = List_searchByPredicate((List*)self,Set_searchPredicate,&cp).item;
		return(item) ? *((word_t*)((char*)item + sizeof(void*))) : invalidWordValue();
	}
	word_t Set_getValue(Set* self, void* key, size_t keySize) {
		LinkSearchResult rs = List_searchByItem((List*)self, key, keySize);
		return (rs.item) ? *((word_t*)(rs.item) + 1) : invalidWordValue();
	}
	bool_t Set_getByValue(Set* self, word_t key, void* value, size_t valueSize) {
		void* item = List_searchByValue((List*)self, key).item;
		if (item) {
			Memory_copy(value,((word_t*)item) + 1,valueSize);
			return 1;
		}
		return 0;
	}
	


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __SET_INCLUDED__



