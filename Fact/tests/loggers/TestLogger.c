#include "TestLogger.h"
#include <stddef.h>
void testLogger() {
	loggers_module(0,0,0);
	Logger* logger = Logger_default;
	Logger_sectionBegin(logger, "Logger", "Test start...");
	Logger_trace(logger,"Logger.trace","Trace message with number: %d and %d.\n",12,13);
	Logger_message(logger, "Logger.message", "Message with char string: %ls and %ls.\n", "[ooo]", "[ppp]");
	Logger_info(0, "Logger.info","Infomation with address:%p and %p",logger,logger);
	Logger_success(0, "Logger.success","success with category with xn: %x,%x",23,23);
	Logger_notice(0, "Logger.notice","Notice with c:%c and %c.",'x','y');
	Logger_warn(0, "Logger.warn","Warn with lc:%lc,%lc",(wchar_t)'X',(wchar_t)'Y');
	Logger_exception(0, "Logger.exception", "Exception with byte string:%s,%s", "hello", "world");
	Logger_error(0, "Logger.error","Error with float:%f,%f.",0.1,0.2);

	Logger_sectionEnd(0, "Logger", "Test end.");
}