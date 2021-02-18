#include "loggers_module.h"
#include "TLogger.h"
TLogger TLogger_defaultInstance;
bool_t loggers_initialized = 0;

void loggers_module(void* p) {
	if (loggers_initialized) return;
	loggers_initialized = 1;
	TLoggerVTBL.offset = 0;
	TLoggerVTBL.vfp0 = &TLogger__output;
	TLogger_defaultInstance.level = 0;
	TLogger_default = &TLogger_defaultInstance;
}


