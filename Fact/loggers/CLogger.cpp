#include "CLogger.h"
#include <stdio.h>

void CLogger::message(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	printf_s(message);
}

void CLogger::output(LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
	printf_s(message);
}