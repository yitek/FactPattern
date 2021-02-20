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
	
	if (level & LogLevel_SectionBegin) color = 15 << 4 | 0; // 黄底蓝字
	if (level & LogLevel_SectionEnd) color = 8; // 黄底蓝字
	if (level & LogLevel_Error) color = 4<<4 | 15; // 浅红色
	if (level & LogLevel_Exception) color=12; // 浅红色
	if (level & LogLevel_Warn)color = 6; //黄色
	if (level & LogLevel_Notice)color = 1; //蓝色
	if (level & LogLevel_Success) color = 2; //绿色
	if (level & LogLevel_Info) color=15; //白色
	if (level & LogLevel_Message) color = 8; //灰色 7 = 白色
	if (level & LogLevel_Trace) color=5; //紫色
	
	if (level & LogLevel_SectionEnd) color = 7 << 4 | color;

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
		if (ch == '\n') {
			//不打印最后一个回车
			if (*p == 0) break;
		}
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
usize_t Logger__tabs = 0;
void Logger__output(struct stLogger* self, LogLevels lv, const byte_t* category ,const byte_t* message,void* args) {
	if (lv < self->level) return;
	if (lv & LogLevel_SectionBegin) putchar('\n');
	setLevelColor(LogLevel_Message);
	printNow();
	putchar(' ');
	

	if (lv & LogLevel_SectionBegin) {
		Logger__tabs++;
		for (usize_t i = 0, j = Logger__tabs - 1; i < j; i++) putchar('\t');
	}
	else if (lv & LogLevel_SectionEnd) {
		for (usize_t i = 0, j = Logger__tabs - 1; i < j; i++) putchar('\t');
		if (Logger__tabs == 0) log_exit(ExitCode_critical, "Logger._output", "Not matched SectionBegin.");
		Logger__tabs--;

	}
	else {
		for (usize_t i = 0, j = Logger__tabs - 1; i < j; i++) putchar('\t');
	}
	setLevelColor(lv);
	putchar('[');
	if (category) printf_s("%s",category);
	else printf_s("LOG");
	putchar(']');putchar(':'); putchar(' ');
	//
	if (args == 0) {
		printf_s("%s\n", message);
		return;
	}
	Logger__printf(message,args);
	setLevelColor(0);
	putchar('\n');
	if (lv & LogLevel_SectionEnd) putchar('\n');
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



Logger* Logger__construct__(Logger* self, LoggerOptions* opts) {
	if (!self) {
		self = m_allocate(Logger,0);
		if (!self) {
			log_exit(ExitCode_memory,"Logger.__construct__","Cannot allocate memory.");
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

Test* Test__current =0;
Logger* Test__logger =0;

void Test_begin(const byte_t* category, const byte_t* message, ...) {
	Test* test = m_allocate(Test, 0);
	test->category = m_cstr(category);
	if (Test__current) {
		test->parent = Test__current; 
		test->parent->childCount++;
	}
	else test->parent = 0;
	test->childCount = test->failedChildCount = test->assertCount = test->failCount = 0;
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(Test__logger ? Test__logger : Logger_default, LogLevel_SectionBegin, category, message, &valist);
	va_end(valist);
	Test__current = test;
}
void Test_assert(const byte_t* category, bool_t condition, const byte_t* message, ...) {
	Test* test = Test__current;
	LogLevels rs;
	if (condition) {
		rs = LogLevel_Success;
		if (test) {
			test->assertCount++;
		}
	}
	else {
		rs = LogLevel_Error;
		if (test) {
			test->assertCount++;
			test->failCount++;
		}
	}
	va_list valist;
	va_start(valist, message);
	Logger_output__virtual__(Test__logger?Test__logger:Logger_default, rs, category, message, &valist);
	va_end(valist);
}

void Test_end() {
	Test* test = Test__current;
	Test__current = test->parent;
	if (!test) { log_exit(ExitCode_critical, "Test.end", "Not matched Test.begin."); return; }
	word_t lv;
	if (test->failCount) {
		Test* p = test->parent;
		while (p) {
			p->failedChildCount++;
			p = p->parent;
		}
		word_t lv = LogLevel_Exception | LogLevel_SectionEnd;
		Logger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)",test->assertCount,test->failCount,test->childCount,test->failedChildCount);
	}
	else if (test->failedChildCount) {
		if (test->failCount) {
			Test* p = test->parent;
			while (p) {
				p->failedChildCount++;
				p = p->parent;
			}
			word_t lv = LogLevel_Warn | LogLevel_SectionEnd;
			Logger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
		}
	}
	else {
		word_t lv = LogLevel_Success | LogLevel_SectionEnd;
		Logger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
	}
	m_free(test->category);
	m_free(test);
}

