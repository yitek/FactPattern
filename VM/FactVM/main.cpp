// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GCMemory.h"


void ListTest();
void StringTest();

void AlignAllocatorTest();

int main()
{
    ListTest();
    StringTest();
    AlignAllocatorTest();
    unsigned char i = -1;
    printf("按任意键结束%d...",i);
    return _getch();
}
typedef struct  {
    int id;
    char name[20];
} Student;
int lookupStudent(Student* student,size_t index,void* id) {
    return student->id == (int)id;
}
void ListTest() {
    printf_s("=====test list====\n");
    List* list = List___construct__(0, 0,0);
    printf_s("创建列表:count=%d\n",(int)list->count);
    Student* stu = (Student*)List_append(list,sizeof(Student),0, 0);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 1;
    memcpy(stu->name, "jack", 5);
    
    stu = (Student*)List_append(list, sizeof(Student), 0, 0);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 2;
    memcpy(stu->name, "eva", 4);

    stu = (Student*)List_append(list, sizeof(Student), 0, 0);
    printf_s("新添一个元素:count=%d\n", (int)list->count);
    stu->id = 3;
    memcpy(stu->name, "rose", 5);

    stu = (Student*)List_search(list, 0,0);
    printf_s("获取0号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = (Student*)List_search(list, (LinkPredicate)1,0);
    printf_s("获取1号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = (Student*)List_search(list, (LinkPredicate)2,0);
    printf_s("获取2号元素:id=%d,name=%s\n", stu->id, stu->name);

    stu = (Student*)List_search(list, (LinkPredicate)lookupStudent,(void*)2);
    printf_s("获取id=2的元素:id=%d,name=%s\n", stu->id, stu->name);

    int c = List_remove(list,(LinkPredicate)1,0,0);
    printf_s("移除[1]元素:count=%d\n", (int)list->count);
    stu = (Student*)List_search(list, (LinkPredicate)0, 0);
    printf_s("获取0号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = (Student*)List_search(list, (LinkPredicate)1, 0);
    printf_s("获取1号元素:id=%d,name=%s\n", stu->id, stu->name);
    stu = (Student*)List_search(list, (LinkPredicate)2, 0);
    printf_s("can not find [2]:%ld\n", (long)stu);
}

void StringTest() {
    printf_s("=====test string====\n");
    
    const String* str = String___construct__(0, (const char_t*)L"hello kitty! how kit are you?", 0,0,0);
    printf_s("创建字符串:%ls\n", (const wchar_t*)String_buffer(str));
    String_cout(str);
    const String* patten = String___construct__(0, (const char_t*)L"kit", 0,0,0);
    size_t at = String_search(str,patten,10);
    printf_s("\nsearch\"%ls\" in \"%ls\"\nfound at%d\n", (const wchar_t*)String_buffer(patten), (const wchar_t*)String_buffer(str),at);
    const String* substr = String_substr(str,at,-1,0,0);
    printf_s("sub(%d)=%ls\n", at, (const wchar_t*)String_buffer(substr));
}

void AlignAllocatorTest() {
    printf_s("=====test align Allocator====\n");
    GCMemoryOptions opts;
    opts.pageSize = 24;
    opts.pageCount = 10;
    opts.enableGC = 0;
    Memory* allocator = (Memory*)GCMemory_construct(0, &opts);
    printf_s("创建分配器\n");
    void* p1 = allocator->require(allocator,4,0);
    printf_s("Require一个单元\n");
    void* p2 = allocator->require(allocator, 4, 0);
    printf_s("Require一个单元\n");
    void* p3 = allocator->require(allocator, 4, 0);
    printf_s("Require一个单元\n");
    void* p4 = allocator->require(allocator, 4, 0);
    printf_s("Require一个单元\n");
    int ok= allocator->release(allocator,p2);
    printf_s("释放一个单元%d\n",ok);
    void* p5 = allocator->require(allocator, 4, 0);
    printf_s("分配一个单元,same addr as first time:%d\n",p5==p2);

}
