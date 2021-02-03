#pragma once

/// <summary>
/// 机器字
/// </summary>
typedef char BIT08;

typedef short BIT16;
/// <summary>
/// 常用数
/// </summary>
typedef int BIT32;

/// <summary>
/// 常用数
/// </summary>
typedef int BIT64;

typedef int HOST_WORD;
typedef int HOST_DWORD;
typedef char* HOST_ADDR;

typedef enum 
{
	//停机
	PAUSE = 0,
	/// <summary>
	/// 存储操作
	/// </summary>
	DIRECTIVE_STORAGE = 1,
	/// <summary>
	/// 加载全局地址的值到主寄存器
	/// </summary>
	LOAD = 1,
	/// <summary>
	/// 存储主寄存器的值到内存
	/// </summary>
	STORE = 2,
	// 主寄存器(CR)与io交互操作: input(c)/output(c)
	INPUT = 3,
	OUTPUT = 4,
	// 堆栈寄存器(SR)与内存交互操作:push(n/c)/pop(nc)
	PUSH = 5,
	// 
	POP = 6,
	///		内存数据传送操作:trans
	ADDR = 7,
	TRANS = 8,
	/// 算数操作
	DIRECTIVE_ARITHMETIC = 16,
	///		逻辑运算指令:and(c)/or(c)/xor(c)/not(c)
	AND = 16,
	OR = 17,
	XOR = 18,
	NOT = 19,
	///		位移运算指令:shift
	LSHIFT = 20,
	RSHIFT = 21,
	///		算数运算指令:add(c/1)/sub(c/1)/mul(c)/div(c)
	ADD = 24,
	SUB = 25,
	MUL = 26,
	DIV = 27,
	
	
	/// 跳转指令:
	DIRECTIVE_JUMP = 40,
	// 跳转或条件跳转:goto
	GOTO = 40,
	IF = 41,
	// 函数数调用与返回:call/return
	CALL = 42,
	RETURN = 43,
	
	
	/// 同步指令:
	DIRECTIVE_SYNC = 120,
	SPIN = 120,
	LOCK = 121,
	UNLOCK = 122,
	/// New指令(伪指令，堆上分配内存)
	///		new
	SHORT = 1<<8
} DirectiveOpcode;
typedef enum {
	DOPT_None = 0,
	/// <summary>
	/// 8bits指令
	/// bits(WORD) = 8 , bits(POINTER)=16
	/// </summary>
	DOPT_Bits_08 = 0,
	/// <summary>
	/// 16bits指令
	/// bits(WORD) = 16 , bits(POINTER)=32
	/// </summary>
	DOPT_Bits_16 = 1,
	/// <summary>
	/// 32bits指令
	/// bits(WORD) = 32 , bits(POINTER)=32
	/// </summary>
	DOPT_Bits_32 = 2,
	/// <summary>
	/// 64bits指令
	/// bits(WORD) = 64 , bits(POINTER)=64
	/// </summary>
	DOPT_Bits_64 = 3,
	DOPT_Bits = DOPT_Bits_64,
	/// <summary>
	/// 是否是常用数操作,操作数长度定义
	/// bits[3] =  00 00
	/// </summary>
	// OP_NumberSize = 1<<2,
	/// 0 is double 是否是双字操作，操作数长度定义
	/// bits[3] = 01 00
	DOPT_OperandBits_Word = 1 << 2,
	/// 0 is double 是否是双字操作，操作数长度定义
	/// bits[4] 10 00
	DOPT_OperandBits_DWord = 1 << 3,
	/// <summary>
	/// 
	///  bits[4] 11 00
	/// </summary>
	DOPT_OperandBits_Pointer = DOPT_OperandBits_Word | DOPT_OperandBits_Word,
	DOPT_OperandBits = DOPT_OperandBits_Pointer,
	/// <summary>
	/// 操作数数量: 1个操作数
	/// 00 01 00 00
	/// </summary>
	DOPT_OperandCount_1 = 1 << 5,
	/// <summary>
	/// 操作数数量: 2个操作数
	/// 00 10 00 00
	/// </summary>
	DOPT_OperandCount_2= 1 << 6,
	/// <summary>
	/// 操作数数量:1个常量
	/// 00 11 00 00
	/// </summary>
	DOPT_OperandCount_Immediate = DOPT_OperandCount_1 | DOPT_OperandCount_2,
	/// <summary>
	/// 操作数数量
	/// 00 ?? 00 00
	/// </summary>
	DOPT_OperandCount = DOPT_OperandCount_Immediate,
	/// <summary>
	/// 寻址方式: 堆栈
	/// 01 00 00 00
	/// </summary>
	DOPT_Address_Stack = 1 << 7,
	/// <summary>
	/// 寻址方式: 间接/指针
	/// 10 00 00 00
	/// </summary>
	DOPT_Address_Pointer = 1 << 8,
	/// <summary>
	/// 寻址方式 
	/// 11 00 00 00
	/// </summary>
	DOPT_Address = DOPT_Address_Stack | DOPT_Address_Pointer

	
} DirectiveOptions;
/// <summary>
/// 指令码
/// </summary>
typedef struct TDirective {
	/// <summary>
	/// 操作码
	/// DirectiveOperations
	/// </summary>
	char opcode;

	/// <summary>
	/// directive的参数，DirectiveOptions
	/// </summary>
	char options;
	
	
} Directive;

