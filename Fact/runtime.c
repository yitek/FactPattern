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

void outx(uint_t n, favor_t width) {
	uint_t factor = 1;
	favor_t len = 0;
	uint_t x = n;
	do {
		len++;
		x = x / 16;
		factor *= 16;
	} while (x);
	factor /= 16;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((favor_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 16;
		len = 0;
	}
	while (width) {
		uint_t t = n / (factor?factor:1);
		if (t >= 10)out('A' + t-10);
		else out('0' + t);
		--width; n = n % factor; factor = factor / 16;
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}
}

void outlx(ulong_t n, int width) {
	ulong_t factor = 1;
	favor_t len = 0;
	ulong_t x = n;
	do {
		len++;
		x = x / 16;
		factor *= 16;
	} while (x);
	factor /= 16;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((favor_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 16;
		len = 0;
	}
	while (width) {
		byte_t t =(byte_t)( n / (factor ? factor : 1));
		if (t >= 10)out('A' + t - 10);
		else out('0' + t);
		--width; n = n % factor; factor = factor / 16;
	}
	if (len) {

		out('_');
		outx(len, 2);
	}
}

void outu(uint_t n, favor_t width) {
	uint_t factor = 1;
	favor_t len = 0;
	uint_t x = n;
	do {
		len++;
		x = x / 10;
		factor *= 10;
	} while (x);
	factor /= 10;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((favor_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		uint_t t = n / (factor?factor:1);
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}
}

void outlu(ulong_t n, favor_t width) {
	ulong_t factor = 1;
	favor_t len = 0;
	ulong_t x = n;
	do {
		len++;
		x = x / 10;
		factor *= 10;
	} while (x);
	factor /= 10;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if ((favor_t)width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		byte_t t = (byte_t)(n / (factor ? factor : 1));
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;
	}
	if (len) {

		out('_');
		outx(len, 2);
	}
}

void outd(int_t n, favor_t width) {
	bool_t minus = n < 0;
	
	int_t factor = 1;
	favor_t len = 0;
	if (minus) { n = -n; len += 1; }
	int_t x = n;
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
		if (--(width) < 0) width = 0;
		if (--len < 0) len = 1;
	}
	if (width < len) {
		width -= 3; if (width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		int_t t = n / (factor?factor:1);
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;
		
	}
	if (len) {
		
		out('_');
		outx(len, 2);
	}

}

void outld(long_t n, favor_t width) {
	bool_t minus = n < 0;

	long_t factor = 1;
	favor_t len = 0;
	if (minus) { n = -n; len += 1; }
	long_t x = n;
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
		if (--(width) < 0) width = 0;
		if (--len < 0) len = 1;
	}
	if (width < len) {
		width -= 3; if (width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 10;
		len = 0;
	}
	while (width) {
		byte_t t = (byte_t)(n / (factor ? factor : 1));
		out('0' + t);
		--width; n = n % factor; factor = factor / 10;

	}
	if (len) {

		out('_');
		outx(len, 2);
	}

}

void outb(uint_t n, favor_t width) {
	uint_t factor = 1;
	favor_t len = 0;
	uint_t x = n;
	do {
		len++;
		x = x / 2;
		factor *= 2;
	} while (x);
	factor /= 2;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if (width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 2;
		len = 0;
	}
	while (width) {
		uint_t t = n / (factor?factor:1);
		out(t?'1':'0');
		--width; n = n % factor; factor = factor / 2;
	}
	if (len) {
		out('_');
		outx(len, 2);
	}
}

void outlb(ulong_t n, favor_t width) {
	ulong_t factor = 1;
	favor_t len = 0;
	ulong_t x = n;
	do {
		len++;
		x = x / 2;
		factor *= 2;
	} while (x);
	factor /= 2;
	if (width == -1) width = len;
	if (width < len) {
		width -= 3; if (width < 0)width = 0;
		len -= width;
	}
	else {
		for (favor_t i = 0, j = width - len; i < j; i++) factor *= 2;
		len = 0;
	}
	while (width) {
		ulong_t t = n / (factor?factor:1);
		out(t ? '1' : '0');
		--width; n = n % factor; factor = factor / 2;
	}
	if (len) {
		out('_');
		outx(len, 2);
	}
}

void outf(double n, favor_t i, favor_t f) {
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
	favor_t retCount=0;
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
					printf_s("%lc", va_arg((*(va_list*)args), int));
				}
				else out(va_arg((*(va_list*)args), int));
			}
			else if (cmd == 'd') {
				if(isWidth)outld(va_arg((*(va_list*)args), long), n1 ? n1 : -1);
				else outd(va_arg((*(va_list*)args), int), n1 ? n1 : -1);
			}
			else if (cmd == 'u') {
				if (isWidth)outlu(va_arg((*(va_list*)args), unsigned long), n1 ? n1 : -1);
				else outu(va_arg((*(va_list*)args), unsigned int), n1 ? n1 : -1);
			}
			else if(cmd == 'x') {
				if (isWidth)outlx(va_arg((*(va_list*)args), unsigned long), n1 ? n1 : -1);
				else outx(va_arg((*(va_list*)args), int), n1 ? n1 : -1);
			}
			else if (cmd == 'p') {
				out_p(va_arg((*(va_list*)args), void*),sizeof(addr_t)*2);
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
				if (isWidth)outlb(va_arg((*(va_list*)args), unsigned long), n1 ? n1 : -1);
				outb(va_arg((*(va_list*)args), unsigned int), n1 ? n1 : -1);
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

void outs_fmt(const byte_t* str, ...) {
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





void m_look(const void* str, usize_t length, MLookTake take) {
	usize_t block;
	bool_t useTake = length == -1;
	if (length==-1) {
		block = take(*(utiny_t*)str);
	}
	else {
		block = (usize_t)take;
	}
	while (length) {
		outs_fmt("[%p]:", str);
		for (usize_t i = 0; i < block; i++) {
			unsigned char b = *((utiny_t*)str + i);
			outx(b, 2); out(' ');
		}
		out('\t');
		for (usize_t i = 0; i < block; i++) {
			unsigned char b = *((utiny_t*)str + i);
			outb(b, 8); out(' ');
		}
		out('\t');
		for (usize_t i = 0; i < block; i++) {
			unsigned char b = *((utiny_t*)str + i);
			if (b == '\n') { out('\\'); out('n'); }
			else if (b == '\t') { out('\\'); out('t'); }
			else if (b == '\r') { out('\\'); out('r'); }
			else if (b == '\b') { out('\\'); out('b'); }
			else out(b);
		}
		out('\n');
		str =(utiny_t*)str+ block;
		if (useTake) {
			if (!(block = take(*((utiny_t*)str))))break;
		}
		else length--;
	}
}

//////////////////////////////////////////////
// Object,Type,String Meta define

const struct stTObjectMeta TObject__metaInstance = {
	.offset = 0,
	.__toString__ = TObject__toString__,
	.__gettype__ = TObject__gettype__,
	.__compare__ = TObject__compare__,
	.__endle__ = 0
};

const struct stIIteratorMeta IIterator__metaInstance = {
   .__toString__ = KeyValuePair__toString__,
   .__gettype__ = KeyValuePair__gettype__,
   .__compare__ = TObject__compare__,
   .__reset__ = 0,
   .__current__ = 0,
   .__next__ = 0,
   .__endle__ = 0
};

const struct stTObjectMeta KeyValuePair__metaInstance = {
   .__toString__ = KeyValuePair__toString__,
   .__gettype__ = KeyValuePair__gettype__,
   .__compare__ = TObject__compare__,
   .__endle__ = 0
};

const struct stISetMeta ISet__metaInstance = {
   .__toString__ = TObject__toString__,
   .__gettype__ = ISet__gettype__,
   .__compare__ = TObject__compare__,
   .__iterator__ = 0
};
const struct stTObjectMeta TType__metaInstance = {
	.offset = 0,
	.__toString__ = TObject__toString__,
	.__gettype__ = TType__gettype__,
	.__compare__ = TObject__compare__,
	.__endle__ = 0
};

const struct stTObjectMeta TString__metaInstance = {
	.offset = 0,
	.__toString__ = TString__toString__,
	.__gettype__ = TType__gettype__,
	.__compare__ = TObject__compare__,
	.__endle__ = 0
};

/////////////////////////////////
// Object Type define

struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[6 + sizeof(unichar_t)];
} TObject__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 6,
	.inst.length = 6,
	.strDatas = "Object"
};

struct {
	struct stMRefUnit;
	struct stTType inst;
} TObject__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.vftptr = (struct stVFTL*)&TObject__metaInstance,
	.inst.name = &TObject__typenameInstance.inst,
	.inst.size = sizeof(struct stTType),
	.inst.base = 0,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0,
	.inst.genericArguments = 0,
	.inst.extras = 0
};
const struct stTType*const const TObject__type__ = &TObject__typeInstance.inst;



///////////////////
// IIterator() 
struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[11 + sizeof(unichar_t)];
} IIterator__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 11,
	.inst.length = 11,
	.strDatas = "IIterator()"
};


