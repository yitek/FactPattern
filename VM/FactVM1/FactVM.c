// FactVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <malloc.h>
#include <stdio.h>

#include "FactVM.h"

int getDirectiveSize(Directive* instruction) {
    int instructionSize;
    if (instruction->op == INPUT || instruction->op == OUTPUT || (instruction->op & OP_Index) != 0) {
        instructionSize = sizeof(IndexableInstruction);
        if (instruction->options & OP_Operand1) {
            if (instruction->options & OP_Double) instructionSize += sizeof(DWORD);
            else instructionSize += sizeof(WORD);
        }
        
    }
    else {
        instructionSize = sizeof(NoindexInstruction);
        if (instruction->options & OP_Operand1) {
            if (instruction->options & OP_Double) instructionSize += sizeof(DWORD);
            else instructionSize += sizeof(WORD);
            if (instruction->options & OP_Operand2) {
                if (instruction->options & OP_Double) instructionSize += sizeof(DWORD);
                else instructionSize += sizeof(WORD);
            }
        }        
    }
    return instructionSize;
}
void push_cr(Machine* vm, WORD n) {
    vm->crs[3] = vm->crs[2];
    vm->crs[2] = vm->crs[1];
    vm->crs[1] = vm->crs[0];
    vm->crs[0] = n;
}
void push_dr(Machine* vm, DWORD n) {
    vm->drs[1] = vm->drs[0];
    vm->drs[0] = n;
}


int exec_load(Machine* vm, Instruction* ir) {
    if (ir->options & OP_Immediate) {
        if (ir->options & OP_Double) {
            push_dr(vm, ir->noindex.d_operand1);
            return sizeof(NoindexInstruction2);
        }
        else {
            push_cr(vm, ir->noindex.operand1);
            return sizeof(NoindexInstruction1);
        }
    }

    if (ir->options & OP_Index) {
        if (ir->options & OP_Double) {
            push_dr(vm, *(DWORD*)(vm->sr + ir->indexable.index));
            return sizeof(IndexableInstruction);
        }
        else {
            push_dr(vm, *(vm->sr + ir->indexable.index));
            return sizeof(IndexableInstruction);
        }
    }
    else {
        if (ir->options & OP_Double) {
            push_dr(vm, *(DWORD*)ir->noindex.operand1);
            return sizeof(NoindexInstruction1);
        }
        else {
            push_cr(vm, *(WORD*)ir->noindex.operand1);
            return sizeof(NoindexInstruction1);
        }
    }
}

int exec_store(Machine* vm, Instruction* ir) {
    if (ir->options & OP_Immediate) {
        if (ir->options & OP_Double) {
            *((DWORD*)ir->noindex.operand1) = vm->dr0;
            return sizeof(NoindexInstruction2);
        }
        else {
            *((WORD*)ir->noindex.operand1) = vm->cr0;
            return sizeof(NoindexInstruction1);
        }
    }

    if (ir->options & OP_Index) {
        if (ir->options & OP_Double) {
            *(DWORD*)(vm->sr + ir->indexable.index)=vm->dr0;
            return sizeof(IndexableInstruction);
        }
        else {
            *(WORD*)(vm->sr + ir->indexable.index) = vm->cr0;
            return sizeof(IndexableInstruction);
        }
    }
    else {
        if (ir->options & OP_Double) {
            *(DWORD*)ir->noindex.operand1 = vm->dr0;
            return sizeof(NoindexInstruction1);
        }
        else {
            *(WORD*)ir->noindex.operand1 = vm->cr0;
            return sizeof(NoindexInstruction1);
        }
    }
}


int exec_add(Machine* vm, Instruction* ir) {
    if (ir->options & OP_Double) {
        vm->dr0 += vm->dr1;
        return sizeof(NoindexInstruction);
    }
    else {
        vm->cr0 += vm->cr1;
        return sizeof(NoindexInstruction);
    }
    
}
int exec_output(Machine* vm, Instruction* ir) {
    if (ir->indexable.index == 0) {
        putchar(((IndexableInstruction1*)ir)->operand1);
    }
    return sizeof(IndexableInstruction1);
}

char* loadInstructionFromFile(Machine* vm, const char* filename) {
    FILE* fp;
    //判断按读方式打开一个名叫test的文件是否失败
    if (!fopen_s(&fp, filename, "r"))//打开操作不成功
    {
        printf("The file %s can not be opened.\n", filename);
        exit(1);//结束程序的执行
    }
    if (fp) {
        char* buffer = (char*)malloc(1024 * 8);
        if (buffer) {
            fread_s(buffer, 1024 * 8, 1024, 8, fp);
            fclose(fp);
        }
        else {
            printf("can not alloc memory for instructions.\n");
            exit(1);//结束程序的执行
        }
        
        return buffer;
    }
    return 0;
}

Machine* createMachine(const MachineStartOptions* opts) {
    Machine* machine = (Machine*)malloc(sizeof(Machine));
    if (opts->file) {
        loadInstructionFromFile(machine,opts->file);
    }
    return machine;
}



void startMachine(Machine* vm) {
    while (1) {
        Instruction* ir = (Instruction*)vm->pr;

        WORD cost = 0;
        switch (ir->op)
        {
        case STORE:cost = exec_store(vm, ir); break;
        case LOAD:cost = exec_load(vm, ir); break;
        case ADD:cost = exec_add(vm, ir); break;
        case OUTPUT: cost = exec_output(vm,ir); break;
        default:
            break;
        }
        if (cost == 0)return;
        vm->pr += cost;
    }
    
}

void printX(int x) {
    if (x < 10) printf("%d", x);
    else {
        int c = 'A' + x % 10;
        printf("%c", c);
    }
}
void printDirective(Directive* instruction) {
    switch (instruction->op)
    {
        case LOAD:printf("load"); break;
        case STORE:printf("store"); break;
        case OUTPUT:printf("output"); break;
        case ADD:printf("add"); break;
    default:
        break;
    }
    if (instruction->options & OP_Immediate) printf(".c");
    if (instruction->options & OP_Double) printf(".d");
    if (instruction->options & OP_Index) {
        printf(" [");
        ((IndexableInstruction*)instruction)->index;
        printf(" ]");
        if (instruction->options & OP_OperandIndex) {
            if ((instruction->options & OP_Double)) {
                printf(" %ld", ((IndexableInstruction2*)instruction)->d_operand1);
            }
            else {
                printf(" %d", ((IndexableInstruction1*)instruction)->operand1);
            }
        }
    }
    else {
        if (instruction->options & OP_Operand1) {
            if (instruction->options && OP_Operand2) {
                if ((instruction->options & OP_Double)) {
                    printf(" %ld %ld", ((NoindexInstruction4*)instruction)->d_operand1, ((NoindexInstruction4*)instruction)->d_operand2);
                }
                else {
                    printf(" %d %d", ((NoindexInstruction2*)instruction)->operand1, ((NoindexInstruction2*)instruction)->operand2);
                }
            }
            else {
                if ((instruction->options & OP_Double)) {
                    printf(" %ld", ((NoindexInstruction2*)instruction)->d_operand1);
                }
                else {
                    printf(" %d", ((NoindexInstruction2*)instruction)->operand1);
                }
            }
        }
    }
    int size = getDirectiveSize(instruction);
    printf(";//%d",size);
    char* inst = *((char*)instruction);
    for (int i = 0; i < size; i++) {
        int c = inst[i];
        int first = c / 16;
        int last = c % 16;
        printX(first);
        printX(last);
        printf(" ");
    }
    printf("\n");
    
}






// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
