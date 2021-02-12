#include "TestString.h"
#include <stdio.h>

void testString() {
	printf_s("\n== START TEST: String.h ==\n");
	const String* str1 = String___construct__(0, L"Hello ", -1, 0, 0);
	printf_s("������һ���ַ���(%d):",String_length(str1));
	String_cout(str1); printf_s("\n");

	const String* str2 = String___construct__(0, L"Yiy. I'am Yi. How are you.", -1, 0, 0);
	printf_s("�����ڶ����ַ���:");
	String_cout(str2); printf_s("\n");

	const String* str3 = String_concat(str1,str2,0,0);
	printf_s("ƴ���ַ���:");
	String_cout(str3); printf_s("\n");

	const String* token = String___construct__(0,L"Yi",-1,0,0);

	size_t firstAt = String_search(str3,token,0,0);
	printf_s("ƴ���ַ����в���'yi'�Ӵ�:%d\n", firstAt);
	size_t at = String_search(str3,token, firstAt+1, 0);
	printf_s("ƴ���ַ�����,���±�%d��ʼ����'yi'�Ӵ�:%d\n", firstAt+1, at);
	size_t at1 = String_search(str3, token, at+1, 0);
	printf_s("ƴ���ַ�����,���±�%d��ʼ����'yi'�Ӵ�:%d\n", at+1, at1 );

	const String* substr = String_substr(str3,at,-1,0,0);

	printf_s("��ȡ�Ӵ�(len=%d):",String_length(substr));
	String_coutln(substr);

	printf_s("��ȡ�Ӵ��ĵ�2���ַ�%lc\n", String___INDEXGETER__(substr,1));

	printf_s("== END TEST: String.h ==\n");

}