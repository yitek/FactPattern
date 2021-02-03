#pragma once

#pragma once

const int WORD_SIZE = 4;
/// <summary>
/// 定义机器字
/// </summary>
typedef int WORD;

typedef long DWORD;

typedef short INDEX;

typedef char* POINTER;



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
	///		内存数据传送操作:将某个地址的数据传送到指定地址去
	COPY = 7,
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

	/// 同步指令:自旋/加锁/解锁
	SPIN = 120,
	LOCK = 121,
	UNLOCK = 122
	/// New指令(伪指令，堆上分配内存)
	///		new
}Operations;
typedef enum {
	OP_None = 0,
	/// 0 is double 是否是双字操作
	OP_Double = 1,
	/// 1 is immediate 是否是直接数操作
	OP_Immediate = 1 << 1,
	// 是否使用第一个操作数
	OP_Operand1 = 1 << 2,
	// 是否使用第二个操作数
	OP_Operand2 = 1 << 3,
	/// 是否使用Rerective的Index域
	OP_Index = 1 << 4,
	OP_OperandIndex = 1 << 5
} DirectiveOptions;
/// <summary>
/// 指令码
/// </summary>
typedef struct {
	/// <summary>
	/// 寻址指令：
	///		主寄存器(CR)与内存交互操作: load(c)/store(c)
	///		主寄存器(CR)与io交互操作: input(c)/output(c)
	///		堆栈寄存器(SR)与内存交互操作:push(n/c)/pop(nc)
	///		内存数据传送操作:trans
	/// 主寄存器上的运算指令:
	///		算数运算指令:add(c/1)/sub(c/1)/mul(c)/div(c)
	///		逻辑运算指令:and(c)/or(c)/xor(c)/not(c)
	///		位移运算指令:shift
	/// 跳转指令:
	///		跳转或条件跳转:goto
	///		函数调用与返回:call/return
	/// 同步指令:
	///		lock
	///		unlock
	/// New指令(伪指令，堆上分配内存)
	///		new
	/// </summary>
	char opcode;

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
	/// 至少要到一个WORD_SIZE
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
	/// 通用寄存器(Common Register)栈
	/// 4个WORD的深度
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
	/// 堆栈(Stack Register)寄存器
	/// </summary>
	WORD* sr;
	/// <summary>
	/// 程序(Program Register)寄存器
	/// 当前指令的地址
	/// </summary>
	char* pr;
	/// <summary>
	/// 指令寄存器
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
