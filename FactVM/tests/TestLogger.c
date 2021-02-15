#include "TestLogger.h"

void testLogger() {
	Logger* logger = Logger_default();
	Logger_sectionBegin(logger, L"Logger", L"Test start...");
	Logger_trace(logger,L"Logger.trace",L"Trace message with number: %d and %d.\n",12,13);
	Logger_message(logger, L"Logger.message", L"Message with char string: %ls and %ls.\n", L"[ooo]", L"[ppp]");
	Logger_info(0, L"Logger.info",L"Infomation with address:%p and %p",logger,logger);
	Logger_success(0, L"Logger.success",L"success with category with xn: %x,%x",23,23);
	Logger_notice(0, L"Logger.notice",L"Notice with c:%c and %c.",'x','y');
	Logger_warn(0, L"Logger.warn",L"Warn with lc:%lc,%lc",(char_t)'X',(char_t)'Y');
	Logger_exception(0, L"Logger.exception", L"Exception with byte string:%s,%s", "hello", "world");
	Logger_error(0, L"Logger.error",L"Error with float:%f,%f.",0.1,0.2);

	Logger_sectionEnd(0, L"Logger", L"Test end.");
}