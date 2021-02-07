// FactVM.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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
    printf("�����������%d...",i);
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
    List* list = List_construct(0, sizeof(Student),0);
    printf_s("�����б�:count=%d\n",(int)list->count);
    Student* stu = (Student*)List_append(list,0);
    printf_s("����һ��Ԫ��:count=%d\n", (int)list->count);
    stu->id = 1;
    memcpy(stu->name, "jack", 5);
    
    stu = (Student*)List_append(list, 0);
    printf_s("����һ��Ԫ��:count=%d\n", (int)list->count);
    stu->id = 2;
    memcpy(stu->name, "eva", 4);

    stu = (Student*)List_append(list, 0);
    printf_s("����һ��Ԫ��:count=%d\n", (int)list->count);
    stu->id = 3;
    memcpy(stu->name, "rose", 5);

    stu = List_search(list, 0,0);
    printf_s("��ȡ0��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_search(list, (void*)1,0);
    printf_s("��ȡ1��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_search(list, (void*)2,0);
    printf_s("��ȡ2��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);

    stu = List_search(list, lookupStudent,(void*)2);
    printf_s("��ȡid=2��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);

    int c = List_remove(list,(void*)1,0,0);
    printf_s("�Ƴ�[1]Ԫ��:count=%d\n", (int)list->count);
    stu = List_search(list, (void*)0, 0);
    printf_s("��ȡ0��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_search(list, (void*)1, 0);
    printf_s("��ȡ1��Ԫ��:id=%d,name=%s\n", stu->id, stu->name);
    stu = List_search(list, (void*)2, 0);
    printf_s("can not find [2]:%ld\n", (long)stu);
}

void StringTest() {
    printf_s("=====test string====\n");
    const String* str = String_construct(0, L"hello kitty! how kit are you?", 0,0);
    printf_s("�����ַ���:%ls\n",String_buffer(str));
    String_print(str);
    const String* patten = String_construct(0, L"kit", 0,0);
    size_t at = String_search(str,patten,10);
    printf_s("\nsearch\"%ls\" in \"%ls\"\nfound at%d\n", String_buffer(patten), String_buffer(str),at);
    const String* substr = String_substr(str,at,-1,0);
    printf_s("sub(%d)=%ls\n", at, String_buffer(substr));
}

void AlignAllocatorTest() {
    printf_s("=====test align Allocator====\n");
    GCMemoryOptions opts;
    opts.pageSize = 24;
    opts.pageCount = 10;
    opts.enableGC = 0;
    Memory* allocator = (Memory*)GCMemory_construct(0, &opts);
    printf_s("����������\n");
    void* p1 = allocator->require(allocator,4,0);
    printf_s("Requireһ����Ԫ\n");
    void* p2 = allocator->require(allocator, 4, 0);
    printf_s("Requireһ����Ԫ\n");
    void* p3 = allocator->require(allocator, 4, 0);
    printf_s("Requireһ����Ԫ\n");
    void* p4 = allocator->require(allocator, 4, 0);
    printf_s("Requireһ����Ԫ\n");
    int ok= allocator->release(allocator,p2);
    printf_s("�ͷ�һ����Ԫ%d\n",ok);
    void* p5 = allocator->require(allocator, 4, 0);
    printf_s("����һ����Ԫ,same addr as first time:%d\n",p5==p2);

}
