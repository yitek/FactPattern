#include "TType.h"

TType* TType__create(const char* name, usize_t mixins, usize_t interfaces, usize_t fields, usize_t methods, usize_t properties, TMemory* mm) {
	usize_t nameSize = m_strlen(name);
	usize_t asize = (mixins + interfaces + fields + methods + properties) * sizeof(struct stTArray) + m_strlen(name) + sizeof(TType);
	TType* type = (TType*)m_alloc( asize,0, MemoryKind_disCollect | MemoryKind_system | MemoryKind_readonly,mm);
	byte_t* buffer = (byte_t*)(type + 1);
	// name
	type->name = (struct stTString*)buffer; m_copy(buffer += sizeof(struct stTString), name, nameSize); buffer += nameSize;
	type->name->bytes = nameSize; type->name->length = -1;
	// mixins
	type->mixins = (struct stTArray*)buffer;
	type->mixins->length = mixins;
	buffer += sizeof(struct stTArray); buffer += sizeof(TType*) * mixins;
	// interface
	type->interfaces = (struct stTArray*)buffer;
	type->mixins->length = interfaces;
	buffer += sizeof(struct stTArray); buffer += sizeof(TType*) * interfaces;
	//fields
	type->fields = (struct stTArray*)buffer;
	type->fields->length = fields;
	buffer += sizeof(struct stTArray); buffer += sizeof(struct stTField) * fields;
	//fields
	type->methods = (struct stTArray*)buffer;
	type->methods->length = methods;
	buffer += sizeof(struct stTArray); buffer += sizeof(TMethod) * methods;

	type->properties = (struct stTArray*)buffer;
	type->properties->length = properties;
	buffer += sizeof(struct stTArray); buffer += sizeof(TMethod) * properties;
	return type;
}
