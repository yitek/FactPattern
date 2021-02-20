#include "layout.h"
#include <malloc.h>

void* m_alloc(usize_t size, uword_t memtype){return malloc(size);}
bool_t m_free(void* p){ free(p); return 1;}