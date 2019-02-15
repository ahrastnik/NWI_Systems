#include "NWI_108A1.h"

// Instructions
const char *insts[] = { "nop", "ret", "copy", "jezr", "lda", "sta", "jmp", "call",
	"add", "sub", "and", "or", "xor", "sll", "srl", "neg", NULL };

const unsigned int *inst_ops[] = { NOP, RET, COPY, JEZR, LDA, STA, JMP, CALL,
	ADD, SUB, AND, OR, XOR, SLL, SRL, NEG };

const char *regs[] = { "r0", "r1", "r2", "r3", "t0", "tcon0", "tcmpa0", "tcmpb0",
	"tx0", "rx0", "uconta0", "ucontb0", "baud0", "ustat0",
	"pin0", "dir0", "ire0", "ir_trg0", NULL };

const unsigned int *reg_addrs[] = { R0, R1, R2, R3, T0, TCON0, TCMPA0, TCMPB0,
	TX0, RX0, UCONTA0, UCONTB0, BAUD0, USTAT0, PIN0, DIR0, IRE0, IR_TRG0 };

DECLDIR int sys_cmpl(char *cmd, var_t *vars, unsigned int var_num, char *params[], unsigned int param_num, unsigned int *cmpl, unsigned int *cmpl_num) {
	unsigned int i = 0, j = 0, cmdIndex, paramIndex, cmdCmpl, parCmpl[8];
	char cmdOk = 0, paramsOk = 0;
	// Check if the command is in the command list
	cmdIndex = getStringIndex(cmd, insts);
	cmdCmpl = inst_ops[cmdIndex];
	// Return if the command is the list
	if (cmdIndex == -1) return 3;
	// Set command length
	if (cmdIndex == 4 || cmdIndex == 5 || cmdIndex == 6 || cmdIndex == 7) *cmpl_num = 2;
	else *cmpl_num = 1;
	// Return if only the command length was requested
	if (cmpl == NULL) return 0;
	// Compile parameters
	for (i = 0; i < param_num; i++) {
		// Check if parameter is a register
		paramIndex = getStringIndex(params[i], regs);
		parCmpl[i] = reg_addrs[paramIndex];
		if (paramIndex != -1) continue;
		// Check if parameter is a number in HEX or DEC or BIN
		if (!strToNum(params[i], &parCmpl[i])) return 4;
	}
	// Init the compiled command array
	*cmpl = 0;
	cmpl[1] = 0;
	// Compile Command
	if (cmdIndex < 8) {
		switch (cmdIndex) {
		case 0:// NOP
			if (param_num != 0) return 4;
			break;
		case 1:// RET
			if (param_num != 0) return 4;
			break;
		case 2:// COPY
			if (param_num != 2) return 4;
			*cmpl = (parCmpl[0] << 2) | parCmpl[1];
			break;
		case 3:// JEZR
			if (param_num != 1) return 4;
			*cmpl = parCmpl[0];
			break;
		case 4:// LDA
			if (param_num != 2) return 4;
			*cmpl_num = 2;
			*cmpl = parCmpl[0] << 2;
			*cmpl |= 0x01;// Set storage type to RAM
			cmpl[1] = parCmpl[1];
			break;
		case 5:// STA
			if (param_num != 2) return 4;
			*cmpl_num = 2;
			*cmpl = parCmpl[0];
			*cmpl |= 0x04;// Set storage type to RAM
			cmpl[1] = parCmpl[1];
			break;
		case 6:// JMP
			if (param_num != 1) return 4;
			*cmpl_num = 2;
			cmpl[1] = parCmpl[0];
			break;
		case 7:// CALL
			if (param_num != 1) return 4;
			*cmpl_num = 2;
			cmpl[1] = parCmpl[0];
			break;
		default:
			break;
		}
	}
	else {
		// Logical Arithmetic Instructions
		if (param_num != 2) return 4;
		*cmpl = (parCmpl[0] << 2) | parCmpl[1];
	}

	// Add instruction code
	*cmpl |= cmdCmpl << 4;

	return 0;
}

DECLDIR int sys_bus() {
	return BUS_WIDTH;
}

DECLDIR int sys_size() {
	return MEM_SIZE;
}

DECLDIR int sys_mem() {
	return MEM_TYPE;
}

static int isVariableValid(var_t * vars, unsigned int var_num, char * variable) {
	unsigned int i;
	for (i = 0; i < var_num; i++) {
		if (strcmp(vars[i].name, variable) == 0) return i;
	}
	return -1;
}