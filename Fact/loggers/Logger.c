#include "Logger.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#ifdef __WIN__
#include <windows.h>
#endif

LoggerMETA loggerMETA;

LoggerGCLayout Logger_defaultInstance;
Logger* Logger_default =0;


void setLevelColor(LogLevels level);

#ifdef __WIN__
void setLevelColor(LogLevels level){
	
	
	WORD color = 7;
	if (level == LogLevel_SectionEnd) color = 8 << 4 | 15; // 灰底白字
	if (level == LogLevel_SectionBegin) color = 14 << 4 | 9; // 黄底蓝字
	if (level == LogLevel_Error) color = 4; // 浅红色
	if (level == LogLevel_Exception) color=12; // 浅红色
	if (level == LogLevel_Warn)color = 6; //黄色
	if (level == LogLevel_Notice)color = 1; //蓝色
	if (level == LogLevel_Success) color = 2; //绿色
	if (level == LogLevel_Info) color=15; //白色
	if (level == LogLevel_Message) color = 8; //灰色 7 = 白色
	if (level == LogLevel_Trace) color=5; //紫色
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	
}

#endif


void Logger__printf(const byte_t* p, void* args) {
	bool_t hasPlaceholder = 0;
	byte_t prev = 0;
	byte_t ch=0;
	while (*p) {
		ch = *p;p++;
		if (ch == L'%') {
			if (hasPlaceholder) putchar('%');
			else hasPlaceholder = 1;
			continue;
		}
		if (!hasPlaceholder) {
			putchar(ch); continue;
		}
		if (prev) {
			if (prev == L'l') {
				if (ch == 'c') {
					printf_s("%lc", va_arg((*(va_list*)args), int));
					hasPlaceholder = 0;
					continue;
				}
				if (ch == 'd') {
					printf_s("%ld", va_arg((*(va_list*)args), long));
					hasPlaceholder = 0;
					continue;
				}
				if (ch == 's') {
					printf_s("%ls", va_arg((*(va_list*)args), wchar_t*));
					hasPlaceholder = 0;
					continue;
				}

			}
		}
		else {
			if (ch == 'c') {
				printf_s("%c", va_arg((*(va_list*)args), int));
				hasPlaceholder = 0;
				continue;
			}
			if (ch == 'd') {
				printf_s("%d", va_arg((*(va_list*)args), int));
				hasPlaceholder = 0;
				continue;
			}
			if (ch == 's') {
				printf_s("%s", va_arg((*(va_list*)args), byte_t*));
				hasPlaceholder = 0;
				continue;
			}
			if (ch == 'p') {
				printf_s("%p", va_arg((*(va_list*)args), void*));
				hasPlaceholder = 0;
				continue;
			}
			if (ch == 'f') {
				printf_s("%f", va_arg((*(va_list*)args), double));
				hasPlaceholder = 0;
				continue;
			}
			if (ch == 'x') {
				printf_s("%x", va_arg((*(va_list*)args), word_t));
				hasPlaceholder = 0;
				continue;
			}
			prev = ch;
		}
	}
	if (ch != '\n')putchar('\n');
}

void printNow() {
	time_t  t;
	time(&t);
	char tbuffer[32];
	ctime_s(tbuffer, 32, &t);
	char* p = tbuffer;
	while (*p != '\n') { putchar(*p); p++; }
}
word_t log_exit(word_t code, const byte_t* category, const byte_t* message, ...) {
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(Logger_default, LogLevel_Error, category, message, &valist);
	va_end(valist);
	exit(code);
	return 0;
}

void Logger__output(struct stLogger* self, LogLevels lv, const byte_t* category ,const byte_t* message,void* args) {
	if (lv < self->level) return;
	
	if (lv >=LogLevel_Error) {
		if(lv==LogLevel_SectionBegin) putchar('\n');
		setLevelColor(LogLevel_Message);
		printNow(); putchar(' ');
		setLevelColor(lv);
	}
	else {
		setLevelColor(LogLevel_Message);
		printNow(); putchar(' ');
		setLevelColor(LogLevel_Info);
		putchar('[');
		setLevelColor(lv);
	}
	
	
	if (category) printf_s("%s",category);
	else printf_s("LOG");
	if(lv<LogLevel_Error)setLevelColor(LogLevel_Info);
	putchar(']');putchar(':'); putchar(' ');
	if (lv < LogLevel_Error)setLevelColor(0);
	//
	if (args == 0) {
		printf_s("%s\n", message);
		return;
	}
	Logger__printf(message,args);
	setLevelColor(0);
	
}

void Logger_log(Logger* self, LogLevels level, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, level, category, message, &valist);
	va_end(valist);
}
void Logger_trace(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Trace, category, message, &valist);
	va_end(valist);
}


void Logger_message(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Message, category, message, &valist);
	va_end(valist);
}
void Logger_info(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Info, category, message, &valist);
	va_end(valist);
}
void Logger_success(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Success, category, message, &valist);
	va_end(valist);
}
void Logger_notice(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Notice, category, message, &valist);
	va_end(valist);
}

void Logger_warn(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Warn, category, message, &valist);
	va_end(valist);
}
void Logger_exception(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Exception, category, message, &valist);
	va_end(valist);
}
void Logger_error(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_Error, category, message, &valist);
	va_end(valist);
}
void Logger_sectionBegin(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(self, LogLevel_SectionBegin, category, message, &valist);
	va_end(valist);
}

void Logger_sectionEnd(Logger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = Logger_default;
	va_list valist;
	va_start(valist, message);
	Logger__output(self, LogLevel_SectionEnd, category, message, &valist);
	va_end(valist);
}

void log_assert(const byte_t* category,bool_t condition,const byte_t* message, ...) {
	Logger* logger = Logger_default;
	LogLevels rs = condition ? LogLevel_Success : LogLevel_Error;
	va_list valist;
	va_start(valist, message);
	Logger__output(logger, rs, category, message, &valist);
	va_end(valist);
}

Logger* Logger__construct__(Logger* self, LoggerOptions* opts) {
	if (!self) {
		self = m_allocate(Logger,0);
		if (!self) {
			log_exit(0,"Logger.__construct__","Cannot allocate memory.");
		}
	}
	self->__meta__ = (ObjectMetaLayout*)&loggerMETA;
	if (opts) {
		m_copy(&self->level,opts,sizeof(LoggerOptions));
	}
	else {
		self->level = LogLevel_None;
	}
	
	return self;
}

void Logger__destruct__(Logger* self, bool_t existed) {
	if (!existed) {
		if (self != Logger_default)m_free(self);
	}
}

