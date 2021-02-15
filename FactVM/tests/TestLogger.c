#include "TestLogger.h"

void testLogger() {
	Logger* logger = Logger_default();
	Logger_Trace(logger,0,L"Trace message with number: %d and %d.\n",12,13);
	Logger_Message(logger, 0, L"Message with char string: %ls and %ls.\n", L"[ooo]", L"[ppp]");
	Logger_Info(0,0,L"Infomation with address:%p and %p",logger,logger);
	Logger_Success(0,L"Category",L"success with category with xn: %x,%x",23,23);
	Logger_Notice(0,0,L"Notice with c:%c and %c.",'x','y');
	Logger_Warn(0,0,L"Warn with lc:%lc,%lc",(char_t)'X',(char_t)'Y');
	Logger_Exception(0, 0, L"Exception with byte string:%s,%s", "hello", "world");
	Logger_Error(0,0,L"Error with float:%f,%f.",0.1,0.2);
}