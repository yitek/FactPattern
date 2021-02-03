#pragma once

#pragma once

const int WORD_SIZE = 4;
/// <summary>
/// ���������
/// </summary>
typedef int WORD;

typedef long DWORD;

typedef short INDEX;

typedef char* POINTER;



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
	///		�ڴ����ݴ��Ͳ���:��ĳ����ַ�����ݴ��͵�ָ����ַȥ
	COPY = 7,
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

	/// ͬ��ָ��:����/����/����
	SPIN = 120,
	LOCK = 121,
	UNLOCK = 122
	/// Newָ��(αָ����Ϸ����ڴ�)
	///		new
}Operations;
typedef enum {
	OP_None = 0,
	/// 0 is double �Ƿ���˫�ֲ���
	OP_Double = 1,
	/// 1 is immediate �Ƿ���ֱ��������
	OP_Immediate = 1 << 1,
	// �Ƿ�ʹ�õ�һ��������
	OP_Operand1 = 1 << 2,
	// �Ƿ�ʹ�õڶ���������
	OP_Operand2 = 1 << 3,
	/// �Ƿ�ʹ��Rerective��Index��
	OP_Index = 1 << 4,
	OP_OperandIndex = 1 << 5
} DirectiveOptions;
/// <summary>
/// ָ����
/// </summary>
typedef struct {
	/// <summary>
	/// Ѱַָ�
	///		���Ĵ���(CR)���ڴ潻������: load(c)/store(c)
	///		���Ĵ���(CR)��io��������: input(c)/output(c)
	///		��ջ�Ĵ���(SR)���ڴ潻������:push(n/c)/pop(nc)
	///		�ڴ����ݴ��Ͳ���:trans
	/// ���Ĵ����ϵ�����ָ��:
	///		��������ָ��:add(c/1)/sub(c/1)/mul(c)/div(c)
	///		�߼�����ָ��:and(c)/or(c)/xor(c)/not(c)
	///		λ������ָ��:shift
	/// ��תָ��:
	///		��ת��������ת:goto
	///		���������뷵��:call/return
	/// ͬ��ָ��:
	///		lock
	///		unlock
	/// Newָ��(αָ����Ϸ����ڴ�)
	///		new
	/// </summary>
	char opcode;

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
/// 4 bytes
/// </summary>
typedef struct _Instruction{
	union {
		Directive directive;
		char bytes[WORD_SIZE];
	};

}Instruction;
/// <summary>
///  4 bytes
/// </summary>
typedef union _IndexInstruction {
	struct {
		Directive directive;
		INDEX index;
	};
	/// <summary>
	/// ����Ҫ��һ��WORD_SIZE
	/// </summary>
	char bytes[WORD_SIZE];
}IndexInstruction;

typedef struct
{
	struct _Instruction;
	WORD operand1;

}Instruction1;



typedef struct
{
	Directive directive;
	INDEX index;
	WORD operand1;

} IndexInstruction1;

typedef struct
{
	Directive directive;
	union {
		struct {
			WORD operand1;
			WORD operand2;
		};
		DWORD d_operand1;
	};


}Instruction2;

typedef struct
{
	union {
		Directive directive;
		struct {
			union {
				char op;
				char operation;
			};
			char options;
		};
	};
	INDEX index;
	union {
		struct {
			WORD operand1;
			WORD operand2;
		};
		DWORD d_operand1;
	};


}IndexableInstruction2;

typedef struct
{
	union {
		Directive directive;
		struct {
			union {
				char op;
				char operation;
			};
			char options;
		};
	};
	WORD operand1;
	DWORD operand2;

}NoindexInstruction3;

typedef struct IndexableInstruction3
{
	union {
		Directive directive;
		struct {
			union {
				char op;
				char operation;
			};
			char options;
		};
	};
	INDEX index;
	WORD operand1;
	DWORD operand2;

}IndexableInstruction3;

typedef struct
{
	union {
		Directive directive;
		struct {
			union {
				char op;
				char operation;
			};
			char options;
		};
	};
	DWORD d_operand1;
	DWORD d_operand2;

}NoindexInstruction4;

typedef struct IndexInstruction4
{
	union {
		Directive directive;
		struct {
			union {
				char op;
				char operation;
			};
			char options;
		};
	};
	INDEX index;
	DWORD d_operand1;
	DWORD d_operand2;

}IndexInstruction4;
typedef union {
	struct {
		WORD operand1;
		WORD operand2;
	};
	struct {
		WORD first;
		DWORD d_second;
	};
	struct {
		DWORD d_operand1;
		DWORD d_operand2;
	};
}Operand;
typedef struct
{
	union {
		Directive directive;
		struct {
			char op;
			char options;
		};
	};
	union {
		struct {
			INDEX index;

			union {
				Operand operand;
				struct {
					WORD operand1;
					WORD operand2;
				};
				struct {
					WORD first;
					DWORD d_second;
				};
				struct {
					DWORD d_operand1;
					DWORD d_operand2;
				};
			};

		}indexable;
		struct {
			char paddings[(sizeof(WORD) - sizeof(Directive)) > 0 ? sizeof(WORD) - sizeof(Directive) : 0];
			union {
				WORD operand;
				DWORD d_operand;
				struct {
					WORD operand1;
					WORD operand2;
				};
				struct {
					WORD first;
					DWORD d_second;
				};
				struct {
					DWORD d_operand1;
					DWORD d_operand2;
				};
			};
		} noindex;
	};

} Instruction;



typedef struct {
	/// <summary>
	/// ͨ�üĴ���(Common Register)ջ
	/// 4��WORD�����
	/// </summary>
	union {
		WORD crs[4];
		DWORD drs[2];
		struct {
			WORD cr0;
			WORD cr1;
			WORD cr2;
			WORD cr3;
		};
		struct {
			DWORD dr0;
			DWORD dr1;
		};
	};
	/// <summary>
	/// ��ջ(Stack Register)�Ĵ���
	/// </summary>
	WORD* sr;
	/// <summary>
	/// ����(Program Register)�Ĵ���
	/// ��ǰָ��ĵ�ַ
	/// </summary>
	char* pr;
	/// <summary>
	/// ָ��Ĵ���
	/// </summary>
	Instruction ir;
} Machine;
typedef struct {
	const char* file;
} MachineStartOptions;

int getDirectiveSize(Directive* instruction);

Machine* createMachine(const MachineStartOptions* opts);

void startMachine(Machine* vm);

void printDirective(Directive* instruction);
