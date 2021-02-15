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
typedef struct stLogger {
	LogLevels level;
	void (*output)(struct stLogger* self,LogLevels lv,const char_t* category,const char_t* message,void* args);
} Logger;

extern Logger* defaultLoggerInstance;
void Logger_printf(char_t* p, void* args);
void terminate(word_t code,const char_t* message,...);
Logger* Logger___construct__(Logger* self,void* opts);
void Logger_trace(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_message(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_info(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_success(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_notice(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_warn(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_exception(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_error(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_sectionBegin(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_sectionEnd(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_log(Logger* self,LogLevels level, const char_t* category, const char_t* message, ...);
void assert(const char_t* category,bool_t condition,const char_t* message,...);
inline Logger* Logger_default() {
	return  defaultLoggerInstance ? defaultLoggerInstance : (defaultLoggerInstance= Logger___construct__(0,0));
}

#endif // end ifndef __LOGGER_INCLUDED__
