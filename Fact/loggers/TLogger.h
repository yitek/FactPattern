/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../def.h"
#ifndef __TLOGGER_INCLUDED__ 
#define __TLOGGER_INCLUDED__
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
	} TLogger;
	typedef void (*LoggerOutput)(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	
	extern struct stVTBL loggerVTBL;

	extern TLogger* TLogger_default;

	typedef struct stLoggerGCLayout{ 
		TGCUnitLayout __GC__;
		struct stLogger;
	}TLoggerGCLayout;
	extern TLoggerGCLayout TLogger_defaultInstance;
	

	TLogger* TLogger__construct__(TLogger* self, LoggerOptions* opts);
	void TLogger__destruct__(TLogger* self, bool_t existed);
	void TLogger_trace(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_message(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_info(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_success(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_notice(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_warn(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_exception(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_error(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_sectionBegin(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_sectionEnd(TLogger* self, const byte_t* category, const byte_t* message, ...);
	void TLogger_log(TLogger* self, LogLevels level, const byte_t* category, const byte_t* message, ...);

	void TLogger__output(struct stLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	void TLogger__printf(const byte_t* p, void* args);

	static inline void TLogger_output__virtual__(TLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
		((LoggerOutput)loggerVTBL.vfp0)(self, lv, category, message, args);
	}
	void log_assert(const byte_t* category, bool_t condition, const byte_t* message, ...);
	void log_exit(word_t code, const byte_t* category, const byte_t* message, ...);
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TLOGGER_INCLUDED__
