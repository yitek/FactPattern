#include "TestEncoding.h"
#include "../../Test.h"
#include <stdio.h>
void testUtf8() {
	Test_begin("UTF8", "test start:");
	const utiny_t* utf8Str = "H1严2一3呢4\0";
	printf_s("%s\n", utf8Str);
	m_look(utf8Str,8,4);
	
	EncodingCountResult rs = UTF8_count(utf8Str);
	usize_t len = m_strlen(utf8Str);
	Test_assert("UTF8.count",1,"encode8(\"%s\"):length:%d,bytes:%d,strlen()=%d",utf8Str,rs.charCount,rs.byteCount,len);
	Test_end();
}

void testEncoding() {
	Test_begin("Encoding","test start:");
	testUtf8();
	Test_end();
}