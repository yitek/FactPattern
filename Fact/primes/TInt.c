#include "TInt.h"
#define _ALLOC_(size,arg,kind,mm) m_alloc(size,arg,kind,mm)
#define _ALLOC1_(size,arg,kind,mm) m_alloc1(size,arg,kind,mm)
#define _FREE_(p,mm) m_free(p,mm)


struct {
	struct stMRefUnit;
	struct stTString inst;
	char chars[3 + sizeof(unichar_t)];
} Int_nameInstance = {
	.__ref__ = 0,
	.inst.__meta__ = &TString__metaInstance,
	.inst.length = 3,
	.inst.bytes = 3,
	.chars = "Int"
};
struct {
	struct stMRefUnit;
	struct stTType inst;
} Int_typeInstance = {
	.__ref__ = 0,
	.inst.name = &Int_nameInstance,
	.inst.kind = TypeKind_struct,
	.inst.size = sizeof(int_t),
	.inst.__meta__ = 0

};
const struct stTType*const TInt__type__ = &Int_typeInstance.inst;
struct {
	struct stClazzMeta;
	void(*__endle__)();
} Int__meta__ = {
	.offset=0,
	.__gettype__=0,
	.__endle__ =0
};

TInt* TInt__construct__(TInt* self,int_t value ,TMemory* mm, MemoryKinds mkind) {
	if (!self) {
		self = _ALLOC_(sizeof(TInt), &Int_typeInstance.inst,mkind,mm);
		*((ClazzMeta**)(&self->__meta__)) = (ClazzMeta*)&Int__meta__;
	}
	self->value = value;

}


#undef _ALLOC_
#undef _ALLOC1_
#undef _FREE_