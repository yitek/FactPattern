#include "TestLink.h"
#include "../../Test.h"
#include <stdio.h>

bool_t fnLinkPredicate(void* item, size_t index, void* args) {
	LinkStudent* stu =(LinkStudent*) ((char*)item - sizeof(Link));
	TLogger_trace(0,"Link.predicate","in predicate,index=%d,args=%p\n",index,args);
	return stu->age == (int)args;
}
void testLink() {
	Test_begin("Link","START TEST: Link.h\n");
	
	LinkStudent* stu = (LinkStudent*)TMemory_alloc(0,sizeof(LinkStudent),0,0);
	stu->next = 0;
	stu->no = 1;
	stu->name = "Jack";
	stu->age = 12;
	Test_assert("Link.append", Link_length((Link*)stu)==1,"链长:%d\n",Link_length((Link*)stu));
	size_t index = Link_searchByItem((Link*)stu, &stu->no, sizeof(LinkStudent) - sizeof(Link)).index;
	Test_assert("Link.searchByItem",index==0,"按item搜索:index=%d,item= { no: %d, name: %ls, age: %d }\n",index ,stu->no, stu->name, stu->age);


	LinkStudent* stu1 = (LinkStudent*)TMemory_alloc(0, sizeof(LinkStudent), 0, 0);
	stu1->no = 2;
	stu1->name = "Rose";
	stu1->age = 13;
	Link_append((Link*)stu, (Link*)stu1,0);
	Test_assert("Link.length", Link_length((Link*)stu)==2,"链表添加节点(len=%d)\n", Link_length((Link*)stu));
	LinkStudent*  stu0 = (LinkStudent*)Link_searchByValue((Link*)stu, 2).item;
	Test_assert("Link.searchByValue",stu1==stu0&&stu0->age==stu1->age && stu0->no==2,"按值(value=2)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	LinkStudent* stu2 = (LinkStudent*)TMemory_alloc(0, sizeof(LinkStudent), 0, 0);
	stu2->no = 3;
	stu2->name = "Petter";
	stu2->age = 14;
	Link_append((Link*)stu, (Link*)stu2, (Link*)stu1);
	Test_assert("Link.append", Link_length((Link*)stu) == 3, "链表添加节点(len=%d)\n", Link_length((Link*)stu));
	stu0 = (LinkStudent*)Link_searchByIndex((Link*)stu, 2);
	Test_assert("Link.searchByIndex", stu0 == stu2 && stu0->age == stu2->age && stu0->no == 3, "按值(index=2)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	LinkStudent* stu3 = (LinkStudent*)TMemory_alloc(0, sizeof(LinkStudent), 0, 0);
	stu3->no = 4;
	stu3->name = "Jess";
	stu3->age = 15;
	Link_append((Link*)stu, (Link*)stu3,0);
	Test_assert("Link.append", Link_length((Link*)stu) == 4, "链表添加节点(len=%d)\n", Link_length((Link*)stu));
	stu0 = (LinkStudent*)Link_searchByPredicate((Link*)stu, fnLinkPredicate,(void*)15).item;
	Test_assert("Link.searchByPredicate", stu0 == stu3 && stu0->age == stu3->age && stu0->no == 4, "按值(index=2)搜索: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);


	stu0 = (LinkStudent*)Link_removeByIndex((Link*)stu,1).link;
	Test_assert("Link.removeByIndex"
		, Link_length((Link*)stu) == 3 && stu0==stu1
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 0) == stu
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 1) == stu2
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 2) == stu3
		,"链[1]被移除(len=%d): { no: %d, name: %ls, age: %d }\n",Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);

	stu0 = (LinkStudent*)Link_removeByPredicate((Link*)stu, fnLinkPredicate,(void*)14).link;
	Test_assert("Link.removeByPredicate"
		, Link_length((Link*)stu) == 2 && stu0 == stu2
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 0) == stu
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 1) == stu3
		, "链[1]被移除(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);


	stu0 = (LinkStudent*)Link_removeByValue((Link*)stu, 1).link;
	Test_assert("Link.removeByValue"
		, Link_length((Link*)stu) == 2 && stu0 == stu
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 0) == stu
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 1) == stu3
		, "链[value=1]执行了移除操作，但表头无法被移除，什么都没有变(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);

	stu0 = (LinkStudent*)Link_removeByItem((Link*)stu,&stu3->no,sizeof(LinkStudent) -sizeof(Link)).link;
	Test_assert("Link.removeByItem"
		, Link_length((Link*)stu) == 1 && stu0 == stu3
		&& (LinkStudent*)Link_searchByIndex((Link*)stu, 0) == stu
		, "链[stu3]执行了移除操作(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);

	TMemory_free(0, stu);
	TMemory_free(0, stu1);
	TMemory_free(0, stu2);
	TMemory_free(0, stu3);
	
	Test_end();
} 