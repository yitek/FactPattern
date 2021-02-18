#pragma once
/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "TLogger.h"
#ifndef __CLOGGER_INCLUDED__ 
#define __CLOGGER_INCLUDED__

#ifdef __cplusplus 
extern "C" {
#endif

	class CLogger {
	private:
		LogLevels level;
	public:
		CLogger() {
			this->level = LogLevel_None;
		}
	protected:
		virtual void output(LogLevels lv, const byte_t* category, const byte_t* message, void* args);
		void message(TLogger* self, const byte_t* category, const byte_t* message, ...);
	};
#ifdef __cplusplus 
} //extern "C" {
#endif
#endif // end ifndef __CLOGGER_INCLUDED__
