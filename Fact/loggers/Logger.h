/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../def.h"
#ifndef __LOGGER_INCLUDED__ 
#define __LOGGER_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum {
		LogLevel_None = 0,
		LogLevel_Trace = 1,
		LogLevel_Message = 2,
		LogLevel_Info = 3,
		LogLevel_Success = 4,
		LogLevel_Notice = 5,
		LogLevel_Warn = 6,
		LogLevel_Exception = 7,
		LogLevel_Error = 8,
		LogLevel_SectionBegin = 16,
		LogLevel_SectionEnd = 17
	}LogLevels;

	typedef struct stLoggerOptions {
		LogLevels level;
	}LoggerOptions;

	typedef struct stLogger {
		struct stVirtStructLayout;
		struct stLoggerOptions;
	} Logger;
	typedef void (*LoggerOutput)(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	
	extern struct stVTBL loggerVTBL;

	extern Logger* Logger_default;

	typedef struct stLoggerGCLayout{ 
		ObjectLayout __GC__;
		struct stLogger;
	}LoggerGCLayout;
	extern LoggerGCLayout Logger_defaultInstance;
	

	Logger* Logger__construct__(Logger* self, LoggerOptions* opts);
	void Logger__destruct__(Logger* self, bool_t existed);
	void Logger_trace(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_message(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_info(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_success(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_notice(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_warn(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_exception(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_error(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_sectionBegin(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_sectionEnd(Logger* self, const byte_t* category, const byte_t* message, ...);
	void Logger_log(Logger* self, LogLevels level, const byte_t* category, const byte_t* message, ...);

	void Logger__output(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	void Logger__printf(const byte_t* p, void* args);

	static inline void Logger_output__virtual__(Logger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
		((LoggerOutput)loggerVTBL.vfp0)(self, lv, category, message, args);
	}
	void log_assert(const byte_t* category, bool_t condition, const byte_t* message, ...);
	word_t log_exit(word_t code, const byte_t* category, const byte_t* message, ...);
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __LOGGER_INCLUDED__
