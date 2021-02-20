/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../layout.h"
#ifndef __LOGGER_INCLUDED__ 
#define __LOGGER_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum {
		LogLevel_None = 0,
		LogLevel_SectionBegin = 1 ,
		LogLevel_SectionEnd = 1 << 1,
		LogLevel_Trace = 1<<8,
		LogLevel_Message = 1<<9,
		LogLevel_Info = 1 << 10,
		LogLevel_Success = 1 << 11,
		LogLevel_Notice = 1 << 12,
		LogLevel_Warn = 1 << 13,
		LogLevel_Exception = 1 << 14,
		LogLevel_Error = 1 << 15,
		
		
	}LogLevels;

	typedef struct stLoggerOptions {
		LogLevels level;
	}LoggerOptions;

	typedef struct stTest {
		char* category;
		usize_t assertCount;
		usize_t failCount;
		usize_t childCount;
		usize_t failedChildCount;
		struct stTest* parent;
	}Test;

	typedef struct stLogger {
		struct stTObject;
		struct stLoggerOptions;
	} Logger;

	typedef void (*LoggerOutput)(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	
	typedef struct stLoggerMETA {
		struct stObjectMetaLayout;
		LoggerOutput output;
	}LoggerMETA;

	extern LoggerMETA loggerMETA;

	extern Logger* Logger_default;

	typedef struct stLoggerLayout{ 
		struct stMemoryRefUnit;
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
	static inline void Logger_output__virtual__(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
		((LoggerMETA*)(((TObject*)self)->__meta__))->output(self, lv, category, message, args);
	}

	
	void Logger__printf(const byte_t* p, void* args);
	
	word_t log_exit(word_t code, const byte_t* category, const byte_t* message, ...);
	
	
	extern Test* Test__current;
	extern Logger* Test__logger;
	void Test_begin(const byte_t* category, const byte_t* message, ...);
	void Test_end();
	void Test_assert(const byte_t* category, bool_t condition, const byte_t* message, ...);

	typedef enum{
		
		ExitCode_memory = 1,
		ExitCode_critical = 2,
		ExitCode_argument = 3
	} ExitCodes;
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __LOGGER_INCLUDED__
