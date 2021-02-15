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
	
}LogLevels;
typedef struct stLogger {
	LogLevels level;
	void (*output)(struct stLogger* self,LogLevels lv,const char_t* category,const char_t* message,void* args);
} Logger;

extern Logger* defaultLoggerInstance;
void Logger_printf(char_t* p, void* args);
void terminate(word_t code,const char_t* message,...);
Logger* Logger___construct__(Logger* self,void* opts);
void Logger_Trace(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Message(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Info(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Success(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Notice(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Warn(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Exception(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Error(Logger* self, const char_t* category, const char_t* message, ...);
void Logger_Log(Logger* self,LogLevels level, const char_t* category, const char_t* message, ...);
void assert(const char_t* category,bool_t condition,const char_t* message,...);
inline Logger* Logger_default() {
	return  defaultLoggerInstance ? defaultLoggerInstance : (defaultLoggerInstance= Logger___construct__(0,0));
}

#endif // end ifndef __LOGGER_INCLUDED__
