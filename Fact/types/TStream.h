/*****************************************************
*
* author:yiy
*
* description: 默认日志类
*
******************************************************/

#pragma once

#include "../layout.h"
#ifndef __TSTREAM_INCLUDED__ 
#define __TSTREAM_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct stOutStream {
		struct stTObject;
	} TOutStream;

	typedef struct stOutStreamMeta {
		struct stObjectMetaLayout;
		usize_t (*writeByte)(TOutStream* self, byte_t byte);
		usize_t (*write)(TOutStream* self, byte_t* buffer, usize_t bufferSize);
		usize_t (*writeWord)(TOutStream* self, word_t byte);
		usize_t (*writeStr)(TOutStream* self, const char* str);
		
	}TOutStreamMeta;


	typedef struct stInStream{
		struct stTObject;
		byte_t lastByte;
		bool_t hasCache;

	} TInStream;

	typedef struct stInStreamMeta {
		struct stObjectMetaLayout;
		byte_t (*readByte)(TInStream* self);
		usize_t (*read)(TInStream* self, byte_t* buffer, usize_t bufferSize);
		word_t (*readWord)(TInStream* self);
		usize_t (*readStr)(TInStream* self, char* buffer, usize_t bufferSize);
	}TInStreamMeta;

	

	
	

	typedef struct stStream {
		struct stOutStream;
	} TStream;

	typedef struct stStreamMeta {
		struct stOutStreamMeta out;
		struct stInStreamMeta in;
	}TStreamMeta;

	extern TInStreamMeta TInStream__meta__;
	extern TOutStreamMeta TOutStream__meta__;
	extern TStreamMeta TStream__meta__;
	extern TStream TStream__defaultInstance;
	extern TStream* TStream_default;
	
	usize_t TOutStream_writeByte(TOutStream* self, byte_t byte);
	usize_t TOutStream_write(TOutStream* self, byte_t* buffer, usize_t bufferSize);
	usize_t TOutStream_writeWord(TOutStream* self,word_t value);
	usize_t TOutStream_writeStr(TOutStream* self, const char* str);

	byte_t TInStream_readByte(TInStream* self);
	word_t TInStream_readWord(TInStream* self);
	usize_t TInStream_readStr(TInStream* self,char* buffer, usize_t bufferSize);
	usize_t TInStream_read(TInStream* self, byte_t* buffer, usize_t bufferSize);
	static inline usize_t TInStream_pushback(TInStream* self) {
		if (self->hasCache) return 0;
		self->hasCache = 1;
		return 1;
	}
	
	static inline byte_t TStream_readByte(TStream* self) { return TInStream_readByte((TInStream*)self); }
	static inline usize_t TStream_read(TStream* self, byte_t* buffer, usize_t bufferSize) { return TInStream_read((TInStream*)self,buffer,bufferSize); }
	static inline word_t TStream_readWord(TStream* self) { return TInStream_readWord((TInStream*)self); }
	static inline usize_t TStream_readStr(TInStream* self, char* buffer, usize_t bufferSize) {return TInStream_readStr(self,buffer,bufferSize);}

	static inline TStream* TStream_writeByte(TStream* self, byte_t byte) { return (TStream*)TOutStream_writeByte((TOutStream*)self,byte); }
	static inline usize_t TStream_write(TStream* self, byte_t* buffer, usize_t bufferSize) { return TOutStream_write((TOutStream*)self, buffer,bufferSize); }
	static inline usize_t TStream_writeWord(TOutStream* self, word_t value) {return TOutStream_writeWord(self, value); }
	static inline usize_t TStream_writeStr(TOutStream* self, const char* str) { return TOutStream_writeStr((TOutStream*)self, str); }

#ifdef __cplusplus 
} //extern "C" 
#endif

#endif // end ifndef __TSTREAM_INCLUDED__
