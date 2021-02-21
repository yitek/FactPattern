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

	typedef struct stTLoggerOptions {
		LogLevels level;
	}TLoggerOptions;

	typedef struct stTest {
		char* category;
		usize_t assertCount;
		usize_t failCount;
		usize_t childCount;
		usize_t failedChildCount;
		struct stTest* parent;
	}Test;

	typedef struct stTLogger {
		struct stTObject;
		struct stTLoggerOptions;
	} TLogger;

	typedef void (*TLoggerOutput)(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	
	typedef struct stTLoggerMeta {
		struct stObjectMetaLayout;
		TLoggerOutput output;
	}TLoggerMeta;

	extern TLoggerMeta Logger__meta__;

	extern TLogger* TLogger_default;

	typedef struct stTLoggerLayout{ 
		struct stMemoryRefUnit;
		struct stTLogger;
	}TLoggerGCLayout;
	extern TLoggerGCLayout TLogger_defaultInstance;
	

	TLogger* TLogger__construct__(TLogger* self, TLoggerOptions* opts);
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

	void TLogger__output(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);
	static inline void TLogger_output__virtual__(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
		((TLoggerMeta*)(((TObject*)self)->__meta__))->output(self, lv, category, message, args);
	}

	
	void TLogger__printf(const byte_t* p, void* args);
	
	word_t log_exit(word_t code, const byte_t* category, const byte_t* message, ...);
	
	
	extern Test* Test__current;
	extern TLogger* Test__logger;
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
