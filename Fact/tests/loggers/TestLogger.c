#include "TestLogger.h"
#include <stddef.h>
void testTLogger() {
	TLogger* logger = TLogger_default;
	Test_begin("TLogger", "Test start...");
	TLogger_trace(logger,"TLogger.trace","Trace message with number: %d and %d.\n",12,13);
	TLogger_message(logger, "TLogger.message", "Message with char string: %ls and %ls.\n", "[ooo]", "[ppp]");
	TLogger_info(0, "TLogger.info","Infomation with address:%p and %p",logger,logger);
	TLogger_success(0, "TLogger.success","success with category with xn: %x,%x",23,23);
	TLogger_notice(0, "TLogger.notice","Notice with c:%c and %c.",'x','y');
	TLogger_warn(0, "TLogger.warn","Warn with lc:%lc,%lc",(wchar_t)'X',(wchar_t)'Y');
	TLogger_exception(0, "TLogger.exception", "Exception with byte string:%s,%s", "hello", "world");
	TLogger_error(0, "TLogger.error","Error with float:%f,%f.",0.1,0.2);

	Test_end();
}