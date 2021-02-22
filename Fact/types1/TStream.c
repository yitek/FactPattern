#include "TStream.h"
#include <stdio.h>
InStreamMeta TInStream__meta__;
TOutStreamMeta TOutStream__meta__;
TStreamMeta TStream__meta__;
TStream TStream__defaultInstance;
TStream* TStream_default=0;

usize_t TOutStream_writeByte(TOutStream* self, byte_t byte) {
	putchar(byte);
	return 1;
}
usize_t TOutStream_write(TOutStream* self, byte_t* buffer, usize_t bufferSize) {
	while (bufferSize) {
		putchar(*buffer);
		buffer++;
	}
	return bufferSize;
}

usize_t TOutStream_writeWord(TOutStream* self, word_t value) {
	printf_s("%d",value);
	return sizeof(word_t);
}

usize_t TOutStream_writeStr(TOutStream* self, const char* str) {
	printf_s("%s", str);
	return m_strlen(str);
}

byte_t TInStream_readByte(TInStream* self) {
	if (self->hasCache) {
		self->hasCache = 0;
		return self->lastByte;
	}
	return self->lastByte = getchar();
}
inline word_t TInStream_readWord10(TInStream* self) {
	
	word_t value = 0;
	bool_t minus = 0;
	usize_t c = 0;
	for (;;c++) {
		char ch = TInStream_readByte(self);
		if (ch == '-') {
			if (c == 0) { minus = 1; continue; }
			else {
				TInStream_pushback(self);
				break;
			}
		}
		else if (ch >= '0' && ch <= '9') {
			value *= 10;
			value += ch - '0';
		}
		else {
			TInStream_pushback(self);
			break;
		}
		
	}
	return value;
}
word_t TInStream_readWord(TInStream* self) {
	return TInStream_readWord10(self);
}
usize_t TInStream_read(TInStream* self, byte_t* buffer, usize_t bufferSize) {
	while (bufferSize) {
		*buffer = getchar();
		buffer++;
	}
	return bufferSize;
}
usize_t TInStream_readStr(TInStream* self, byte_t* buffer, usize_t bufferSize) {
	usize_t c = 0;
	while (1) {
		byte_t ch = getchar();
		*buffer++ = ch;
		c++;
		if (ch=='\n' || c==bufferSize) break;
	}
	*buffer = 0;
	return c;
}


