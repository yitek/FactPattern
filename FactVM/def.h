/*****************************************************
* 
* author:yiy
* 
* description: ���ö����ļ�
* ����һЩ������������
* 32λ��64λ�����ܹ�����ͬ��Դ����
* 
******************************************************/

#pragma once

// ��Ԥ������ǩ������ͷ�ļ��Ľ�������
// ʵ�ֶ�����ø�ͷ�ļ�ʱ��ͷ�ļ��������ھ���Ԥ��������ͱ�������Դ������ֻ����һ��
// ��ṹΪ #ifndef __Some_Mark
//          #define __some_Mark
//         #endif
#ifndef __DEF_INCLUDED__ 
#define __DEF_INCLUDED__


typedef char* addr_t;
typedef char byte_t;
typedef unsigned short char_t;
typedef unsigned int bool_t;


#ifdef _WIN64
typedef long word_t;
typedef unsigned long size_t;
typedef long pointer_t;
#else // else _WIN64
typedef int word_t;
typedef unsigned int size_t;
typedef int pointer_t;
#endif // endif _WIN64

#endif // end ifndef __DEF_INCLUDED__