#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "FactVM.h"






Machine* createMachine(const MachineCreationOptions* opts) {
	Machine* machine = (Machine*)malloc(sizeof(Machine));
	return machine;
}