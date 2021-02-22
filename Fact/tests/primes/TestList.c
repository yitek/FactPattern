#include "TestList.h"
#include "../../Test.h"

void testList() {
	Test_begin("List","START TEST: List.h ==");
	List* list = List__construct__(0, 0, 0,0);
	Test_assert("List.__construct__",list && list->length==0 && list->head==0&& list->tail==0 && List_length(list)==0,"创建链表(len=%d)\n",List_length(list));
	ListStudent* stu;
	ListStudent* stu0 = stu = List_append(list, sizeof(ListStudent), 0, 0,0);
	stu->no = 1;
	stu->name = "Jack";
	stu->age = 12;
	Test_assert("List.append"
		,stu!=0
		&& List_length(list)==1
		&& list->head == ((Link*)stu)-1
		&& list->head==list->tail
		,"追加元素(len=%d): { no: %d, name: %s, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);
	
	ListStudent* stu1 = stu = List_unshift(list, sizeof(ListStudent), 0, 0,0);
	stu->no = 2;
	stu->name = "Rose";
	stu->age = 13;
	Test_assert("List.unshift"
		,list->length==2
		&& list->head == ((Link*)stu1) - 1
		&& list->tail == ((Link*)stu0) - 1
		,"队首追加元素(len=%d): { no: %d, name: %s, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);

	ListStudent* stu2 = stu = List_push(list, sizeof(ListStudent), 0, 0,0);
	stu->no = 3;
	stu->name = "Petter";
	stu->age = 14;
	Test_assert("List.push"
		, List_length(list)==3
		&& list->tail == ((Link*)stu2) - 1
		,"队尾追加元素(len=%d): { no: %d, name: %s, age: %d }\n", List_length(list), stu->no, stu->name, stu->age);


	Array* arr = List_toArray(list,0,sizeof(ListStudent),0,0,0);
	bool_t isPass = 1;
	
	for (usize_t i = 0, j = Array_length(arr); i < j; i++) {
		stu = Array__get__(arr, i, sizeof(ListStudent));
		
		if (i == 0 && stu->no != 2) {
			isPass = 0; break;
		}
		else if (i == 1 && stu->no != 1) {
			isPass = 0; break;
		}
		else if (i == 2 && stu->no != 3) {
			isPass = 0; break;
		}
		TLogger_message(Test__logger,"List.toArray","\t数组元素[%d]: { no: %d, name: %ls, age: %d }\n", i, stu->no, stu->name, stu->age);

	}
	Test_assert("List.toArray",isPass, "转数组(len=%d)\n", Array_length(arr));

	ListStudent student;
	List_pop(list, &student,sizeof(ListStudent),0);
	Test_assert("List.pop",student.age==14&& List_length(list)==2,"队尾弹出元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), student.no, student.name, student.age);
	List_shift(list, &student, sizeof(ListStudent), 0);
	Test_assert("List.shift", student.age == 13 && List_length(list) == 1, "队尾弹出元素(len=%d): { no: %d, name: %ls, age: %d }\n", List_length(list), student.no, student.name, student.age);

	List__destruct__(list,0, 0);


	Test_end();
}