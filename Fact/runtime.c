#include "runtime.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#ifdef __WIN__
#include <windows.h>
#endif


TMemoryMeta TMemory__meta__ = {
	.offset = 0,
	.get_type = 0,
	.alloc = TMemory_alloc,
	.free = TMemory_free,
	.__destruct__ = 0,
};

struct stTMemoryLayout TMemory_instance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TMemory__meta__
};
TMemory* TMemory_default = (TMemory*)((byte_t*)(&TMemory_instance) + sizeof(MRefUnit));

void* TMemory_alloc(TMemory* self, usize_t size, void* mInitArgs, MemoryKinds mkind) {
	void* p = malloc(size);
	if (p) {
		return p;
	}
	else {
		log_exit(ExitCode_memory, "TMemory.alloc", "Cannot allocate memory!");
		exit(1);
	}
}
bool_t TMemory_free(TMemory* self, void* p) { free(p); return 1; }


TMemory* TMemory__construct__(TMemory* self) {
	if (!self) {
		self = (TMemory*)malloc(sizeof(TMemory));
		if (!self) {
			log_exit(ExitCode_memory, "TMemory.__construct__", "Cannot allocate memory!");
			exit(1);
		}
	}
	self->__meta__ = (ClazzMeta*)&TMemory__meta__;
	return self;
}

void TMemory__destruct__(TMemory* self,bool_t existed) {
	if (self == &TMemory_instance.inst ) {
		TLogger_notice(0,"TMemory.__destruct__","The prime TMemory instance should not be destructed.");
		return;
	}

	if (!existed) free(self);
}







LoggerMeta TLogger__meta__ = {
	.offset = 0,
	.output = TLogger__output
};

struct stLoggerLayout {
	struct stMRefUnit;
	struct stTLogger;
} TLogger_defaultInstance = {
	.__meta__ = (ClazzMeta*)&TLogger__meta__,
	.level = 0,
	.__ref__ = 0
};
TLogger* TLogger_default = (TLogger*)((byte_t*)&TLogger_defaultInstance + sizeof(MRefUnit));


void setLevelColor(LogLevels level);

#ifdef __WIN__
void setLevelColor(LogLevels level) {


	WORD color = 7;

	if (level & LogLevel_SectionBegin) color = 15 << 4 | 0; // 黄底蓝字
	if (level & LogLevel_SectionEnd) color = 8; // 黄底蓝字
	if (level & LogLevel_Error) color = 4 << 4 | 15; // 浅红色
	if (level & LogLevel_Exception) color = 12; // 浅红色
	if (level & LogLevel_Warn)color = 6; //黄色
	if (level & LogLevel_Notice)color = 1; //蓝色
	if (level & LogLevel_Success) color = 2; //绿色
	if (level & LogLevel_Info) color = 15; //白色
	if (level & LogLevel_Message) color = 8; //灰色 7 = 白色
	if (level & LogLevel_Trace) color = 5; //紫色

	if (level & LogLevel_SectionEnd) color = 7 << 4 | color;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色

}

#endif


void TLogger__printf(const byte_t* p, void* args) {
	bool_t hasPlaceholder = 0;
	byte_t prev = 0;
	byte_t ch = 0;
	while (*p) {
		ch = *p; p++;
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
	TLogger_output__virtual__(TLogger_default, LogLevel_Error, category, message, &valist);
	va_end(valist);
	exit(code);
	return 0;
}
usize_t TLogger__tabs = 0;
void TLogger__output(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
	if (lv < self->level) return;
	if (lv & LogLevel_SectionBegin) putchar('\n');
	setLevelColor(LogLevel_Message);
	printNow();
	putchar(' ');


	if (lv & LogLevel_SectionBegin) {
		TLogger__tabs++;
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) putchar('\t');
	}
	else if (lv & LogLevel_SectionEnd) {
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) putchar('\t');
		if (TLogger__tabs == 0) log_exit(ExitCode_critical, "TLogger._output", "Not matched SectionBegin.");
		TLogger__tabs--;

	}
	else {
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) putchar('\t');
	}
	setLevelColor(lv);
	putchar('[');
	if (category) printf_s("%s", category);
	else printf_s("LOG");
	putchar(']'); putchar(':'); putchar(' ');
	//
	if (args == 0) {
		printf_s("%s\n", message);
		return;
	}
	TLogger__printf(message, args);
	setLevelColor(0);
	putchar('\n');
	if (lv & LogLevel_SectionEnd) putchar('\n');
}

