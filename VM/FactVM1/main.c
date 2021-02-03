#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Emitor.h"

int main()
{
    printf("%d\n", (sizeof(WORD) - sizeof(Directive)) > 0 ? sizeof(WORD) - sizeof(Directive) : 0);
    printf("%d\n", sizeof(NoindexInstruction));
    NoindexInstruction ins;
    Emitor* emitor = createEmitor(1024);
    emitor->printTrace = 1;
    emit(emitor,LOAD,OP_Immediate,2);
    emit(emitor,LOAD,OP_Immediate,3);
    emit0(emitor,ADD,OP_None);
    saveEmitorToFile(emitor,"program.o");
    printf("press any key to continue...");
    _getch();
}