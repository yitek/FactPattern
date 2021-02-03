#pragma once
#include "FactVM.h"


typedef struct structInstructChunk {
	int count;
	int size;
	struct structInstructChunk* next;
	
}InstructChunk;
typedef struct  
{
	int chunkSize;
	int size;
	int printTrace;
	InstructChunk* head;
	InstructChunk* tail;
}Emitor;

Emitor* createEmitor(int chunkSize);
void distoryEmitor(Emitor* emitor);
void saveEmitorToFile(const Emitor* emitor,const char* filename);

void emit0(Emitor* emitor, Operations operation, DirectiveOptions options);
void emit(Emitor* emitor, Operations operation, DirectiveOptions options, WORD operand1);
void emit2(Emitor* emitor, Operations operation, DirectiveOptions options, WORD operand1, WORD operand2);
void emit12(Emitor* emitor, Operations operation, DirectiveOptions options, DWORD operand1);
void emit3(Emitor* emitor, Operations operation, DirectiveOptions options, WORD operand1, DWORD operand2);
void emit4(Emitor* emitor, Operations operation, DirectiveOptions options, DWORD operand1, DWORD operand2);