void TLogger_log(TLogger* self, LogLevels level, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, level, category, message, &valist);
	va_end(valist);
}
void TLogger_trace(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Trace, category, message, &valist);
	va_end(valist);
}


void TLogger_message(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Message, category, message, &valist);
	va_end(valist);
}
void TLogger_info(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Info, category, message, &valist);
	va_end(valist);
}
void TLogger_success(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Success, category, message, &valist);
	va_end(valist);
}
void TLogger_notice(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Notice, category, message, &valist);
	va_end(valist);
}

void TLogger_warn(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Warn, category, message, &valist);
	va_end(valist);
}
void TLogger_exception(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Exception, category, message, &valist);
	va_end(valist);
}
void TLogger_error(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_Error, category, message, &valist);
	va_end(valist);
}
void TLogger_sectionBegin(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_SectionBegin, category, message, &valist);
	va_end(valist);
}

void TLogger_sectionEnd(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger__output(self, LogLevel_SectionEnd, category, message, &valist);
	va_end(valist);
}



TLogger* TLogger__construct__(TLogger* self, LoggerOptions* opts) {
	if (!self) {
		self = ((TLogger*)TMemory_alloc(0,sizeof(TLogger),0, 0));
		if (!self) exit(0);
	}
	self->__meta__ = (ClazzMeta*)&TLogger__meta__;
	if (opts) {
		m_copy(&self->level, opts, sizeof(LoggerOptions));
	}
	else {
		self->level = LogLevel_None;
	}

	return self;
}

Test* Test__current = 0;
TLogger* Test__logger = 0;

void TLogger__destruct__(TLogger* self, bool_t existed) {
	Test* test = Test__current;
	while (test) {
		TMemory_free(0, (void*)test->category);
		Test* p = test->parent;
		TMemory_free(0, test);
		test = p;
	}
	Test__current = 0;
	if (!existed) {
		if (self != TLogger_default) {
			TMemory_free(0, self);
		}
	}
}



void Test_begin(const byte_t* category, const byte_t* message, ...) {
	Test* test = m_allocate(Test, 0,0,0);
	test->category = m_cstr(category);
	if (Test__current) {
		test->parent = Test__current;
		test->parent->childCount++;
	}
	else test->parent = 0;
	test->childCount = test->failedChildCount = test->assertCount = test->failCount = 0;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(Test__logger ? Test__logger : TLogger_default, LogLevel_SectionBegin, category, message, &valist);
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
	TLogger_output__virtual__(Test__logger ? Test__logger : TLogger_default, rs, category, message, &valist);
	va_end(valist);
}

void Test_end() {
	Test* test = Test__current;
	Test__current = test->parent;
	if (!test) { log_exit(ExitCode_critical, "Test.end", "Not matched Test.begin."); return; }
	if (test->failCount) {
		Test* p = test->parent;
		while (p) {
			p->failedChildCount++;
			p = p->parent;
		}
		word_t lv = LogLevel_Exception | LogLevel_SectionEnd;
		TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
	}
	else if (test->failedChildCount) {
		if (test->failCount) {
			Test* p = test->parent;
			while (p) {
				p->failedChildCount++;
				p = p->parent;
			}
			word_t lv = LogLevel_Warn | LogLevel_SectionEnd;
			TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
		}
	}
	else {
		word_t lv = LogLevel_Success | LogLevel_SectionEnd;
		TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
	}
	TMemory_free(0,(void*)test->category);
	TMemory_free(0,test);
}
