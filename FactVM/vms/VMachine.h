/*****************************************************
*
* author:yiy
*
* description: 虚拟机类

*
******************************************************/

#pragma once
#include "../memories/GCMemory.h"
#ifndef __VMACHINE_INCLUDED__ 
#define __VMACHINE_INCLUDED__
// c语言整合进cpp的标准用法,指定里面的符号按照c语言方式产生
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum
	{
		//停机
		PAUSE = 0,
		/// 寻址指令：
		///		主寄存器(CR)与内存交互操作: load(c)/store(c)
		LOAD = 1,
		STORE = 2,
		///		主寄存器(CR)与io交互操作: input(c)/output(c)
		INPUT = 3,
		OUTPUT = 4,
		///		堆栈寄存器(SR)与内存交互操作:push(n/c)/pop(nc)
		PUSH = 5,
		POP = 6,
		///		内存数据传送操作:trans
		ADDR = 7,
		TRANS = 8,
		/// 主寄存器上的运算指令:
		///		算数运算指令:add(c/1)/sub(c/1)/mul(c)/div(c)
		ADD = 16,
		SUB = 17,
		MUL = 18,
		DIV = 19,
		///		逻辑运算指令:and(c)/or(c)/xor(c)/not(c)
		AND = 32,
		OR = 33,
		XOR = 34,
		NOT = 35,
		///		位移运算指令:shift
		SHIFT = 40,
		/// 跳转指令:
		// 函数数调用与返回:call/return
		CALL = 48,
		RETURN = 49,
		// 跳转或条件跳转:goto
		GO = 50,
		IF = 51,

		/// 同步指令:
		SPIN = 120,
		LOCK = 121,
		UNLOCK = 122
		/// New指令(伪指令，堆上分配内存)
		///		new
	} Operations;
	typedef enum {
		OP_None = 0,
		/// <summary>
		/// 00 000 001 is double 是否是双字操作
		/// </summary>
		O_DWORD = 1,
		/// <summary>
		/// 00 000 010 is immediate 是否是直接数操作
		/// </summary>
		OP_Immediate = 1 << 1,
		/// <summary>
		/// 00 000 100 is immediate 是否是直接数操作
		/// </summary>
		OP_Pointer = 1 << 2,
		/// <summary>
		/// 00 001 000 是否使用Rerective的Index域
		/// </summary>
		OP_Operand0 = 1 << 3,
		/// <summary>
		/// 00 010 000 是否使用第一个操作数
		/// </summary>
		OP_Operand1 = 1 << 4,
		/// <summary>
		/// 00 100 000 是否使用第二个操作数
		/// </summary>
		OP_Operand2 = 1 << 5,
		/// <summary>
		/// 00 110 000
		/// </summary>
		OP_Operand3 = OP_Operand1 | OP_Operand2
	} DirectiveOptions;
	/// <summary>
	/// 指令码
	/// </summary>
	typedef struct TDirective {
		/// <summary>
		/// 寻址指令：
		///		主寄存器(CR)与内存交互操作: load(c)/store(c)
		///		主寄存器(CR)与io交互操作: input(c)/output(c)
		///		堆栈寄存器(SR)与内存交互操作:push(n/c)/pop(nc)
		///		内存数据传送操作:trans
		/// 主寄存器上的运算指令:
		///		算数运算指令:add(c/1)/sub(c/1)/mul(c)/div(c)
		///		逻辑运算指令:and(c)/or(c)/xor(c)/not(c)
		///		位移运算指令:shift_left,shift_right
		/// 跳转指令:
		///		跳转或条件跳转:goto
		///		函数调用与返回:call/return
		/// 同步指令:
		///		spin 自旋
		///		lock 加锁
		///		unlock 解锁
		/// New指令(伪指令，堆上分配内存)
		///		new
		/// </summary>
		char operation;

		/// <summary>
		/// directive的参数，根据directive不同而含义不同，一般的含义如下:
		/// 0 is double 是否是双字操作
		/// 1 is immediate 是否是直接数操作
		/// 2 is global(0) or local(1) 是否是全局地址还是SP的相对地址
		/// 3 is direct address 是否是直接寻址/间接寻址(指针访问)
		/// 4 direction 0=left,1 right 方向
		/// 5 is loop 是否是循环swift使用
		/// </summary>
		char options;
	} Directive;

	/// <summary>
	/// 指令
	/// 最小的指令跟一样WORD_SIZE大
	/// </summary>
	typedef struct TInstruction {
		union {
			struct {
				struct TDirective;
				byte_t operand0[sizeof(word_t) - sizeof(Directive) ? sizeof(word_t) - sizeof(Directive) : sizeof(word_t)];
			};
			word_t instruction0;
		};
		union {
			struct {
				word_t operand1;
				word_t operand2;
				word_t operand3;
			};
			struct {
				dword_t d_operand1;
				dword_t d_operand2;
				dword_t d_operand3;
			};
		};
	}Instruction;

	struct stVMachine;

	typedef struct stVCore {
		/// <summary>
		/// 8个word的通用寄存器栈
		/// </summary>
		union {
			word_t crs[8];
			struct {
				word_t cr0;
				word_t cr1;
				word_t cr2;
				word_t cr3;
				word_t cr4;
				word_t cr5;
				word_t cr6;
				word_t cr7;
			};
			dword_t dcrs[4];
			struct {
				dword_t dcr0;
				dword_t dcr1;
				dword_t dcr2;
				dword_t dcr3;
			};
		};
		/// <summary>
		/// 堆栈指针
		/// </summary>
		word_t* sp;
		/// <summary>
		/// 指令指针
		/// </summary>
		Instruction* ip;

		struct stVMachine* machine;
	}VCore;

	typedef struct stVMachineOptions {
		struct stAlignedMemoryOptions;
		size_t coreCount;
	}VMachineOptions;

	typedef struct stVMachine {
		struct stVMachineOptions;
		VCore** cores;
		GCMemory* rw_memory;
		AlignedMemory* r_memory;
		Memory* sys_memory;
	}VMachine;

	VMachine* VMachine___construct__(VMachine* self, VMachineOptions* opts);
#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __VMACHINE_INCLUDED__



