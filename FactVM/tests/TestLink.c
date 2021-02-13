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
	printf_s("����:%d\n",Link_length((Link*)stu));
	size_t index = Link_searchByItem((Link*)stu, &stu->no, sizeof(LinkStudent) - sizeof(Link)).index;
	printf_s("��item����:index=%d,item= { no: %d, name: %ls, age: %d }\n",index ,stu->no, stu->name, stu->age);


	LinkStudent* stu1 = mm->require(mm, sizeof(LinkStudent), 0);
	stu1->no = 2;
	stu1->name = L"Rose";
	stu1->age = 13;
	Link_append((Link*)stu, (Link*)stu1,0);
	printf_s("������ӽڵ�(len=%d)\n", Link_length((Link*)stu));
	LinkStudent*  stu0 = (LinkStudent*)Link_searchByValue((Link*)stu, 2).item;
	printf_s("��ֵ(value=2)����: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	LinkStudent* stu2 = mm->require(mm, sizeof(LinkStudent), 0);
	stu2->no = 3;
	stu2->name = L"Petter";
	stu2->age = 14;
	Link_append((Link*)stu, (Link*)stu2, (Link*)stu1);
	printf_s("������ӽڵ�(len=%d)\n", Link_length((Link*)stu));
	 stu0 = (LinkStudent*)Link_searchByIndex((Link*)stu, 2);
	printf_s("����(stu2 Petter)����: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);
	LinkStudent* stu3 = mm->require(mm, sizeof(LinkStudent), 0);
	stu3->no = 4;
	stu3->name = L"Jess";
	stu3->age = 15;
	Link_append((Link*)stu, (Link*)stu3,0);
	printf_s("������ӽڵ�(len=%d)\n", Link_length((Link*)stu));
	stu0 = (LinkStudent*)Link_searchByPredicate((Link*)stu, fnLinkPredicate,(void*)15).item;
	printf_s("����(stu2 Jess)����: { no: %d, name: %ls, age: %d }\n", stu0->no, stu0->name, stu0->age);

	stu0 = (LinkStudent*)Link_removeByIndex((Link*)stu,1).link;
	printf_s("��[1]���Ƴ�(len=%d): { no: %d, name: %ls, age: %d }\n",Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByPredicate((Link*)stu, fnLinkPredicate,(void*)14).link;
	printf_s("��[age=14]���Ƴ�(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByValue((Link*)stu, 1).link;
	printf_s("��[value=1]���Ƴ�(len=%d)(��ͷ�޷����Ƴ�..): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);
	stu0 = (LinkStudent*)Link_removeByItem((Link*)stu,&stu3->no,sizeof(LinkStudent) -sizeof(Link)).link;
	printf_s("��[item=4]���Ƴ�(len=%d): { no: %d, name: %ls, age: %d }\n", Link_length((Link*)stu), stu0->no, stu0->name, stu0->age);

	mm->release(mm, stu);
	mm->release(mm, stu1);
	mm->release(mm, stu2);
	mm->release(mm, stu3);
	printf_s("== END TEST: Link.h ==\n");
} 