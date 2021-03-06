#include "TestRuntime.h"
#include "../Test.h"
#include <stdio.h>
void testNum() {
	Test_begin("runtime(out..)","start test:");
	outln("test %x:");
	outs("0x0 %x:\t000000==");
	outx(0x0, 8); out('\n');
	outs("0xA234BC %x:\tA234BC==");
	outx(0xA234BC,-1); out('\n');
	outs("0xA234B0 %x6:\tA234B0==");
	outx(0xA234B0, 6); out('\n');
	outs("0xA234BC %x8:\t00A234BC==");
	outx(0xA234BC, 8); out('\n');
	outs("0xA234BC %x2:\t_06==");
	outx(0xA234BC, 2); out('\n');
	outs("0xA234BC %x5:\tA2_04==");
	outx(0xA234BC, 5); out('\n');
	outln(0);

	outln("test %u:");
	outs("123456 %u:\t123456==");
	outu(123456, -1); out('\n');
	outs("123450 %u6:\t123450==");
	outu(123450, 6); out('\n');
	outs("123456 %u8:\t00123456==");
	outu(123456, 8); out('\n');
	outs("123456 %u2:\t_06==");
	outu(123456, 2); out('\n');
	outs("123456 %u5:\t12_04==");
	outu(123456, 5); out('\n');
	outln(0);

	outln("test %d:");
	outs("-123450 %d:\t-123450==");
	outd(-123450, -1); out('\n');
	outs("-123456 %d7:\t-123456==");
	outd(-123456, 7); out('\n');
	outs("-123456 %d8:\t-0123456==");
	outd(-123456, 8); out('\n');
	outs("-123456 %d2:\t-_06==");
	outd(-123456, 2); out('\n');
	outs("-123456 %d5:\t-1_05==");
	outd(-123456, 5); out('\n');
	outln(0);

	outln("test %b:");
	outs("0b110011 %b:\t110011==");
	outb(0b110011, -1); out('\n');
	outs("0b110010 %b6:\t110010==");
	outb(0b110010, 6); out('\n');
	outs("0b110011 %b8:\t00110011==");
	outb(0b110011, 8); out('\n');
	outs("0b110011 %b2:\t_06==");
	outb(0b110011, 2); out('\n');
	outs("0b110011 %b5:\t11_04==");
	outb(0b110011, 5); out('\n');
	outln(0);

	Test_end();
}

void testFormat() {
	Test_begin("outf", "start test:");
	//outs_fmt("%\%");
	outs("format string: ");
	const char* fstr = "%d7%s,%u8%b %f12.3";
	outln(fstr);
	outln("-88,\"hello\",123456789,0b1111,32.3");
	outb(0b1111,-1);
	outs_fmt("%d7%s,%u8%b %f12.5\n",-88,"hello",123456789,0b1111,32.3);

	outcs_fmtln(OutBackColor_blue|OutForeColor_red,"hello\n\n %s\n\n","yi");
	Test_end();
}
void testLook() {
	Test_begin("m_look", "start test:");
	const char* str = "hello word\n";
	m_look(str,4,(MLookTake)4);
	Test_end();
}
void testIO() {
	Test_begin("runtime.IO", "start test:");
	testNum();
	testFormat();
	testLook();
	Test_end();
}
void testTLogger() {
	TLogger* logger = TLogger_default;
	Test_begin("TLogger", "Test start...");
	TLogger_trace(logger, "TLogger.trace", "Trace message with number: %d and %d.\n", 12, 13);
	TLogger_message(logger, "TLogger.message", "Message with char string: %ls and %ls.\n", "[ooo]", "[ppp]");
	TLogger_info(0, "TLogger.info", "Infomation with address:%p and %p", logger, logger);
	TLogger_success(0, "TLogger.success", "success with category with xn: %x,%x", 23, 23);
	TLogger_notice(0, "TLogger.notice", "Notice with c:%c and %c.", 'x', 'y');
	TLogger_warn(0, "TLogger.warn", "Warn with lc:%lc,%lc", (wchar_t)'X', (wchar_t)'Y');
	TLogger_exception(0, "TLogger.exception", "Exception with byte string:%s,%s", "hello", "world");
	TLogger_error(0, "TLogger.error", "Error with float:%f,%f.", 0.1, 0.2);

	Test_end();
}
void testRuntime(){
	Test_begin("runtime","start test");
	testIO();
	testTLogger();
	Test_end();
}