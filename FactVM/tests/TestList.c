#include "TestList.h"
#include <stdio.h>

void testList() {
	printf_s("\n== START TEST: List.h ==\n");
	List* list = List___construct__(0, 0, 0);
	printf_s("创建链表(len=%d)\n",List_length(list));
	ListStudent* stu = List_append(list, sizeof(ListStudent), 0, 0);
	stu->no = 1;
	stu->name = L"Jack";
	stu->age = 12;
	printf_s("追加元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);
	
	stu = List_unshift(list, sizeof(ListStudent), 0, 0);
	stu->no = 2;
	stu->name = L"Rose";
	stu->age = 13;
	printf_s("队首追加元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);

	stu = List_push(list, sizeof(ListStudent), 0, 0);
	stu->no = 3;
	stu->name = L"Petter";
	stu->age = 14;
	printf_s("队尾追加元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);


	Array* arr = List_toArray(list,0,sizeof(ListStudent),0,0);
	printf_s("转数组(len=%d)\n", Array_length(arr));
	for (size_t i = 0, j = Array_length(arr); i < j; i++) {
		stu = Array___INDEXGETER__(arr, i, sizeof(ListStudent));
		printf_s("\t数组元素[%d]: { no: %d, name: %ls, age: %d }\n", i, stu->no, stu->name, stu->age);

	}
	ListStudent stu0;
	List_pop(list, &stu0,sizeof(ListStudent),0);
	printf_s("队尾弹出元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), stu0.no, stu0.name, stu0.age);
	List_shift(list, &stu0, sizeof(ListStudent), 0);
	printf_s("队首弹出元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), stu0.no, stu0.name, stu0.age);


	printf_s("== END TEST: List.h ==\n");
}