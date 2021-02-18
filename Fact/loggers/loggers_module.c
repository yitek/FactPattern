#include "loggers_module.h"
#include "TLogger.h"

bool_t loggers_initialized = 0;

void loggers_module(void* p) {
	if (loggers_initialized) return;
	loggers_initialized = 1;
	loggerVTBL.offset = 0;
	loggerVTBL.vfp0 = &TLogger__output;
	TLogger_defaultInstance.level = 0;
	TLogger_default = (TLogger*)((byte_t*)&TLogger_defaultInstance + sizeof(TGCUnitLayout));
}


