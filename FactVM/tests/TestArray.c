#include "TestArray.h"
#include "../loggers/Logger.h"
#include <stdio.h>
void testArray() {
	Logger_sectionBegin(0,L"Array",L"Test start...");
	ArrayStudent students[3];
	students[0].no = 1;
	students[0].name = L"Jack";
	students[0].age = 12;
	students[1].no = 2;
	students[1].name = L"Rose";
	students[1].age = 13;
	students[2].no = 3;
	students[2].name = L"Peter";
	students[2].age = 11;

	ArrayStudent students1[3];
	students1[0].no = 4;
	students1[0].name = L"Jess";
	students1[0].age = 12;
	students1[1].no = 5;
	students1[1].name = L"Willim";
	students1[1].age = 13;
	students1[2].no = 6;
	students1[2].name = L"Caro";
	students1[2].age = 11;
	const Array* first = Array___construct__(0, &students, 3, sizeof(ArrayStudent), 0, 0);
	assert(L"Array.__construct__",first!=0,L"生成第1个数组(length=%d)\n",Array_length(first));
	
	ArrayStudent* stu = Array_buffer(first);
	printf_s("获取缓冲区(指向第一个元素的指针): { no: %d, name: %ls, age: %d }\n", stu->no, stu->name, stu->age);

	const Array* seconds = Array___construct__(0,&students1,2,sizeof(ArrayStudent),0,0);
	printf_s("生成第二个数组(length=%d)\n", Array_length(seconds));

	const Array* concated = Array_concat(first, seconds, sizeof(ArrayStudent),0,0);
	printf_s("将2个数组连接起来(length=%d)\n", Array_length(concated));

	const Array* clip = Array_clip(concated,2,2,sizeof(ArrayStudent),0,0);
	printf_s("将连接后的数组切片，从[2]元素开始，取2个元素(length=%d)\n", Array_length(clip));

	stu = Array___INDEXGETER__(clip, 0, sizeof(ArrayStudent));
	printf_s("获取[0]的学生: { no: %d, name: %ls, age: %d}\n", stu->no, stu->name, stu->age);
	stu = Array___INDEXGETER__(clip, 1, sizeof(ArrayStudent));
	printf_s("获取[1]的学生: { no: %d, name: %ls, age: %d}\n", stu->no, stu->name, stu->age);


	Logger_sectionEnd(0,L"Array",L"Test done!");
}