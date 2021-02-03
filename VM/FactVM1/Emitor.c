// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "FactVM.h"
#include "Emitor.h"


Emitor* createEmitor(int size) {
	Emitor* pChunk = (Emitor*)malloc(sizeof(Emitor));
	if (pChunk) {
		if (size == 0)pChunk->chunkSize = 4096;
		else pChunk->chunkSize = size;
		pChunk->head = pChunk->tail = 0;
	}
	return pChunk;
}

void distoryEmitor(Emitor* emitor) {
	InstructChunk* chunk = emitor->head;
	while (chunk) {
		InstructChunk* next = chunk->next;
		free(chunk);
		chunk = next;
	}
	free(emitor);
}

Directive* allocInstructionFromChunk(Emitor* emitor, int instructSize) {
	InstructChunk* pChunk = emitor->tail;
	if (!pChunk) {
		pChunk = emitor->head = emitor->tail = (InstructChunk*)malloc(emitor->chunkSize);
		if (!pChunk) return 0;
	}

	if ((pChunk->size - pChunk->count) < instructSize) {
		InstructChunk* nextChunk = (InstructChunk*)malloc(emitor->chunkSize);
		if (!nextChunk) return 0;
		nextChunk->size = emitor->chunkSize;
		nextChunk->count = sizeof(InstructChunk);
		pChunk->next = nextChunk;
		emitor->tail = nextChunk;
		pChunk = nextChunk;
	}
	pChunk->count += instructSize;
	return (Directive *) (((HOSTADDR)pChunk) + instructSize);
}

Operand* internalEmit(Emitor* emitor, Operations operation, DirectiveOptions options,INDEX index) {
	int instructionSize;
	HOSTADDR retDirective;
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		instructionSize = sizeof(IndexableInstruction);
		if (options & OP_Operand1) {
			if (options & OP_Double) instructionSize += sizeof(DWORD);
			else instructionSize += sizeof(WORD);
		}
		if (options & OP_Operand2) {
			printf("EXCEPTION(Argument):OP_Index跟OP_Operand2不能同时为True");
			exit(1);
		}
		IndexableInstruction* directive =(IndexableInstruction*) allocInstructionFromChunk(emitor, instructionSize);
		directive->op = operation;
		directive->options = options;
		directive->index = index;
		if (getDirectiveSize(directive) != instructionSize) {
			printf("EXCEPTION(Program):getDirectiveSize()得到的指令大小跟emit()中得到的指令大小不一致");
			exit(1);
		}
		retDirective = (HOSTADDR)directive;
		retDirective += sizeof(IndexableInstruction);
	}
	else {
		instructionSize = sizeof(NoindexInstruction);
		if (options & OP_Operand1) {
			if (options & OP_Double) instructionSize += sizeof(DWORD);
			else instructionSize += sizeof(WORD);
			if (options & OP_Operand2) {
				if (options & OP_Double) instructionSize += sizeof(DWORD);
				else instructionSize += sizeof(WORD);
			}
		}
		else if (options & OP_Operand2) {
			printf("EXCEPTION(Argument):设置OP_Operand2之前必须先设置OP_Operand1");
			exit(1);
		}
		NoindexInstruction* directive = (NoindexInstruction*)allocInstructionFromChunk(emitor, instructionSize);
		directive->op = operation;
		directive->options = options;
		if (getDirectiveSize(directive) != instructionSize) {
			printf("EXCEPTION(Program):getDirectiveSize()得到的指令大小跟emit()中得到的指令大小不一致");
			exit(1);
		}
		retDirective = (HOSTADDR)directive;
		retDirective += sizeof(NoindexInstruction);
	}
	
	return (Operand*)(retDirective);
}
void emit0(Emitor* emitor, Operations operation, DirectiveOptions options) {
	internalEmit(emitor,operation,options,0);
}
void emit(Emitor* emitor, Operations operation, DirectiveOptions options,WORD operand1) {
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		internalEmit(emitor, operation, options, operand1);
	}
	else {
		Operand* operand = internalEmit(emitor, operation, options, 0);
		operand->first = operand1;
	}
}

void emit2(Emitor* emitor, Operations operation, DirectiveOptions options, WORD operand1,WORD operand2) {
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		Operand* operand = internalEmit(emitor, operation, options, operand1);
		operand->first = operand1;
	}
	else {
		Operand* operand = internalEmit(emitor, operation, options, 0);
		operand->operand1 = operand1;
		operand->operand2 = operand2;
	}
}
void emit12(Emitor* emitor, Operations operation, DirectiveOptions options, DWORD operand1) {
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		Operand* operand = internalEmit(emitor, operation, options, (INDEX)operand1);
	}
	else {
		Operand* operand = internalEmit(emitor, operation, options, 0);
		operand->d_operand1 = operand1;
	}
}

void emit3(Emitor* emitor, Operations operation, DirectiveOptions options, WORD operand1, DWORD operand2) {
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		Operand* operand = internalEmit(emitor, operation, options, operand1);
		operand->d_operand1 = operand2;
	}
	else {
		Operand* operand = internalEmit(emitor, operation, options, 0);
		operand->operand1 = operand1;
		operand->d_second = operand2;
	}
}
void emit4(Emitor* emitor, Operations operation, DirectiveOptions options, DWORD operand1, DWORD operand2) {
	if (operation == INPUT || operation == OUTPUT || (options & OP_Index) != 0) {
		Operand* operand = internalEmit(emitor, operation, options, (INDEX)operand1);
		operand->d_operand1 = operand2;
	}
	else {
		Operand* operand = internalEmit(emitor, operation, options, 0);
		operand->d_operand1 = operand1;
		operand->d_second = operand2;
	}
}

void saveEmitorToFile(const Emitor* emitor ,const char* filename) {

	//定义一个名叫fp文件指针
	FILE* fp;
	//判断按读方式打开一个名叫test的文件是否失败
	if(!fopen_s(&fp,filename,"w"))//打开操作不成功
	{
		printf("The file %s can not be opened.\n",filename);
		exit(1);//结束程序的执行
	}
	if (fp) {
		InstructChunk* chunk = emitor->head;
		while (chunk) {
			fwrite((HOSTADDR)chunk + sizeof(InstructChunk), sizeof(WORD), chunk->count / sizeof(WORD), fp);
			chunk = chunk->next;
		}
		fclose(fp);
	}
	
}







