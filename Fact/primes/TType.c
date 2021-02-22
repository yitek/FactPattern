#include "TType.h"

TType* TType__create(const char* name, usize_t mixins, usize_t interfaces, usize_t fields, usize_t methods, usize_t properties, TMemory* mm) {
	usize_t nameSize = m_strlen(name);
	usize_t asize = (mixins + interfaces + fields + methods + properties) * sizeof(TArray) + m_strlen(name) + sizeof(TType);
	TType* type = (TType*)TMemory_alloc__virtual__(mm, asize, MemoryKind_disCollect | MemoryKind_system | MemoryKind_readonly);
	byte_t* buffer = (byte_t*)(type + 1);
	// name
	type->name = (TString*)buffer; m_copy(buffer += sizeof(TString), name, nameSize); buffer += nameSize;
	type->name->bytes = nameSize; type->name->length = -1;
	// mixins
	type->mixins = (TArray*)buffer;
	type->mixins->length = mixins;
	buffer += sizeof(TArray); buffer += sizeof(TType*) * mixins;
	// interface
	type->interfaces = (TArray*)buffer;
	type->mixins->length = interfaces;
	buffer += sizeof(TArray); buffer += sizeof(TType*) * interfaces;
	//fields
	type->fields = (TArray*)buffer;
	type->fields->length = fields;
	buffer += sizeof(TArray); buffer += sizeof(TField) * fields;
	//fields
	type->methods = (TArray*)buffer;
	type->methods->length = methods;
	buffer += sizeof(TArray); buffer += sizeof(TMethod) * methods;

	type->properties = (TArray*)buffer;
	type->properties->length = properties;
	buffer += sizeof(TArray); buffer += sizeof(TMethod) * properties;
	return type;
}
