#include "TestArray.h"
#include "../loggers/TLogger.h"
#include <stdio.h>
void testArray() {
	TLogger_sectionBegin(0,"Array","Test start...");
	ArrayStudent students[3];
	students[0].no = 1;
	students[0].name = "Jack";
	students[0].age = 11;
	students[1].no = 2;
	students[1].name = "Rose";
	students[1].age = 12;
	students[2].no = 3;
	students[2].name = "Peter";
	students[2].age = 13;

	ArrayStudent students1[3];
	students1[0].no = 4;
	students1[0].name = "Jess";
	students1[0].age = 14;
	students1[1].no = 5;
	students1[1].name = "Willim";
	students1[1].age = 15;
	students1[2].no = 6;
	students1[2].name = "Caro";
	students1[2].age = 16;
	const Array* first = Array_construct(0, &students, 3, sizeof(ArrayStudent), 0, 0);
	assert("Array.__construct__",first!=0 && first->length==3,"生成第1个数组[%p](length=%d)\n",first,Array_length(first));
	
	ArrayStudent* stu = (ArrayStudent*)Array_buffer(first);
	assert("Array.buffer"
		,stu->no==1 && stu->age==11
		,"获取缓冲区(指向第一个元素的指针): { no: %d, name: %s, age: %d }\n", stu->no, stu->name, stu->age);

	const Array* seconds = Array_construct(0,&students1,2,sizeof(ArrayStudent),0,0);
	assert("Array.__construct__", seconds->length== 2, "生成第1个数组[%p](length=%d)\n",seconds, Array_length(seconds));
	const Array* concated = Array_concat(0,0, sizeof(ArrayStudent),0, 0, 0);
	assert("Array.concat", concated==0 && Array_length(concated)==0, "将2个空数组连接起来还是空数组[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(first, 0, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.concat", concated == first, "数组连接空数组将返回非空的那数组(左边数组)[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(0, seconds, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.concat", concated == seconds, "数组连接空数组将返回非空的那数组(右边数组)[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(first, seconds, sizeof(ArrayStudent),0, 0, 0);
	assert("Array.concat",concated->length==5,"将2个数组连接起来[%p](length=%d)\n",concated, Array_length(concated));

	const Array* clip = Array_clip(0, 2, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.clip", clip==0, "NULL数组切片得到NULL数组[%p](length=%d)\n", clip, Array_length(clip));
	clip = Array_clip(concated, 5, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.clip", clip == 0, "起始位置超过了数组长度，得到null数组[%p](length=%d)\n", clip, Array_length(clip));
	clip = Array_clip(concated, 4, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.clip", clip->length==1, "获取的长度不能超过数组总长，会被截断[%p](length=%d)\n", clip, Array_length(clip));

	clip = Array_clip(concated, 1, -1, sizeof(ArrayStudent), 0, 0, 0);
	assert("Array.clip", clip->length == 4, "不指定长度(-1),会自动截取到最后[%p](length=%d)\n", clip, Array_length(clip));
	Array_destruct((Array*)clip,0); 

	clip = Array_clip(concated,2,2,sizeof(ArrayStudent),0, 0, 0);
	assert("Array.clip",clip->length==2,"将连接后的数组[%p]切片，从[2]元素开始，取2个元素[%p](length=%d)\n",clip, Array_length(clip));

	stu = Array_get(clip, 0, sizeof(ArrayStudent));
	assert("Array.get"
		,stu->no==3 && stu->age==13
		,"获取clip[0]的学生: { no: %d, name: %s, age: %d}\n", stu->no, stu->name, stu->age);
	stu = Array_get(clip, 1, sizeof(ArrayStudent));
	assert("Array.get",stu->no==4 && stu->age==14,"获取clip[1]的学生: { no: %d, name: %s, age: %d}\n", stu->no, stu->name, stu->age);
	Array_destruct((Array*)clip, 0);
	Array_destruct((Array*)concated, 0);
	Array_destruct((Array*)first, 0);
	Array_destruct((Array*)seconds, 0);

	TLogger_sectionEnd(0,"Array","Test done!");
}