#include "Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>


Logger* defaultLoggerInstance;

void setLevelColor(LogLevels level);


void setLevelColor(LogLevels level){
	
	
	WORD color = 7;
	if (level == LogLevel_SectionEnd) color = 8 << 4 | 15; // �ҵװ���
	if (level == LogLevel_SectionBegin) color = 14 << 4 | 9; // �Ƶ�����
	if (level == LogLevel_Error) color = 4; // ǳ��ɫ
	if (level == LogLevel_Exception) color=12; // ǳ��ɫ
	if (level == LogLevel_Warn)color = 6; //��ɫ
	if (level == LogLevel_Notice)color = 1; //��ɫ
	if (level == LogLevel_Success) color = 2; //��ɫ
	if (level == LogLevel_Info) color=15; //��ɫ
	if (level == LogLevel_Message) color = 8; //��ɫ 7 = ��ɫ
	if (level == LogLevel_Trace) color=5; //��ɫ
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ��ǰ���ھ��
	SetConsoleTextAttribute(handle, color);//������ɫ
	
}




void Logger_printf(char_t* p, void* args) {
	bool_t hasPlaceholder = 0;
	char_t prev = 0;
	char_t ch=0;
	while (*p) {
		ch = *p;p++;
		if (ch == L'%') {
			if (hasPlaceholder) putwchar(L'%');
			else hasPlaceholder = 1;
			continue;
		}
		if (!hasPlaceholder) {
			putwchar(ch); continue;
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
					printf_s("%ls", va_arg((*(va_list*)args), wchar_t*));
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
				printf_s("%ls", va_arg((*(va_list*)args), char_t*));
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
void terminate(word_t code, const char_t* message, ...) {
	setLevelColor(LogLevel_Error);
	printNow();
	printf_s(" [Terminate] with code:%d\n", code);
	if (message) {
		va_list valist;
		va_start(valist, message);
		Logger_printf((char_t*)message, &valist);
		va_end(valist);
	}
	
	
	exit(code);
	
}

void Logger_output(struct stLogger* self, LogLevels lv, const char_t* category ,const char_t* message,void* args) {
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
	
	
	if (category) printf_s("%ls",category);
	else printf_s("LOG");
	if(lv<LogLevel_Error)setLevelColor(LogLevel_Info);
	putchar(']');putchar(':'); putchar(' ');
	if (lv < LogLevel_Error)setLevelColor(0);
	//
	if (args == 0) {
		printf_s("%ls\n", message);
		return;
	}
	Logger_printf((char_t*)message,args);
	setLevelColor(0);
	
}

void Logger_log(Logger* self, LogLevels level, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, level, category, message, &valist);
	va_end(valist);
}
void Logger_trace(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Trace, category, message, &valist);
	va_end(valist);
}


void Logger_message(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Message, category, message, &valist);
	va_end(valist);
}
void Logger_info(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Info, category, message, &valist);
	va_end(valist);
}
void Logger_success(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Success, category, message, &valist);
	va_end(valist);
}
void Logger_notice(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Notice, category, message, &valist);
	va_end(valist);
}

void Logger_warn(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Warn, category, message, &valist);
	va_end(valist);
}
void Logger_exception(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Exception, category, message, &valist);
	va_end(valist);
}
void Logger_error(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_Error, category, message, &valist);
	va_end(valist);
}
void Logger_sectionBegin(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_SectionBegin, category, message, &valist);
	va_end(valist);
}

void Logger_sectionEnd(Logger* self, const char_t* category, const char_t* message, ...) {
	if (!self) self = Logger_default();
	va_list valist;
	va_start(valist, message);
	self->output(self, LogLevel_SectionEnd, category, message, &valist);
	va_end(valist);
}

void assert(const char_t* category,bool_t condition,const char_t* message, ...) {
	Logger* logger = Logger_default();
	LogLevels rs = condition ? LogLevel_Success : LogLevel_Error;
	va_list valist;
	va_start(valist, message);
	logger->output(logger, rs, category, message, &valist);
	va_end(valist);
}

Logger* Logger___construct__(Logger* self, void* opts) {
	if (!self) {
		self = malloc(sizeof(Logger));
		if (!self) {
			system("color 04");
			printf_s("[Logger___construct__]:Cannot allocate memory.");
			exit(1);
			return 0;
		}
	}
	self->output = Logger_output;
	self->level = LogLevel_None;
	return self;
}

void Logger___destruct__(Logger* self, bool_t existed) {
	if (!existed) {
		if (self == defaultLoggerInstance)defaultLoggerInstance = 0;
		free(self);
	}
}