#ifndef NWI_108A1
#define NWI_108A1

#include <stdio.h>
#include <string.h>
#include "common.h"

// DLL function prefix
#define DLL_EXPORT
#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

// Main
#define BUS_WIDTH	8
#define MEM_SIZE	1024
#define MEM_TYPE	1

// Register
// General Purpose Registers
#define R0		0x00
#define R1		0x01
#define R2		0x02
#define R3		0x03

// Timer - T0
#define T0		0x10
#define TCON0	0x11
#define TCMPA0	0x12
#define TCMPB0	0x13

// UART - UA0
#define TX0		0x18
#define RX0		0x19
#define UCONTA0	0x1A
#define UCONTB0	0x1B
#define BAUD0	0x1C
#define USTAT0	0x1D

// IO Bank - IO0
#define PIN0	0x24
#define DIR0	0x25
#define IRE0	0x26
#define IR_TRG0	0x27

// Instructions
#define NOP		0x00
#define RET		0x01
#define COPY	0x02
#define JEZR	0x03

#define LDA		0x04
#define STA		0x05
#define JMP		0x06
#define CALL	0x07

#define ADD		0x08
#define SUB		0x09
#define AND		0x0A
#define OR		0x0B
#define XOR		0x0C
#define SLL		0x0D
#define SRL		0x0E
#define NEG		0x0F

typedef struct var {
	unsigned int adr, index;
	unsigned char size, slot_pos;
	char name[32];
	int val;
} var_t;

// Compile command
DECLDIR int sys_cmpl(char *cmd, var_t *vars, unsigned int var_num, char *params[], unsigned int param_num, unsigned int *cmpl, unsigned int *cmpl_num);
// System data
DECLDIR int sys_bus();
DECLDIR int sys_size();
DECLDIR int sys_mem();

static int isVariableValid(var_t *vars, unsigned int var_num, char *variable);

#endif