/// <summary>
/// 指令结构
/// </summary>
typedef union {
	struct TDirective;
	struct {
		Directive direactive;
		union {
			struct {
				char operand1;
				char opernad2;
			};
			struct {
				char word1;
				char word2;
			};
			struct {
				short dword1;
				short dword2;
			};
			struct {
				short pointer1;
				short pointer2;
			};
		};
	} bits08;
	struct {
		Directive direactive;
		union {
			struct {
				short operand1;
				short opernad2;
			};
			struct {
				short word1;
				short word2;
			};
			struct {
				int dword1;
				int dword2;
			};
			struct {
				int pointer1;
				int pointer2;
			};
		};
	} bits16;
	struct {
		Directive direactive;
		union {
			struct {
				int operand1;
				int opernad2;
			};
			struct {
				int word1;
				int word2;
			};
			struct {
				long dword1;
				long dword2;
			};
			struct {
				int pointer1;
				int pointer2;
			};
		};
	} bits32;
	struct {
		Directive direactive;
		union {
			struct {
				int operand1;
				int opernad2;
			};
			struct {
				long word1;
				long word2;
			};
			struct {
				long long dword1;
				long long dword2;
			};
			struct {
				long pointer1;
				long pointer2;
			};
		};
	} bits64;

} Instruction;

typedef union TCommonRegister {
	union {
		struct {
			BIT08 r0;
			BIT08 r1;
			BIT08 r2;
			BIT08 r3;
		};
		BIT08 rs[4];
	}bits08;

	union {
		struct {
			BIT16 r0;
			BIT16 r1;
			BIT16 r2;
			BIT16 r3;
		};
		BIT16 rs[4];
	}bits16;
	union {
		struct {
			BIT32 r0;
			BIT32 r1;
			BIT32 r2;
			BIT32 r3;
		};
		BIT16 crs[4];
	}bits32;
	union {
		struct {
			BIT64 r0;
			BIT64 r1;
			BIT64 r2;
			BIT64 r3;
		};
		BIT16 rs[4];
	}bits64;
} CommonRegister;

typedef struct TMMU {
	HOST_DWORD* (*read)(HOST_ADDR vAddress, int size, HOST_WORD sectionDescriptor);
	void (*write)(HOST_ADDR vAddress, HOST_DWORD value, int size, HOST_WORD sectionDescriptor);
	void (*lock)(HOST_ADDR vAddress, HOST_WORD sectionDescriptor);
	void (*unlock)(HOST_ADDR vAddress, HOST_WORD sectionDescriptor);
} MMU;




/// <summary>
/// 中断处理函数
/// </summary>
typedef int* Interrupt(int index);


/// <summary>
/// 机器结构
/// </summary>

typedef struct TMachine {
	/// <summary>
	/// 通用寄存器(Common Register)栈
	/// 4个WORD的深度
	/// </summary>
	union TCommonRegister;
	
	/// <summary>
	/// 堆栈(Stack Register)寄存器
	/// </summary>
	BIT08* sp;

	/// <summary>
	/// section 段寄存器
	/// 用户进程隔离
	/// </summary>
	HOST_WORD scp;

	/// <summary>
	/// 程序(Program Register)寄存器
	/// 当前指令的地址
	/// </summary>
	BIT08* pr;
	/// <summary>
	/// 指令寄存器
	/// </summary>
	Instruction ir;
	/// <summary>
	/// 中断向量表入口寄存器
	/// </summary>
	Interrupt* ivr;
	/// <summary>
	/// 内存管理单元
	/// </summary>
	MMU* mmu;
	
} Machine;

typedef struct {
	const char* file;
	/// <summary>
	/// 内存页大小
	/// </summary>
	int memoryPageSize;
	/// <summary>
	/// 最大内存页数
	/// </summary>
	int memoryMaxPageCount;
	/// <summary>
	/// 起始时分配的内存页数
	/// </summary>
	int memoryStartPageCount;
	int stackSize;
} MachineCreationOptions;

Machine* createMachine(const MachineCreationOptions* opts);

void destroyMachine(Machine* vm);

int getDirectiveSize(Instruction* instruction);



void printDirective(Instruction* instruction);
