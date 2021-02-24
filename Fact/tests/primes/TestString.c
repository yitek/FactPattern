#include "TestString.h"
#include "../../Test.h"


void testString() {
	Test_begin("String", "START TEST: String.h =="); 
	String* s1 = String__construct__(0, (const byte_t*)"空数1", 0, 0, 0, MemoryKind_normal);
	Test_assert("String.__construct__",s1 && s1->length==3,"新建数组\"%s\",{length:%ld,bytes:%ld}",String_buffer(s1),s1->length,s1->bytes);
	String* s2 = String__construct__(0, (const byte_t*)"空数2", 0, 0, 0, MemoryKind_normal);
	Test_assert("String.__construct__", s2 && s2->length == 3, "新建数组\"%s\",{length:%ld,bytes:%ld}", String_buffer(s2), String_length(s2), s2->bytes);

	String* s12 = String_concat(s1, s2, 0, 0, MemoryKind_normal);
	Test_assert("String.concat", s12 && s12->length == 6 && s12->bytes == s1->bytes + s2->bytes, "s1+s2 = \"%s\",{length:%ld,bytes:%ld}", String_buffer(s12), String_length(s12), s12->bytes);

	String* s3 = String_clip(s12, 2, 2, 0, 0, 0);
	Test_assert("String.clip", s3 && s3->length == 2 , "s3 = s12.clip(2,2) = \"%s\",{length:%ld,bytes:%ld}", String_buffer(s3), String_length(s3), s3->bytes);

	usize_t at = String_search(s12, s3, 0, 0);
	Test_assert("String.search", at == 2, "s12.search(s3,2) =%ld",at);

	unichar_t c = String__get__(s12,2);
	Test_assert("String.__get__", c == '1', "s12[2] =%c", c);

	String* s4 = String__construct__(0, (const byte_t*)"空数1", 0, 0, 0, MemoryKind_normal);
	favor_t s = String__compare__(s1,s4);
	Test_assert("String.__compare__",s==0,"s1 == s4");
	s = String__compare__(s3, s2);
	Test_assert("String.__compare__", s < 0, "s3 < s2");
	s = String__compare__(s12, s3);
	Test_assert("String.__compare__", s > 0, "s12 > s3");


	String__destruct__(s1, 0, 0);
	String__destruct__(s2, 0, 0);
	String__destruct__(s12, 0, 0);
	String__destruct__(s3, 0, 0);
	String__destruct__(s4, 0, 0);
	Test_end();
}