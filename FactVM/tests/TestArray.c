#include "TestArray.h"
#include "../loggers/Logger.h"
#include <stdio.h>
void testArray() {
	Logger_sectionBegin(0,L"Array",L"Test start...");
	ArrayStudent students[3];
	students[0].no = 1;
	students[0].name = L"Jack";
	students[0].age = 11;
	students[1].no = 2;
	students[1].name = L"Rose";
	students[1].age = 12;
	students[2].no = 3;
	students[2].name = L"Peter";
	students[2].age = 13;

	ArrayStudent students1[3];
	students1[0].no = 4;
	students1[0].name = L"Jess";
	students1[0].age = 14;
	students1[1].no = 5;
	students1[1].name = L"Willim";
	students1[1].age = 15;
	students1[2].no = 6;
	students1[2].name = L"Caro";
	students1[2].age = 16;
	const Array* first = Array_construct(0, &students, 3, sizeof(ArrayStudent), 0, 0);
	assert(L"Array.__construct__",first!=0 && first->length==3,L"���ɵ�1������[%p](length=%d)\n",first,Array_length(first));
	
	ArrayStudent* stu = (ArrayStudent*)Array_buffer(first);
	assert(L"Array.buffer"
		,stu->no==1 && stu->age==11
		,L"��ȡ������(ָ���һ��Ԫ�ص�ָ��): { no: %d, name: %ls, age: %d }\n", stu->no, stu->name, stu->age);

	const Array* seconds = Array_construct(0,&students1,2,sizeof(ArrayStudent),0,0);
	assert(L"Array.__construct__", seconds->length== 2, L"���ɵ�1������[%p](length=%d)\n",seconds, Array_length(seconds));
	const Array* concated = Array_concat(0,0, sizeof(ArrayStudent),0, 0, 0);
	assert(L"Array.concat", concated==0 && Array_length(concated)==0, L"��2�������������������ǿ�����[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(first, 0, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.concat", concated == first, L"�������ӿ����齫���طǿյ�������(�������)[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(0, seconds, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.concat", concated == seconds, L"�������ӿ����齫���طǿյ�������(�ұ�����)[%p](length=%d)\n", concated, Array_length(concated));
	concated = Array_concat(first, seconds, sizeof(ArrayStudent),0, 0, 0);
	assert(L"Array.concat",concated->length==5,L"��2��������������[%p](length=%d)\n",concated, Array_length(concated));

	const Array* clip = Array_clip(0, 2, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.clip", clip==0, L"NULL������Ƭ�õ�NULL����[%p](length=%d)\n", clip, Array_length(clip));
	clip = Array_clip(concated, 5, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.clip", clip == 0, L"��ʼλ�ó��������鳤�ȣ��õ�null����[%p](length=%d)\n", clip, Array_length(clip));
	clip = Array_clip(concated, 4, 2, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.clip", clip->length==1, L"��ȡ�ĳ��Ȳ��ܳ��������ܳ����ᱻ�ض�[%p](length=%d)\n", clip, Array_length(clip));

	clip = Array_clip(concated, 1, -1, sizeof(ArrayStudent), 0, 0, 0);
	assert(L"Array.clip", clip->length == 4, L"��ָ������(-1),���Զ���ȡ�����[%p](length=%d)\n", clip, Array_length(clip));
	Array_destruct((Array*)clip,0); 

	clip = Array_clip(concated,2,2,sizeof(ArrayStudent),0, 0, 0);
	assert(L"Array.clip",clip->length==2,L"�����Ӻ������[%p]��Ƭ����[2]Ԫ�ؿ�ʼ��ȡ2��Ԫ��[%p](length=%d)\n",clip, Array_length(clip));

	stu = Array_get(clip, 0, sizeof(ArrayStudent));
	assert(L"Array.get"
		,stu->no==3 && stu->age==13
		,L"��ȡclip[0]��ѧ��: { no: %d, name: %ls, age: %d}\n", stu->no, stu->name, stu->age);
	stu = Array_get(clip, 1, sizeof(ArrayStudent));
	assert(L"Array.get",stu->no==4 && stu->age==14,L"��ȡclip[1]��ѧ��: { no: %d, name: %ls, age: %d}\n", stu->no, stu->name, stu->age);
	Array_destruct((Array*)clip, 0);
	Array_destruct((Array*)concated, 0);
	Array_destruct((Array*)first, 0);
	Array_destruct((Array*)seconds, 0);

	Logger_sectionEnd(0,L"Array",L"Test done!");
}