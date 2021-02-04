#pragma once
#include "List.h"

/// <summary>
/// 运算核
/// </summary>
struct TCore;
/// <summary>
/// 内存管理单元
/// </summary>
struct TMMU;
/// <summary>
/// 中断管理单元
/// </summary>
struct IMU;
/// <summary>
/// 机器结构
/// </summary>
struct TMachine;

/// <summary>
/// 机器字
/// </summary>
typedef union TBITS8 {
	char value;
	char bytes[1];

} BYTE1;
typedef BYTE1 BYTE;

typedef union TBITS16 {
	short value;
	char bytes[2];
} BYTE2;
typedef BYTE2 SHORT;
/// <summary>
/// 常用数
/// </summary>
typedef union TBITS32 {
	int value;
	char bytes[4];
} BYTE4;
typedef BYTE4 INT;

/// <summary>
/// 常用数
/// </summary>
typedef union TBITS64 {
	long value;
	char bytes[8];
} BYTE8;
typedef BYTE8 LONG;

/// <summary>
/// 常用数
/// </summary>
typedef union TBITS128 {
	long long value;
	char bytes[16];
} BYTE16;
typedef BYTE16 LARGE;

typedef struct TADDR02 {
	unsigned short value;
	char bytes[2];
} ADDR16;

typedef struct TADDR04 {
	unsigned int value;
	char bytes[4];
} ADDR32;

typedef struct TADDR08 {
	unsigned int value;
	char bytes[8];
} ADDR64;

typedef union TADDR {
	ADDR16 bits16;
	ADDR32 bits32;
	ADDR64 bits64;
} ADDR;

typedef int HOST_WORD;
typedef int HOST_DWORD;
typedef char* HOST_ADDR;

typedef struct TMMU {
	struct TMachine* machine;
} MMU;


typedef struct TMMU016 {
	struct MMU;
	BYTE1(*readByte1)(ADDR16 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte1)(ADDR16 vAddress, BYTE1 value, ADDR16 sectionDescriptor);
	BYTE2(*readByte2)(ADDR16 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte2)(ADDR16 vAddress, BYTE2 value, ADDR16 sectionDescriptor);
	BYTE4(*readByte4)(ADDR16 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte4)(ADDR16 vAddress, BYTE4 value, ADDR16 sectionDescriptor);
	BYTE8(*readByte8)(ADDR16 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte8)(ADDR16 vAddress, BYTE8 value, ADDR16 sectionDescriptor);
	BYTE16(*readByte16)(ADDR16 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte16)(ADDR16 vAddress, BYTE16 value, ADDR16 sectionDescriptor);
	void (*ASSIGN)(ADDR16 leftAddress, HOST_ADDR rightAddress, ADDR16 sectionDescriptor);
	ADDR16(*NEW)(size_t size, ADDR16 sectionDescriptor);

	void (*lock)(ADDR16 vAddress, BYTE2 assertValue, BYTE2 newValue, ADDR16 sectionDescriptor);
} MMU016;

typedef struct TMMU016 {
	struct MMU;
	BYTE1(*readByte1)(ADDR32 vAddress, ADDR32 sectionDescriptor);
	void (*writeByte1)(ADDR32 vAddress, BYTE1 value, ADDR32 sectionDescriptor);
	BYTE2(*readByte2)(ADDR32 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte2)(ADDR32 vAddress, BYTE2 value, ADDR32 sectionDescriptor);
	BYTE4(*readByte4)(ADDR32 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte4)(ADDR32 vAddress, BYTE4 value, ADDR32 sectionDescriptor);
	BYTE8(*readByte8)(ADDR32 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte8)(ADDR32 vAddress, BYTE8 value, ADDR32 sectionDescriptor);
	BYTE16(*readByte16)(ADDR32 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte16)(ADDR32 vAddress, BYTE16 value, ADDR32 sectionDescriptor);
	void (*ASSIGN)(ADDR32 leftAddress, HOST_ADDR rightAddress, ADDR32 sectionDescriptor);
	ADDR32(*NEW)(size_t size, ADDR32 sectionDescriptor);

	void (*lock)(ADDR32 vAddress, BYTE4 assertValue, BYTE4 newValue, ADDR32 sectionDescriptor);
} MMU032;

