#include "loggers_module.h"
#include "TLogger.h"

bool_t loggers_initialized = 0;

void loggers_module(void* memory,void* p,void* p1) {
	if (loggers_initialized) return;
	loggers_initialized = 1;
	loggerMETA.offset = 0;
	loggerMETA.output = &TLogger__output;
	
	TLogger_default = (TLogger*)((byte_t*)&TLogger_defaultInstance + sizeof(MemoryRefUnit));
	TLogger__construct__(TLogger_default, 0);
	TLogger_defaultInstance.level = 0;
}


