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
	typedef struct stTLogger {
		vftptr_t vftptr;
		LogLevels level;
	} TLogger;
	typedef void (*LoggerOutput)(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args);

	extern struct stVTBL TLoggerVTBL;

	extern TLogger* TLogger_default;
	static inline void TLogger_output__virtcal__(TLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
		((LoggerOutput)TLoggerVTBL.vfp0)(self, lv, category, message, args);
	}
	void assert(const byte_t* category, bool_t condition, const byte_t* message, ...);
	void terminat(word_t code, const byte_t* message, ...);
	TLogger* TLogger__construct__(TLogger* self, void* opts);
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
	void TLogger__printf(const byte_t* p, void* args);
#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TLOGGER_INCLUDED__
