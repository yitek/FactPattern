#include "loggers_module.h"
#include "Logger.h"

bool_t loggers_initialized = 0;

void loggers_module(void* p) {
	if (loggers_initialized) return;
	loggers_initialized = 1;
	loggerMETA.offset = 0;
	loggerMETA.output = &Logger__output;
	
	Logger_default = (Logger*)((byte_t*)&Logger_defaultInstance + sizeof(GCUnitLayout));
	Logger__construct__(Logger_default, 0);
	Logger_defaultInstance.level = 0;
}


