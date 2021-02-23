#include "TestEncoding.h"
#include "../../Test.h"
#include <stdio.h>
void testUtf8() {
	Test_begin("UTF8", "test start:");
	const utiny_t* utf8Str = "H1严2一3呢4";
	utf8Str = "1链表添加节点，在33";
	printf_s("%s\n", utf8Str);
	m_look(utf8Str,8,(MLookTake)4);
	
	EncodingCountResult rs = UTF8_count(utf8Str);
	usize_t len = m_strlen(utf8Str);
	Test_assert("UTF8.count",rs.charCount==11&&rs.byteCount==len,"encode8(\"%s\"):length:%d,bytes:%d,strlen()=%d",utf8Str,rs.charCount,rs.byteCount,len);
	const char* s1 = "节点";
	printf_s("模式串，%s\n", s1);
	EncodingCountResult rs1 = UTF8_count(s1);
	favor_t len1 = m_strlen(s1);
	Test_assert("UTF8.count", rs1.charCount == 2 && rs1.byteCount == len1, "encode8(\"%s\"):length:%d,bytes:%d,strlen()=%d", s1, rs1.charCount, rs1.byteCount, len1);
	
	EncodingCountResult srs = UTF8_sunday("hi iyhit ", 13, "hit",3, 0);   

	EncodingCountResult srs1 = UTF8_sunday(utf8Str, len, s1, len1, 0);

	const char* s2 = UTF8_skip(utf8Str, 5);
	utf32_t c32 = 0; utf16_t c16 = 0;
	UTF8_convertToUTF32(s2, &c32);//33410
	UTF8_convertToUTF16(s2, &c16); // 0x8282
	putwchar(c32); putwchar(c16);

	Test_end();
}

void testEncoding() {
	Test_begin("Encoding","test start:");
	testUtf8();
	Test_end();
}