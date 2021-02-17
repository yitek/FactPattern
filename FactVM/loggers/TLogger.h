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
typedef enum {
	LogLevel_None  = 0,
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
	LogLevels level;
	void (*output)(struct stTLogger* self,LogLevels lv,const byte_t* category,const byte_t* message,void* args);
} TLogger;

extern TLogger* defaultLoggerInstance;
void TLogger_printf(const byte_t* p, void* args);
void terminate(word_t code,const byte_t* message,...);
TLogger* TLogger___construct__(TLogger* self,void* opts);
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
void TLogger_log(TLogger* self,LogLevels level, const byte_t* category, const byte_t* message, ...);
void assert(const byte_t* category,bool_t condition,const byte_t* message,...);
inline TLogger* TLogger_default() {
	return  defaultLoggerInstance ? defaultLoggerInstance : (defaultLoggerInstance= TLogger___construct__(0,0));
}

#endif // end ifndef __LOGGER_INCLUDED__