struct {
	struct stMRefUnit;
	struct stTType inst;
} IIterator__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_struct,
	.inst.genericRestricts = (struct stTArray*)&KeyValuePair_genericRestrictsInstance,
	.inst.genericArguments = 0,
	.inst.vftptr = (struct stVFTL*)&IIterator__metaInstance,
	.inst.name = &IIterator__typenameInstance.inst,
	.inst.size = 0,
	.inst.base = 0,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0
};
const struct stTType* const const IIterator__type__ = &IIterator__typeInstance.inst;


///////////////////
// KeyValuePair(,) 
struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[15 + sizeof(unichar_t)];
} KeyValuePair__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 15,
	.inst.length = 15,
	.strDatas = "KeyValuePair(,)"
};

const struct stTString* KeyValuePair__toString__(struct stTObject* self) { return &KeyValuePair__typenameInstance.inst; }




const KeyValuePairRestricts KeyValuePair_genericRestrictsInstance = {
	.length = 2,
	.keyRestrict.defaultType = 0,
	.keyRestrict.restrictType = 0,
	.valueRestrict.defaultType = 0,
	.valueRestrict.defaultType = 0
};


struct {
	struct stMRefUnit;
	struct stTType inst;
} KeyValuePair__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_struct,
	.inst.genericRestricts = (struct stTArray*)&KeyValuePair_genericRestrictsInstance,
	.inst.genericArguments = 0,
	.inst.vftptr = (struct stVFTL*)&KeyValuePair__metaInstance,
	.inst.name = &KeyValuePair__typenameInstance.inst,
	.inst.size = 0,
	.inst.base = 0,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0
};
const struct stTType* const const KeyValuePair__type__ = &KeyValuePair__typeInstance.inst;


struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[4 + sizeof(unichar_t)];
} ISet__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 4,
	.inst.length = 4,
	.strDatas = "ISet"
};




struct {
	struct stMRefUnit;
	struct stTType inst;
} ISet__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.genericRestricts = (struct stTArray*)&KeyValuePair_genericRestrictsInstance,
	.inst.genericArguments = 0,
	.inst.vftptr = (struct stVFTL*)&ISet__metaInstance,
	.inst.name = &ISet__typenameInstance.inst,
	.inst.size = sizeof(struct stTType),
	.inst.base = 0,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0
};
const struct stTType* const const ISet__type__ = &ISet__typeInstance.inst;

/////////////////////////////////
// Type's Type define

struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[4 + sizeof(unichar_t)];
} TType__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 4,
	.inst.length = 4,
	.strDatas = "Type"
};

struct {
	struct stMRefUnit;
	struct stTType inst;
} TType__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.vftptr = (struct stVFTL*)&TType__metaInstance,
	.inst.name = &TType__typenameInstance.inst,
	.inst.size = sizeof(struct stTType),
	.inst.base = &TObject__typeInstance.inst,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0,
	.inst.genericArguments = 0,
	.inst.extras = 0
};
const struct stTType*const TType__type__ = &TType__typeInstance.inst;

/////////////////////////////////
// String Type define

struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[6 + sizeof(unichar_t)];
} TString__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 6,
	.inst.length = 6,
	.strDatas = "String"
};

struct {
	struct stMRefUnit;
	struct stTType inst;
} TString__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.name = &TType__typenameInstance.inst,
	.inst.size = sizeof(struct stTString),
	.inst.vftptr = &TString__metaInstance,
	.inst.base = &TObject__typeInstance.inst,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0,
	.inst.genericArguments = 0,
	.inst.extras = 0
};
const struct stTType*const  TString__type__ = &TString__typeInstance.inst;


/////////////////////////////////////
/// TMemory


const struct stTType* TMemory__gettype__(struct stTObject* self) {return TMemory__type__;}

struct {
	struct stTMemoryMeta;
	void(*__endle__)();
} TMemory__metaInstance = {
	.offset = 0,
	.__gettype__ = 0,
	.__compare__ = TObject__compare__,
	.__toString__ = TObject__toString__,
	.__gettype__ = TMemory__gettype__,
	.alloc = TMemory_alloc,
	.alloc1 = TMemory_alloc1,
	.free = TMemory_free,
	.__destruct__ = 0,
	.__endle__ = 0
};

struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[9+ sizeof(unichar_t)];
} TMemory__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 9,
	.inst.length =9,
	.strDatas = "SysMemory"
};

struct {
	struct stMRefUnit;
	struct stTType inst;
} TMemory__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.name = &TMemory__typenameInstance.inst,
	.inst.size = sizeof(struct stTMemory),
	.inst.vftptr = (struct stVFTL*)&TMemory__metaInstance,
	.inst.base = &TObject__typeInstance.inst,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0,
	.inst.genericArguments = 0,
	.inst.extras = 0
};

const struct stTType* const TMemory__type__ = &TMemory__typeInstance.inst;

struct {
	struct stMRefUnit;
	struct stTMemory inst;
}TMemory__defaultInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TMemory__metaInstance
};
TMemory* TMemory_default = &TMemory__defaultInstance.inst;

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
		*((ClazzMeta**)&self->__meta__) = (ClazzMeta*)&TMemory__metaInstance;
	}
	
	return self;
}

void TMemory__destruct__(TMemory* self, bool_t existed) {
	if (self == &TMemory__defaultInstance.inst) {
		TLogger_notice(0, "TMemory.__destruct__", "The prime TMemory instance should not be destructed.");
		return;
	}

	if (!existed) free(self);
}


/////////////////////////////////////
/// TLogger


const struct stTType* TLogger__gettype__(struct stTObject* self) { return TLogger__type__; }

struct {
	struct stTLoggerMeta;
	void(*__endle__)();
} TLogger__metaInstance = {
	.offset = 0,
	.__gettype__ = 0,
	.__compare__ = TObject__compare__,
	.__toString__ = TObject__toString__,
	.__gettype__ = TMemory__gettype__,
	.output = TLogger__output,
	.__endle__ = 0
};

struct {
	struct stMRefUnit;
	struct stTString inst;
	byte_t strDatas[6 + sizeof(unichar_t)];
} TLogger__typenameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TString__metaInstance,
	.inst.bytes = 6,
	.inst.length = 6,
	.strDatas = "Logger"
};

struct {
	struct stMRefUnit;
	struct stTType inst;
} TLogger__typeInstance = {
	.inst.__meta__ = (ClazzMeta*)&TType__metaInstance,
	.inst.kind = TypeKind_class,
	.inst.name = &TLogger__typenameInstance.inst,
	.inst.size = sizeof(struct stTLogger),
	.inst.vftptr = (struct stVFTL*)&TLogger__metaInstance,
	.inst.base = &TObject__typeInstance.inst,
	.inst.mixins = 0,
	.inst.interfaces = 0,
	.inst.genericArguments = 0,
	.inst.methods = 0,
	.inst.fields = 0,
	.inst.properties = 0,
	.inst.extras = 0,
	.inst.genericArguments = 0,
	.inst.extras = 0
};

const struct stTType* const TLogger__type__ = &TLogger__typeInstance.inst;

struct {
	struct stMRefUnit;
	struct stTLogger inst;
}TLogger__defaultInstance = {
	.__ref__ = 0,
	.inst.__meta__ = (ClazzMeta*)&TLogger__metaInstance
};
TLogger* TLogger_default = &TLogger__defaultInstance.inst;

inline static OutColors getLevelColor(LogLevels level) {
	favor_t color = 7;

	if (level & LogLevel_sectionBegin) color = 15 << 4 | 0; // 黄底蓝字
	if (level & LogLevel_sectionEnd) color = 8; // 黄底蓝字
	if (level & LogLevel_error) color = 4 << 4 | 15; // 浅红色
	if (level & LogLevel_exception) color = 12; // 浅红色
	if (level & LogLevel_warn)color = 6; //黄色
	if (level & LogLevel_notice)color = 1; //蓝色
	if (level & LogLevel_success) color = 2; //绿色
	if (level & LogLevel_info) color = 15; //白色
	if (level & LogLevel_message) color = 8; //灰色 7 = 白色
	if (level & LogLevel_trace) color = 5; //紫色

	if (level & LogLevel_sectionEnd) color = 7 << 4 | color;
	return color;
}



