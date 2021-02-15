/*****************************************************
*
* author:yiy
*
* description: �������

*
******************************************************/

#pragma once
#include "../memories/GCMemory.h"
#ifndef __VMACHINE_INCLUDED__ 
#define __VMACHINE_INCLUDED__
// c�������Ͻ�cpp�ı�׼�÷�,ָ������ķ��Ű���c���Է�ʽ����
#ifdef __cplusplus 
extern "C" {
#endif

	typedef enum
	{
		//ͣ��
		PAUSE = 0,
		/// Ѱַָ�
		///		���Ĵ���(CR)���ڴ潻������: load(c)/store(c)
		LOAD = 1,
		STORE = 2,
		///		���Ĵ���(CR)��io��������: input(c)/output(c)
		INPUT = 3,
		OUTPUT = 4,
		///		��ջ�Ĵ���(SR)���ڴ潻������:push(n/c)/pop(nc)
		PUSH = 5,
		POP = 6,
		///		�ڴ����ݴ��Ͳ���:trans
		ADDR = 7,
		TRANS = 8,
		/// ���Ĵ����ϵ�����ָ��:
		///		��������ָ��:add(c/1)/sub(c/1)/mul(c)/div(c)
		ADD = 16,
		SUB = 17,
		MUL = 18,
		DIV = 19,
		///		�߼�����ָ��:and(c)/or(c)/xor(c)/not(c)
		AND = 32,
		OR = 33,
		XOR = 34,
		NOT = 35,
		///		λ������ָ��:shift
		SHIFT = 40,
		/// ��תָ��:
		// �����������뷵��:call/return
		CALL = 48,
		RETURN = 49,
		// ��ת��������ת:goto
		GO = 50,
		IF = 51,

		/// ͬ��ָ��:
		SPIN = 120,
		LOCK = 121,
		UNLOCK = 122
		/// Newָ��(αָ����Ϸ����ڴ�)
		///		new
	} Operations;
	typedef enum {
		OP_None = 0,
		/// <summary>
		/// 00 000 001 is double �Ƿ���˫�ֲ���
		/// </summary>
		O_DWORD = 1,
		/// <summary>
		/// 00 000 010 is immediate �Ƿ���ֱ��������
		/// </summary>
		OP_Immediate = 1 << 1,
		/// <summary>
		/// 00 000 100 is immediate �Ƿ���ֱ��������
		/// </summary>
		OP_Pointer = 1 << 2,
		/// <summary>
		/// 00 001 000 �Ƿ�ʹ��Rerective��Index��
		/// </summary>
		OP_Operand0 = 1 << 3,
		/// <summary>
		/// 00 010 000 �Ƿ�ʹ�õ�һ��������
		/// </summary>
		OP_Operand1 = 1 << 4,
		/// <summary>
		/// 00 100 000 �Ƿ�ʹ�õڶ���������
		/// </summary>
		OP_Operand2 = 1 << 5,
		/// <summary>
		/// 00 110 000
		/// </summary>
		OP_Operand3 = OP_Operand1 | OP_Operand2
	} DirectiveOptions;
	/// <summary>
	/// ָ����
	/// </summary>
	typedef struct TDirective {
		/// <summary>
		/// Ѱַָ�
		///		���Ĵ���(CR)���ڴ潻������: load(c)/store(c)
		///		���Ĵ���(CR)��io��������: input(c)/output(c)
		///		��ջ�Ĵ���(SR)���ڴ潻������:push(n/c)/pop(nc)
		///		�ڴ����ݴ��Ͳ���:trans
		/// ���Ĵ����ϵ�����ָ��:
		///		��������ָ��:add(c/1)/sub(c/1)/mul(c)/div(c)
		///		�߼�����ָ��:and(c)/or(c)/xor(c)/not(c)
		///		λ������ָ��:shift_left,shift_right
		/// ��תָ��:
		///		��ת��������ת:goto
		///		���������뷵��:call/return
		/// ͬ��ָ��:
		///		spin ����
		///		lock ����
		///		unlock ����
		/// Newָ��(αָ����Ϸ����ڴ�)
		///		new
		/// </summary>
		char operation;

		/// <summary>
		/// directive�Ĳ���������directive��ͬ�����岻ͬ��һ��ĺ�������:
		/// 0 is double �Ƿ���˫�ֲ���
		/// 1 is immediate �Ƿ���ֱ��������
		/// 2 is global(0) or local(1) �Ƿ���ȫ�ֵ�ַ����SP����Ե�ַ
		/// 3 is direct address �Ƿ���ֱ��Ѱַ/���Ѱַ(ָ�����)
		/// 4 direction 0=left,1 right ����
		/// 5 is loop �Ƿ���ѭ��swiftʹ��
		/// </summary>
		char options;
	} Directive;

	/// <summary>
	/// ָ��
	/// ��С��ָ���һ��WORD_SIZE��
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
		/// 8��word��ͨ�üĴ���ջ
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
		/// ��ջָ��
		/// </summary>
		word_t* sp;
		/// <summary>
		/// ָ��ָ��
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



