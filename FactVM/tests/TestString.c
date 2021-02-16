#include "TestString.h"
#include "../loggers/Logger.h"
#include <stdio.h>

void testString() {
	Logger_sectionBegin(0, L"String", L"Test start...");
	const String* str1 = String_construct(0, L"Hello ", -1, 0, 0);
	assert(L"String.construct",str1 && str1->length==6,L"创建第一个字符串[%p](%d):",str1,String_length(str1));
	String_cout(str1); printf_s("\n");

	const String* str2 = String_construct(0, L"Yiy. I'am Yi. How are you.", -1, 0, 0);
	//assert(L"String.construct","创建第二个字符串:");
	String_cout(str2); printf_s("\n");

	const String* str3 = String_concat(str1,str2,0,0);
	printf_s("拼接字符串:");
	String_cout(str3); printf_s("\n");

	const String* token = String_construct(0,L"Yi",-1,0,0);

	size_t firstAt = String_search(str3,token,0,0);
	printf_s("拼接字符串中查找'yi'子串:%d\n", firstAt);
	size_t at = String_search(str3,token, firstAt+1, 0);
	printf_s("拼接字符串中,以下标%d开始查找'yi'子串:%d\n", firstAt+1, at);
	size_t at1 = String_search(str3, token, at+1, 0);
	printf_s("拼接字符串中,以下标%d开始查找'yi'子串:%d\n", at+1, at1 );

	const String* substr = String_substr(str3,at,-1,0,0);

	printf_s("获取子串(len=%d):",String_length(substr));
	String_coutln(substr);

	printf_s("获取子串的第2个字符%lc\n", String_get(substr,1));

	printf_s("== END TEST: String.h ==\n");

}