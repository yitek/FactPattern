#include "TLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>


TLogger* defaultLoggerInstance;

void setLevelColor(LogLevels level);


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




void TLogger_printf(const byte_t* p, void* args) {
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
					printf_s("%lc", va_arg((*(va_list*)args), char_t));
					hasPlaceholder = 0;
					continue;
				}
				if (ch == 'd') {
					printf_s("%ld", va_arg((*(va_list*)args), long));
					hasPlaceholder = 0;
					continue;
				}
				if (ch == 's') {
					printf_s("%ls", va_arg((*(va_list*)args), char_t*));
					hasPlaceholder = 0;
					continue;
				}

			}
		}
		else {
			if (ch == 'c') {
				printf_s("%c", va_arg((*(va_list*)args), char));
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

inline void printNow() {
	time_t  t;
	time(&t);
	char tbuffer[32];
	ctime_s(tbuffer, 32, &t);
	char* p = tbuffer;
	while (*p != '\n') { putchar(*p); p++; }
}
void terminate(word_t code, const byte_t* message, ...) {
	setLevelColor(LogLevel_Error);
	printNow();
	printf_s(" [Terminate] with code:%d\n", code);
	if (message) {
		va_list valist;
		va_start(valist, message);
		TLogger_printf(message, &valist);
		va_end(valist);
	}
	
	
	exit(code);
	
}

void TLogger_output(struct stTLogger* self, LogLevels lv, const byte_t* category ,const byte_t* message,void* args) {
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
	TLogger_printf(message,args);
	setLevelColor(0);
	
}

void TLogger_log(TLogger* self, LogLevels level, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, level, category, message, &valist);
	va_end(valist);
}
void TLogger_trace(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Trace, category, message, &valist);
	va_end(valist);
}


void TLogger_message(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Message, category, message, &valist);
	va_end(valist);
}
void TLogger_info(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Info, category, message, &valist);
	va_end(valist);
}
void TLogger_success(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Success, category, message, &valist);
	va_end(valist);
}
void TLogger_notice(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Notice, category, message, &valist);
	va_end(valist);
}

void TLogger_warn(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Warn, category, message, &valist);
	va_end(valist);
}
void TLogger_exception(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Exception, category, message, &valist);
	va_end(valist);
}
void TLogger_error(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Error, category, message, &valist);
	va_end(valist);
}
void TLogger_sectionBegin(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_SectionBegin, category, message, &valist);
	va_end(valist);
}

void TLogger_sectionEnd(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_SectionEnd, category, message, &valist);
	va_end(valist);
}

void assert(const byte_t* category,bool_t condition,const byte_t* message, ...) {
	TLogger* logger = TLogger_default();
	LogLevels rs = condition ? LogLevel_Success : LogLevel_Error;
	va_list valist;
	va_start(valist, message);
	logger->output(logger, rs, category, message, &valist);
	va_end(valist);
}

TLogger* TLogger___construct__(TLogger* self, void* opts) {
	if (!self) {
		self = malloc(sizeof(TLogger));
		if (!self) {
			system("color 04");
			printf_s("[TLogger___construct__]:Cannot allocate memory.");
			exit(1);
			return 0;
		}
	}
	self->output = TLogger_output;
	self->level = LogLevel_None;
	return self;
}

void TLogger___destruct__(TLogger* self, bool_t existed) {
	if (!existed) {
		if (self == defaultLoggerInstance)defaultLoggerInstance = 0;
		free(self);
	}
}