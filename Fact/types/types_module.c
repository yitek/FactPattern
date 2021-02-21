#include "types_module.h"

bool_t TStream_initialized = 0;
void TStream_module(void* memory, void* vm, void* param) {
	if (TStream_initialized) return;
	TInStream__meta__.offset = 0;
	TOutStream__meta__.write = TOutStream_write;
	TOutStream__meta__.writeByte = TOutStream_writeByte;
	TOutStream__meta__.writeWord = TOutStream_writeWord;
	TOutStream__meta__.writeStr = TOutStream_writeStr;

	TInStream__meta__.offset = 0;
	TInStream__meta__.read = TInStream_read;
	TInStream__meta__.readByte = TInStream_readByte;
	TInStream__meta__.readWord = TInStream_readWord;
	TInStream__meta__.readStr = TInStream_readStr;

	m_copy(&TStream__meta__.out, &TOutStream__meta__,sizeof(TOutStreamMeta));
	m_copy(&TStream__meta__.in, &TInStream__meta__, sizeof(TInStreamMeta));
	TStream__meta__.in.offset = -((word_t)sizeof(TOutStreamMeta));
	TStream_initialized = 1;
}

void types_module(void* memory, void* process, void* param) {
	TStream_module(memory,process,param);
}
