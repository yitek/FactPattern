// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MAllocator.h"
#include "List.h"
#include "String.h"

void ListTest();
void StringTest();
int main()
{
    ListTest();
    StringTest();
    unsigned char i = -1;
    printf("按任意键结束%d...",i);
    return _getch();
}
typedef struct  {
    int id;
    char name[20];
} Student;
int lookupStudent(Student* student,void* id) {
    return student->id == (int)id;
}
void ListTest() {
    printf_s("=====test list====\n");
    List* list = List_construct(0, sizeof(Student));
    printf_s("创建列表:count=%d\n",(int)list->count);
    Student* stu = (Student*)List_add(list);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 1;
    memcpy(stu->name, "jack", 5);
    
    stu = (Student*)List_add(list);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 2;
    memcpy(stu->name, "eva", 4);

    stu = (Student*)List_add(list);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 3;
    memcpy(stu->name, "rose", 5);

    stu = List_find(list, 0,0);
    printf_s("获取0号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_find(list, (void*)1,0);
    printf_s("获取1号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_find(list, (void*)2,0);
    printf_s("获取2号元素:id=%d,name=%s\n", stu->id, stu->name);

    stu = List_find(list, lookupStudent,(void*)2);
    printf_s("获取id=2的元素:id=%d,name=%s\n", stu->id, stu->name);

    int c = List_remove(list,(void*)1,0);
    printf_s("移除[1]元素:count=%d\n", (int)list->count);
    stu = List_find(list, (void*)0, 0);
    printf_s("获取0号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_find(list, (void*)1, 0);
    printf_s("获取1号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_find(list, (void*)2, 0);
    printf_s("can not find [2]:%ld\n", (long)stu);
}

void StringTest() {
    printf_s("=====test string====\n");
    String* str = String_construct(0, L"hello kitty! how kit are you?", 0);
    printf_s("创建字符串:%ls\n",String_buffer(str));
    String_print(str);
    String* patten = String_construct(0, L"kit", 0);
    size_t at = String_search(str,patten,10);
    printf_s("\nsearch\"%ls\" in \"%ls\"\nfound at%d\n", String_buffer(patten), String_buffer(str),at);
    String* substr = String_substr(str,at,-1);
    printf_s("sub(%d)=%ls\n", at, String_buffer(substr));
}