favor_t log_exit(favor_t code, const byte_t* category, const byte_t* message, ...) {
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(TLogger_default, LogLevel_error, category, message, &valist);
	va_end(valist);
	exit(code);
	return 0;
}
ufavor_t TLogger__tabs = 0;
void TLogger__output(struct stTLogger* self, LogLevels lv, const byte_t* category, const byte_t* message, void* args) {
	if (lv < self->level) return;
	if (lv & LogLevel_sectionBegin) out('\n');
	outcs_fmt(getLevelColor(LogLevel_message), "%t");
	out(' ');


	if (lv & LogLevel_sectionBegin) {
		TLogger__tabs++;
		for (ufavor_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
	}
	else if (lv & LogLevel_sectionEnd) {
		for (ufavor_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
		if (TLogger__tabs == 0) log_exit(ExitCode_critical, "TLogger._output", "Not matched SectionBegin.");
		TLogger__tabs--;

	}
	else {
		for (ufavor_t i = 0, j = TLogger__tabs - 1; i < j; i++) out('\t');
	}
	OutColors color = getLevelColor(lv);
	//灰底白字
	//outcs_fmt(8<<4 | 15,"[%s]",category?category:"LOG");
	outcs_fmt(color, "[%s]: ", category ? category : "LOG");
	//out(' ');
	outcs_format(color,message,1,args);
	out('\n');
	if (lv & LogLevel_sectionEnd) out('\n');
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
	TLogger_output__virtual__(self, LogLevel_trace, category, message, &valist);
	va_end(valist);
}


void TLogger_message(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_message, category, message, &valist);
	va_end(valist);
}
void TLogger_info(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_info, category, message, &valist);
	va_end(valist);
}
void TLogger_success(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_success, category, message, &valist);
	va_end(valist);
}
void TLogger_notice(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_notice, category, message, &valist);
	va_end(valist);
}

void TLogger_warn(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_warn, category, message, &valist);
	va_end(valist);
}
void TLogger_exception(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_exception, category, message, &valist);
	va_end(valist);
}
void TLogger_error(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_error, category, message, &valist);
	va_end(valist);
}
void TLogger_sectionBegin(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(self, LogLevel_sectionBegin, category, message, &valist);
	va_end(valist);
}

void TLogger_sectionEnd(TLogger* self, const byte_t* category, const byte_t* message, ...) {
	if (!self) self = TLogger_default;
	va_list valist;
	va_start(valist, message);
	TLogger__output(self, LogLevel_sectionEnd, category, message, &valist);
	va_end(valist);
}



TLogger* TLogger__construct__(TLogger* self, LoggerOptions* opts) {
	if (!self) {
		self = ((TLogger*)TMemory_alloc(0,sizeof(TLogger),0, 0));
		if (!self) exit(0);
		*((ClazzMeta**)&self->__meta__) = (ClazzMeta*)&TLogger__metaInstance;
	}
	
	if (opts) {
		m_copy(&self->level, opts, sizeof(LoggerOptions));
	}
	else {
		self->level = LogLevel_none;
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
	Test* test = m_allocate(Test,0,0,0);
	test->category = m_cstr(category);
	if (Test__current) {
		test->parent = Test__current;
		test->parent->childCount++;
	}
	else test->parent = 0;
	test->childCount = test->failedChildCount = test->assertCount = test->failCount = 0;
	va_list valist;
	va_start(valist, message);
	TLogger_output__virtual__(Test__logger ? Test__logger : TLogger_default, LogLevel_sectionBegin, category, message, &valist);
	va_end(valist);
	Test__current = test;
}
void Test_assert(const byte_t* category, bool_t condition, const byte_t* message, ...) {
	Test* test = Test__current;
	LogLevels rs;
	if (condition) {
		rs = LogLevel_success;
		if (test) {
			test->assertCount++;
		}
	}
	else {
		rs = LogLevel_error;
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
		LogLevels lv = LogLevel_exception | LogLevel_sectionEnd;
		TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
	}
	else if (test->failedChildCount) {
		if (test->failCount) {
			Test* p = test->parent;
			while (p) {
				p->failedChildCount++;
				p = p->parent;
			}
			LogLevels lv = LogLevel_error | LogLevel_sectionEnd;
			TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
		}
		else {
			LogLevels lv = LogLevel_warn | LogLevel_sectionEnd;
			TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
		}
	}
	else {
		LogLevels lv = LogLevel_success | LogLevel_sectionEnd;
		TLogger_log(Test__logger, lv, test->category, "asserts: %d(fail:%d),children: %d(fail:%d)", test->assertCount, test->failCount, test->childCount, test->failedChildCount);
	}
	TMemory_free(0,(void*)test->category);
	TMemory_free(0,test);
}


