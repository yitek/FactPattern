/*****************************************************
*
* author:yiy
*
* description: 集合类 Set<TValue,TKey>(带类型)
* 只有集合类才可以使用foreach来遍历
* 其带着2个泛型参数,TValue默认为Any,TKey默认为int
* TSet是一个纯虚接口，带着遍历所需要的3个函数
* __reset__,__current__,__next__
* let list = List()
* print(set:ISet=0) any:
*	set __reset__,then moveNext
*	return set.__current__
* print list
* 
* 
******************************************************/

#pragma once
#include "../runtime.h"
#ifndef __ISET_INCLUDED__ 
#define __ISET_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif
	
	typedef struct stISet ISet;
	
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __ISET_INCLUDED__