typedef struct TMMU064 {
	struct MMU;
	BYTE1(*readByte1)(ADDR64 vAddress, ADDR32 sectionDescriptor);
	void (*writeByte1)(ADDR64 vAddress, BYTE1 value, ADDR64 sectionDescriptor);
	BYTE2(*readByte2)(ADDR64 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte2)(ADDR64 vAddress, BYTE2 value, ADDR64 sectionDescriptor);
	BYTE4(*readByte4)(ADDR64 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte4)(ADDR64 vAddress, BYTE4 value, ADDR64 sectionDescriptor);
	BYTE8(*readByte8)(ADDR64 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte8)(ADDR64 vAddress, BYTE8 value, ADDR64 sectionDescriptor);
	BYTE16(*readByte16)(ADDR64 vAddress, ADDR16 sectionDescriptor);
	void (*writeByte16)(ADDR64 vAddress, BYTE16 value, ADDR64 sectionDescriptor);
	void (*ASSIGN)(ADDR64 leftAddress, HOST_ADDR rightAddress, ADDR64 sectionDescriptor);
	ADDR64(*NEW)(size_t size, ADDR64 sectionDescriptor);

	void (*lock)(ADDR64 vAddress, BYTE16 assertValue, BYTE16 newValue, ADDR64 sectionDescriptor);
} MMU032;



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
	/// ADDR = 7,
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
	//SHORT = 1<<8
} DirectiveOpcode;
typedef enum {
	DOPT_None = 0,
	/// <summary>
	/// 8bits指令
	/// bits(WORD) = 8 , bits(POINTER)=16
	/// 00 00 00 00
	/// </summary>
	DOPT_Bits_08 = 0,
	/// <summary>
	/// 16bits指令
	/// bits(WORD) = 16 , bits(POINTER)=32
	/// 00 00 00 01
	/// </summary>
	DOPT_Bits_16 = 1,
	/// <summary>
	/// 32bits指令
	/// bits(WORD) = 32 , bits(POINTER)=32
	/// 00 00 00 10
	/// </summary>
	DOPT_Bits_32 = 2,
	/// <summary>
	/// 64bits指令
	/// bits(WORD) = 64 , bits(POINTER)=64
	/// 00 00 00 11
	/// </summary>
	DOPT_Bits_64 = 3,
	DOPT_Bits = DOPT_Bits_64,
	/// <summary>
	/// 是否是常用数操作
	/// bits[3] = 00 00 01 00
	/// </summary>
	DOPT_OperandBits_Operand = 0,
	/// <summary>
	/// 是否是常用数操作,操作数长度定义
	/// bits[3] = 00 00 01 00
	/// </summary>
	DOPT_OperandBits_Word = 1<<2,
	/// 0 is double 是否是双字操作，操作数长度定义
	/// bits[4] 00 00 10 00
	DOPT_OperandBits_DWord = 1 << 3,
	/// <summary>
	///  操作数是指针
	///  bits[4] 00 00 11 00
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
				BYTE1 operand1;
				BYTE1 opernad2;
			};
			struct {
				BYTE1 word1;
				BYTE1 word2;
			};
			struct {
				BYTE2 dword1;
				BYTE2 dword2;
			};
			struct {
				ADDR16 pointer1;
				ADDR16 pointer2;
			};
		};
	} bits08;
	struct {
		Directive direactive;
		union {
			struct {
				BYTE2 operand1;
				BYTE2 opernad2;
			};
			struct {
				BYTE2 word1;
				BYTE2 word2;
			};
			struct {
				BYTE4 dword1;
				BYTE4 dword2;
			};
			struct {
				ADDR32 pointer1;
				ADDR32 pointer2;
			};
		};
	} bits16;
	struct {
		Directive direactive;
		union {
			struct {
				BYTE4 operand1;
				BYTE4 opernad2;
			};
			struct {
				BYTE4 word1;
				BYTE4 word2;
			};
			struct {
				BYTE8 dword1;
				BYTE8 dword2;
			};
			struct {
				ADDR32 pointer1;
				ADDR32 pointer2;
			};
		};
	} bits32;
	struct {
		Directive direactive;
		union {
			struct {
				BYTE4 operand1;
				BYTE4 opernad2;
			};
			struct {
				BYTE8 word1;
				BYTE8 word2;
			};
			struct {
				BYTE16 dword1;
				BYTE16 dword2;
			};
			struct {
				ADDR64 pointer1;
				ADDR64 pointer2;
			};
		};
	} bits64;

} Instruction;

