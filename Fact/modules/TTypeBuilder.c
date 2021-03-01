#include "TTypeBuilder.h"
#define _ALLOC_(size,arg,kind,mm) m_alloc(size,arg,kind,mm)
#define _ALLOC1_(size,arg,kind,mm) m_alloc1(size,arg,kind,mm)
#define _FREE_(p,mm) m_free(p,mm)

TTypeBuilder* TTypeBuilder__construct__(TTypeBuilder* self, TMemory* memory, void* listType, MemoryKinds mkind) {
	if (self == 0) {
		if (!memory)memory = TMemory_default;
		if (!self) {
			self = _ALLOC_(sizeof(TTypeBuilder),0,mkind,memory);
		}
	}
	self->mm = memory;
	self->mkind = mkind;
	self->fields = _ALLOC1_(sizeof(TList),0,mkind,memory);
	self->properties = _ALLOC1_(sizeof(TList), 0, mkind, memory);
	self->methods = _ALLOC1_(sizeof(TList), 0, mkind, memory);
	self->interfaces = _ALLOC1_(sizeof(TList), 0, mkind, memory);
	self->mixins = _ALLOC1_(sizeof(TList), 0, mkind, memory);
	self->genericRestricts = _ALLOC1_(sizeof(TList), 0, mkind, memory);
	return self;
}

TTypeBuilder* TTypeBuilder_init(TTypeBuilder* self,const char* name,TypeKinds kind) {
	self->name = TString__construct__(0, name, -1, self->mm, self->mkind);
	ref_inc(self->name);
	self->kind = kind;
	return self;
}
struct stTField* TTypeBuilder_addField(TTypeBuilder* self, const char* name, struct stTType* fieldType,ufavor_t decorators) {
	struct stTField* field = TList_append(self->fields, self->mm, self->mkind);
	field->name = TString__construct__(0, name, -1, self->mm, self->mkind);
	ref_inc(self->name);
	field->type = fieldType;
	field->decorators = decorators;
	return field;
}

#undef _ALLOC_
#undef _ALLOC1_
#undef _FREE_