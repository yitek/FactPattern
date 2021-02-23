#include "runtime.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#ifdef __WIN__
#include <windows.h>
#endif

void out(byte_t c) {
	putchar(c);
}
void outs(const byte_t* str) {
	if (!str)return;
	printf_s("%s", str);
}
void outln(const byte_t* str) {
	if (!str) out('\n');
	else printf_s("%s\n",str);
}

void outx(addr_t n, usize_t width) {
	usize_t factor = 1;
	usize_t len = 0;
	uword_t x = n;
	do {
		len++;
		x = x / 16;
		factor *= 16;
	} while (x);
	factor /= 16;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((word_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (usize_t i = 0, j = width - len; i < j; i++) factor *= 16;
		len = 0;
	}
	while (width) {
		uword_t t = n / factor;
		if (t >= 10)out('A' + t-10);
		else out('0' + t);
		--width; n = n % factor; factor = factor / 16;
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}
}

void outu(uword_t n, usize_t width) {
	usize_t factor = 1;
	usize_t len = 0;
	uword_t x = n;
	do {
		len++;
		x = x / 10;
		factor *= 10;
	} while (x);
	factor /= 10;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((word_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (usize_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		uword_t t = n / factor;
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}
}
void outd(word_t n, usize_t width) {
	bool_t minus = n < 0;
	
	usize_t factor = 1;
	usize_t len = 0;
	if (minus) { n = -n; len += 1; }
	uword_t x = n;
	do {
		len++;
		x = x / 10;
		factor *= 10;
	} while (x);
	factor /= 10;
	if (width == -1) width = len;	
	//负号必须输出
	if (minus) {
		out('-');
		if (--((word_t)width) < 0) width = 0;
		if (--((word_t)len) < 0) len = 1;
	}
	if (width < len) {
		width -= 3; if ((word_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (usize_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		uword_t t = n / factor;
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;
		
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}

}

void outb(uword_t n, usize_t width) {
	usize_t factor = 1;
	usize_t len = 0;
	uword_t x = n;
	do {
		len++;
		x = x / 2;
		factor *= 2;
	} while (x);
	factor /= 2;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((word_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (usize_t i = 0, j = width - len; i < j; i++) factor *= 2;
		len = 0;
	}
	while (width) {
		uword_t t = n / factor;
		out(t?'1':'0');
		--width; n = n % factor; factor = factor / 2;
	}
	if (len) {
		out('_');
		outx(len, 2);
	}
}

void outf(double n, usize_t i, usize_t f) {
	printf_s("%g",n);
}

void outt(const byte_t* fmt) {
	time_t  t;
	time(&t);
	if (fmt) {}
	else {
		char tbuffer[32];
		ctime_s(tbuffer, 32, &t);
		char* p = tbuffer;
		while (*p != '\n') { out(*p); p++; }
	}
	
}

typedef enum {
	OutFormatState_normal =0,
	OutFormatState_format = 1,
	OutFormatState_number1 = 1<<1 | OutFormatState_format,
	OutFormatState_number2 = 1 << 2 | OutFormatState_number1 | OutFormatState_format,
} OutFormatStates;



void outs_format(const byte_t* p,bool_t ignoreEndRet,void* args) {
	OutFormatStates state = OutFormatState_normal;
	byte_t cmd=0;
	int n1 = 0; int n2 = 0; bool_t isWidth = 0;
	byte_t ch = *p;
	bool_t convert = 0;
	usize_t retCount=0;
	while (1) {
		byte_t ch = *p; p++;
		bool_t isEnd = ch==0;
		
		bool_t replace = 0;
		if (state & OutFormatState_format) {
			if (ch == '%') { 
				if (*(p - 2) == '%') {
					cmd = ch; ch = 0;
				}
				replace = 1;
			}
			else if (ch >= '0' && ch <= '9') {
				if (cmd == 'd' || cmd == 'f' || cmd == 'u' || cmd=='x' || cmd=='b') {
					
					if ((state & OutFormatState_number2) > OutFormatState_number1) {
						
						n2 = n2 * 10 + ch - '0';
					}
					else if ((state & OutFormatState_number1)> OutFormatState_format) {
						n1 = n1 * 10 + ch - '0';
					}
					else {
						state |= OutFormatState_number1;
						n1 = n1 * 10 + ch - '0';
					}
					ch = 0;
				}
				else {
					replace = 1;
				}
			}
			else if (ch == '.' && (state & OutFormatState_number1)> OutFormatState_format) {
				state |= OutFormatState_number2;
				ch = 0;
				//state -= OutFormatState_number1;
			}
			else if (ch == 'l' && !isWidth) {
				isWidth = 1;
				ch = 0;
			}
			else if ((ch == 'd' || ch == 'u' || ch == 'p' || ch == 'c' || ch == 'x' || ch == 'b' || ch == 's' || ch == 'f' || ch=='t') & !cmd) {
				cmd = ch;
				ch = 0;
			}
			else {
				replace = 1;
			}
		}
		if (replace) {
			if (cmd == '%') { out(cmd);  }
			else if (cmd == 'c') {
				if (isWidth) {
					printf_s("%lc", va_arg((*(va_list*)args), wchar_t));
				}
				else out(va_arg((*(va_list*)args), char));
			}
			else if (cmd == 'd') {
				outd(va_arg((*(va_list*)args), long), n1 ? n1 : -1);
			}
			else if (cmd == 'u') {
				outu(va_arg((*(va_list*)args), long), n1 ? n1 : -1);
			}
			else if(cmd == 'x') {
				outx((uword_t)va_arg((*(va_list*)args), uword_t), n1 ? n1 : -1);
			}
			else if (cmd == 'p') {
				outx((addr_t)va_arg((*(va_list*)args), void*),sizeof(addr_t)*2);
			}
			else if (cmd == 's') {
				if (isWidth) printf_s("%ls", va_arg((*(va_list*)args), wchar_t*));
				else outs(va_arg((*(va_list*)args), char*));
			}
			else if (cmd == 'f') {
				if ((state & OutFormatState_number2) <= OutFormatState_format) {
					n2 = -1;
				}
				if ((state & OutFormatState_number1) <= OutFormatState_format) {
					n1 = -1;
				}
				outf(va_arg((*(va_list*)args), double),n1,n2);
			}
			else if (cmd == 'b') {
				outb(va_arg((*(va_list*)args), uword_t), n1 ? n1 : -1);
			}
			else if (cmd == 't') {
				outt(0);
			} else {
				out('%'); if (isWidth) out('l');
				out(cmd);
				if ((state & OutFormatState_number1)> OutFormatState_format || (state & OutFormatState_number2)> OutFormatState_format) {
					out(ch);
				}
			}
			state = OutFormatState_normal;
			replace = 0;
			cmd = 0;
			isWidth = 0;
			n1 = n2 = 0;
		}
		if (ch) {
			if (ch == '%') state = OutFormatState_format;
			else {
				if (ignoreEndRet) {
					if (ch == '\n') { retCount++; continue; }
					else if (!isEnd) {
						while (retCount) {
							out('\n'); retCount--;
						}
					}
				}
				
				if (convert) {
					if (ch == '0') out(0);
					else if (ch == 'n') out('\n');
					else if (ch == '\\') out('\\');
					else if (ch == 'r') out('\r');
					else if (ch == 't') out('\t');
					else { out('\\'); out(ch); }
					convert = 0;
				}
				else out(ch);
				//if (ch == '\\') convert = 1;
			}
		}
		if (isEnd)break;
	}
}

void outs_fmt(const byte_t* str,...) {
	va_list valist;
	va_start(valist, str);
	outs_format(str,0,&valist);
	va_end(valist);
	
}


#if defined(__WIN__)
void outc(OutColors color, byte_t ch) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	out(ch);
	SetConsoleTextAttribute(handle, 7);//设置颜色
}
void outcs(OutColors color, const byte_t* str) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	outs(str);
	SetConsoleTextAttribute(handle,7);//设置颜色
}
void outcln(OutColors color, const byte_t* str) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	outs(str); 
	SetConsoleTextAttribute(handle, 7);//设置颜色
	out('\n');
}

void outcs_format(OutColors color,const byte_t* str, bool_t ignoreEndRet, void* args) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	outs_format(str,ignoreEndRet,args);
	SetConsoleTextAttribute(handle, 7);//设置颜色
}
void outcs_fmt(OutColors color, const byte_t* str, ...) {
	va_list valist;
	va_start(valist, str);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	outs_format(str,0,&valist);
	SetConsoleTextAttribute(handle, 7);//设置颜色
	va_end(valist);
}
void outcs_fmtln(OutColors color, const byte_t* str, ...) {
	va_list valist;
	va_start(valist, str);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, color);//设置颜色
	outs_format(str, 1, &valist);
	SetConsoleTextAttribute(handle, 7);//设置颜色
	va_end(valist);
	out('\n');
}
#endif


TMemoryMeta TMemory__meta__ = {
	.offset = 0,
	.get_type = 0,
	.alloc = TMemory_alloc,
	.alloc1 = TMemory_alloc1,
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

void* TMemory_alloc1(TMemory* self, usize_t size, void* mInitArgs, MemoryKinds mkind) {
	void* p = malloc(size);
	if (p) {
		return p;
	}
	else {
		log_exit(ExitCode_memory, "TMemory.alloc1", "Cannot allocate memory!");
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




void m_look(const unsigned char* str, usize_t length, MLookTake take) {
	usize_t block;
	bool_t useTake = length == -1;
	if (length==-1) {
		block = take(*str);
	}
	else {
		block = (usize_t)take;
	}
	while (length) {
		outs_fmt("[%p]:", str);
		for (usize_t i = 0; i < block; i++) {
			byte_t b = *(str + i);
			outx(b, 2); out(' ');
		}
		out('\t');
		for (usize_t i = 0; i < block; i++) {
			byte_t b = *(str + i);
			outb(b, 8); out(' ');
		}
		out('\t');
		for (usize_t i = 0; i < block; i++) {
			byte_t b = *(str + i);
			if (b == '\n') { out('\\'); out('n'); }
			else if (b == '\t') { out('\\'); out('t'); }
			else if (b == '\r') { out('\\'); out('r'); }
			else if (b == '\b') { out('\\'); out('b'); }
			else out(b);
		}
		out('\n');
		str += block;
		if (useTake) {
			if (!(block = take(*str)))break;
		}
		else length--;
	}
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


inline static uword_t getLevelColor(LogLevels level) {
	uword_t color = 7;

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
	return color;
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
	if (lv & LogLevel_SectionBegin) out('\n');
	outcs_fmt(getLevelColor(LogLevel_Message), "%t");
	out(' ');


	if (lv & LogLevel_SectionBegin) {
		TLogger__tabs++;
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
	}
	else if (lv & LogLevel_SectionEnd) {
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
		if (TLogger__tabs == 0) log_exit(ExitCode_critical, "TLogger._output", "Not matched SectionBegin.");
		TLogger__tabs--;

	}
	else {
		for (usize_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
	}
	uword_t color = getLevelColor(lv);
	//灰底白字
	//outcs_fmt(8<<4 | 15,"[%s]",category?category:"LOG");
	outcs_fmt(color, "[%s]: ", category ? category : "LOG");
	//out(' ');
	outcs_format(color,message,1,args);
	out('\n');
	if (lv & LogLevel_SectionEnd) out('\n');
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

void m_printb(const unsigned char* str, usize_t groupc, usize_t length) {
	usize_t g = groupc;
	for (usize_t i = 0; i < length; i++) {
		utiny_t b = str[i];
		if (g == groupc) {}
		for (usize_t j = 0; j < sizeof(utiny_t); j++) {
			utiny_t mask = 1 << j;
			if (b & mask) out('1'); else out('0');
		}
		g--;
		if (g == 0) { out('\n'); g = groupc; }
	}
	out('\n');
}
