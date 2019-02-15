#ifndef BGS
#define BGS

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "NWI_Tools.h"

// DLL function prefix
#define DLL_EXPORT
#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

// Main
#define BUS_WIDTH		32
#define BUS_WIDTH_BYTE	(BUS_WIDTH / 8)
#define MEM_SIZE		1024
#define MEM_TYPE		HARVARD

// Memory partitions
#define ADR_IO			16
#define ADR_SRAM		96
#define ADR_STACK		192

// Registers
// General Purpose Registers
#define R0		0x00
#define R1		0x01
#define R2		0x02
#define R3		0x03
#define R4		0x04
#define R5		0x05
#define R6		0x06
#define R7		0x07
#define R8		0x08
#define R9		0x09
#define R10		0x0A
#define R11		0x0B
#define R12		0x0C
#define R13		0x0D
#define R14		0x0E
#define R15		0x0F

// Timer - T0
#define T0		0x10
#define TCON0	0x11
#define TCMPA0	0x12
#define TCMPB0	0x13

// IO Bank - IO0
#define PIN0	0x24
#define DIR0	0x25
#define IRE0	0x26
#define IR_TRG0	0x27

// UART - UA0
#define TX0		0x18
#define RX0		0x19
#define UCONTA0	0x1A
#define UCONTB0	0x1B
#define BAUD0	0x1C
#define USTAT0	0x1D

// Instructions
#define NOP		0x000
#define MOV		0x010
#define LEA		0x020
#define PUSH	0x030
#define POP		0x040
#define CMP		0x050
#define JMP		0x060
#define JE		0x061
#define JNE		0x062
#define JZ		0x063
#define JNZ		0x064
#define JG		0x065
#define JGE		0x066
#define JL		0x067
#define JLE		0x068
#define JC		0x069
#define JNC		0x06A
#define JO		0x06B
#define JNO		0x06C
#define JS		0x06D
#define JNS		0x06E
#define CALL	0x070
#define RET		0x080
#define STI		0x090
#define CLI		0x0A0

#define ADD		0x200
#define ADC		0x210
#define SUB		0x220
#define MUL		0x230
#define DIV		0x240
#define IDIV	0x250
#define INC		0x260
#define DEC		0x270

#define TEST	0x400
#define NOT		0x410
#define NEG		0x420
#define AND		0x430
#define OR		0x440
#define XOR		0x450
#define SHL		0x460
#define SHR		0x470
#define ROL		0x480
#define ROR		0x490

#define FADD	0x600
#define FSUB	0x610
#define FMUL	0x620
#define FDIV	0x630

// Instruction groups
#define GRP_MANG	0x0 // Managment group
#define GRP_ALU_A	0x2 // ALU Arithmetic group
#define GRP_ALU_L	0x4 // ALU Logical group
#define GRP_FPU		0x6 // FPU group

// Data sizes
#define DB		1
#define DW		2
#define DD		4
#define DQ		8
#define DT		10
#define DO		16

// Flags
#define LONG	(0x1 << 16)
#define ALU		(0x2 << 16)
#define FPU		(0x4 << 16)
#define DSP		(0x8 << 16)

// Instruction managment
#define OP(inst)				(inst >> 4)
#define SET_SUBOP(cmd, SUBOP)	cmd |= (SUBOP << 20)
#define SET_SIZE(cmd, SIZE)		cmd |= ((SIZE << 12) & 0xF000)

#define MAX_PARAMS	3

// Compile command
DECLDIR int sys_cmpl(char *cmd, var_t *vars, unsigned int var_num, char *params[], unsigned int param_num, unsigned int *cmpl, unsigned int *cmpl_num);
// System data
DECLDIR int sys_bus(void);
DECLDIR int sys_size(void);
DECLDIR int sys_mem(void);
DECLDIR int sys_mem_offset(void);

static int isVariableValid(var_t *vars, unsigned int var_num, char *variable);

#endif // !BGS
