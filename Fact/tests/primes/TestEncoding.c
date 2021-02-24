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
	m_look(s1, 4, (MLookTake)4);
	
	EncodingCountResult rs1 = UTF8_count(s1);
	favor_t len1 = m_strlen(s1);
	Test_assert("UTF8.count", rs1.charCount == 2 && rs1.byteCount == len1, "encode8(\"%s\"):length:%d,bytes:%d,strlen()=%d", s1, rs1.charCount, rs1.byteCount, len1);
	
	const utf8_t* left = "hi iyhit ";
	const utf8_t* right = "hit";

	EncodingCountResult srs = UTF8_sunday(left, 13, right, 3, 0);

	Test_assert("UTF8.sunday",srs.charCount==5,"search(%s) in (%s):charAt:%lu,byteAt:%lu",left,right,srs.charCount,srs.byteCount);


	EncodingCountResult srs1 = UTF8_sunday(utf8Str, len, s1, len1, 0);
	Test_assert("UTF8.sunday", srs.charCount == 5, "search(%s) in (%s):charAt:%lu,byteAt:%lu", utf8Str, s1, srs.charCount, srs.byteCount);
	const utf8_t* scp = UTF8_skip(utf8Str, srs.charCount);
	const utf8_t* pcp = UTF8_skip(s1,0);
	bool_t equal = m_equal(scp, pcp, len1);
	Test_assert("UTF8.skip",equal&&scp-utf8Str==srs1.byteCount&&equal,"[%p] + %ld = [%p]",utf8Str,srs1.byteCount,s1);

	utf32_t c0 = UTF8_get(s1, 0);
	utf32_t c1 = UTF8_get(utf8Str, srs.charCount);
	// 节: 33410 0x8282
	Test_assert("UTF8.get",c0==c1 && c0==33410 , "UTF8_get(s1)=n%u == 33410==0x8282", c1);

	utf32_t c32 = 0; utf16_t c16 = 0;
	//UTF8_convertToUTF32(s2, &c32);//33410
	//UTF8_convertToUTF16(s2, &c16); // 0x8282
	//putwchar(c32); putwchar(c16);

	Test_end();
}

void testEncoding() {
	Test_begin("Encoding","test start:");
	testUtf8();
	Test_end();
}