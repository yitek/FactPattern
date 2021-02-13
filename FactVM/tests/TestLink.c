#include "TestLink.h"
#include "../memories/Memory.h"
#include <stdio.h>

bool_t fnLinkPredicate(void* item, size_t index, void* args) {
	LinkStudent* stu =(LinkStudent*) ((char*)item - sizeof(Link));
	printf_s("in predicate,index=%d,args=%p\n",index,args);
	return stu->age == (int)args;
}
void testLink() {
	printf_s("\n== START TEST: Link.h ==\n");
	Memory* mm = Memory_default();
	LinkStudent* stu = mm->require(mm, sizeof(LinkStudent), 0);
	stu->next = 0;
	stu->no = 1;
	stu->name = L"Jack";
	stu->age = 12;
	printf_s("链长:%d\n",Link_length((Link*)stu));
	size_t index = Link_searchByItem((Link*)stu, &stu->no, sizeof(LinkStudent) - sizeof(Link)).index;
	printf_s("按item搜索:index=%d,item= { no: %d, name: %ls, age: %d }\n",index ,stu->no, stu->name, stu->age);


	LinkStudent* stu1 = mm->require(mm, sizeof(LinkStudent), 0);
	stu1->no = 2;
	stu1->name = L"Rose";
	stu1->age = 13;
	Link_append((Link*)stu, (Link*)stu1,0);
	printf_s("链表添加节点(len=%d)\n", Link_length((Link*)stu));
	LinkStudent*  stu0 = (LinkStudent*)Link_searchByValue((Link*)stu, 2).item;
	printf_s("按值(value=2)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	LinkStudent* stu2 = mm->require(mm, sizeof(LinkStudent), 0);
	stu2->no = 3;
	stu2->name = L"Petter";
	stu2->age = 14;
	Link_append((Link*)stu, (Link*)stu2, (Link*)stu1);
	printf_s("链表添加节点(len=%d)\n", Link_length((Link*)stu));
	 stu0 = (LinkStudent*)Link_searchByIndex((Link*)stu, 2);
	printf_s("按项(stu2 Petter)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);
	LinkStudent* stu3 = mm->require(mm, sizeof(LinkStudent), 0);
	stu3->no = 4;
	stu3->name = L"Jess";
	stu3->age = 15;
	Link_append((Link*)stu, (Link*)stu3,0);
	printf_s("链表添加节点(len=%d)\n", Link_length((Link*)stu));
	stu0 = (LinkStudent*)Link_searchByPredicate((Link*)stu, fnLinkPredicate,(void*)15).item;
	printf_s("按项(stu2 Jess)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	stu0 = (LinkStudent*)Link_removeByIndex((Link*)stu,1).link;
	printf_s("链[1]被移除(len=%d): { no: %d, name: %ls, age: %d }\n",Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByPredicate((Link*)stu, fnLinkPredicate,(void*)14).link;
	printf_s("链[age=14]被移除(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByValue((Link*)stu, 1).link;
	printf_s("链[value=1]被移除(len=%d)(表头无法被移除..): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByItem((Link*)stu,&stu3->no,sizeof(LinkStudent) -sizeof(Link)).link;
	printf_s("链[item=4]被移除(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);

	mm->release(mm, stu);
	mm->release(mm, stu1);
	mm->release(mm, stu2);
	mm->release(mm, stu3);
	printf_s("== END TEST: Link.h ==\n");
} 