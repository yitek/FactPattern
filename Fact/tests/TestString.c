#include "TestString.h"
#include "../loggers/Logger.h"
#include <stdio.h>

void testString() {
	Logger_sectionBegin(0, "String", "Test start...");
	const String* str1 = String_construct(0, "你好严一", 0, 0, 0);
	assert("String.construct",str1 && str1->length==4,"创建第一个字符串[%p](length=%d)",str1,String_length(str1));
	String_coutln(str1);

	const String* str2 = String_construct(0, "你好..I'am 程序员.", 0, 0, 0);
	//const char* str = "吗";
	//printf_s("你好..I'am 程序员."); ?? 系统错误？ 吗 员不能正常显示啊？
	assert("String.construct", str2 && str2->length == 12, "创建第一个字符串[%p](length=%d):%s", str2, String_length(str2), String_buffer(str2));
	const String* str3 = String_concat(str1,str2,0,0);
	assert("String.concat", str3 && str3->length == 21, "拼接前2个字符串[%p](length=%d):%s", str3, String_length(str3),String_buffer(str3));

	

	const String* token = String_construct(0,"你好",0,0,0);

	size_t firstAt = String_search(str3,token,0,0);
	assert("String.search",firstAt==0,"拼接字符串中查找'你好'子串:%d\n", firstAt);
	size_t at = String_search(str3,token, firstAt+1, 0);
	//printf_s("拼接字符串中,以下标%d开始查找'yi'子串:%d\n", firstAt+1, at);
	size_t at1 = String_search(str3, token, at+1, 0);
	//printf_s("拼接字符串中,以下标%d开始查找'yi'子串:%d\n", at+1, at1 );

	const String* substr = String_substr(str3,at,-1,0,0);

	printf_s("获取子串(len=%d):",String_length(substr));
	//String_coutln(substr);

	//printf_s("获取子串的第2个字符%lc\n", String_get(substr,1));

	printf_s("== END TEST: String.h ==\n");

}