typedef union {
	BYTE1 rs[8];
	struct {
		BYTE1 r0;
		BYTE1 r1;
		BYTE1 r2;
		BYTE1 r3;
		BYTE1 r4;
		BYTE1 r5;
		BYTE1 r6;
		BYTE1 r7;
	};
	BYTE2 drs[4];
	struct {
		BYTE2 dr0;
		BYTE2 dr1;
		BYTE2 dr3;
		BYTE2 dr4;
	};
	ADDR16 ars[4];
	struct {
		ADDR16 ar0;
		ADDR16 ar1;
	};
} CommonRegister1;

typedef union {
	BYTE2 rs[8];
	struct {
		BYTE2 r0;
		BYTE2 r1;
		BYTE2 r2;
		BYTE2 r3;
		BYTE2 r4;
		BYTE2 r5;
		BYTE2 r6;
		BYTE2 r7;
	};
	BYTE4 drs[4];
	struct {
		BYTE4 dr0;
		BYTE4 dr1;
		BYTE4 dr3;
		BYTE4 dr4;
	};
	ADDR32 ars[2];
	struct {
		ADDR32 ar0;
		ADDR32 ar1;
	};
} CommonRegister2;

typedef union {
	BYTE4 rs[8];
	struct {
		BYTE4 r0;
		BYTE4 r1;
		BYTE4 r2;
		BYTE4 r3;
		BYTE4 r4;
		BYTE4 r5;
		BYTE4 r6;
		BYTE4 r7;
	};
	BYTE8 drs[4];
	struct {
		BYTE8 dr0;
		BYTE8 dr1;
		BYTE8 dr3;
		BYTE8 dr4;
	};
	ADDR32 ars[2];
	struct {
		ADDR32 ar0;
		ADDR32 ar1;
	};
} CommonRegister4;

typedef union {
	BYTE8 rs[8];
	struct {
		BYTE8 r0;
		BYTE8 r1;
		BYTE8 r2;
		BYTE8 r3;
		BYTE8 r4;
		BYTE8 r5;
		BYTE8 r6;
		BYTE8 r7;
	};
	BYTE16 drs[4];
	struct {
		BYTE16 dr0;
		BYTE16 dr1;
		BYTE16 dr3;
		BYTE16 dr4;
	};
	ADDR64 ars[2];
	struct {
		ADDR64 ar0;
		ADDR64 ar1;
	};
} CommonRegister8;

typedef union TCommonRegister {
	CommonRegister1 bits8;
	CommonRegister2 bits16;
	CommonRegister4 bits32;
	CommonRegister8 bits64;
} CommonRegister;


/// <summary>
/// 中断处理函数
/// </summary>
typedef int* Interrupt(int index);


/// <summary>
/// 机器结构
/// </summary>
struct TMachine;
typedef struct TCore {
	/// <summary>
	/// 通用寄存器(Common Register)栈
	/// 4个WORD的深度
	/// </summary>
	union TCommonRegister;
	
	
	/// <summary>
	/// 堆栈(Stack Register)寄存器
	/// </summary>
	ADDR sp;

	/// <summary>
	/// section 段寄存器
	/// 用户进程隔离
	/// </summary>
	ADDR scp;
	/// <summary>
	/// 程序(Program Register)寄存器
	/// 当前指令的地址
	/// </summary>
	ADDR pr;
	/// <summary>
	/// 指令寄存器
	/// </summary>
	Instruction ir;
	
	struct TMachine* machine;
	
} Core;

typedef int (*InstructionExecutor)(Core* core,Instruction* instruction);

typedef struct TMachine{
	/// <summary>
	/// 中断向量表入口寄存器
	/// </summary>
	Interrupt* ivr;
	
	/// <summary>
	/// 内核
	/// </summary>
	List cores;

	/// <summary>
	/// 内存管理单元
	/// 可以挂接不同的内存管理模块
	/// </summary>
	MMU* mmu;
	/// <summary>
	/// 可编程扩展的指令集
	/// </summary>
	List instructions;
}Machine;


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
	/// 核心数量
	/// </summary>
	int coreCount;
} MachineCreationOptions;

Machine* Machine_construct(Machine*  self,const MachineCreationOptions* opts);

void Machine_destruct(Machine* vm);

int getDirectiveSize(Instruction* instruction);



void printDirective(Instruction* instruction);
