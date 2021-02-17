#include "VMachine.h"
#include "../memories/GCMemory.h"

VMachine* VMachine_initialize(VMachine* self, VMachineOptions* opts) {
	self->rw_memory = GCMemory___construct__(0, 0, (AlignedMemoryOptions*)opts, 0);
	self->r_memory = AlignedMemory___construct__(0, (AlignedMemoryOptions*)opts, 0);
	self->cores = self->sys_memory->require(self->sys_memory,sizeof(VCore*)* opts->coreCount,0);
	return self;
}
VMachine* VMachine___construct__(VMachine* self, VMachineOptions* opts) {
	Memory* mm = Memory_default();
	if (!self) self = mm->require(mm,sizeof(VMachine),0);
	self->sys_memory = mm;
	if (!opts) {
		VMachineOptions dftOpts;
		dftOpts.coreCount = 1;
		dftOpts.pageSize = 1024 * 2;
		dftOpts.pageCount = 0;
		dftOpts.unitMetaSize = sizeof(GCUnit);
		return VMachine_initialize(self,&dftOpts);
	}
	else {
		if (!opts->coreCount) opts->coreCount = 1;
		if (!opts->unitMetaSize) opts->unitMetaSize = sizeof(GCUnit);
		return VMachine_initialize(self, opts);
	}
	
}