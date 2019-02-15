#include "BGS.h"

// Instructions
const char *insts[] = { "nop", "mov", "lea", "push", "pop", "cmp",
	"jmp", "je", "jne", "jz", "jnz", "jg", "jge", "jl", "jle", "jc", "jnc", "jo", "jno", "js", "jns",
	"call", "ret", "sti", "cli", "add", "sub", "mul", "div", "div", "idiv", "inc", "dec",
	"test", "not", "neg", "and", "or", "xor", "shl", "shr", "rol", "ror",
	"fadd", "fsub", "fmul", "fdiv", NULL };

const unsigned int inst_ops[] = { NOP, MOV, LEA, PUSH, POP, CMP,
	JMP, JE, JNE, JZ, JNZ, JG, JGE, JL, JLE, JC, JNC, JO, JNO, JS, JNS,
	CALL, RET, STI, CLI, ADD, ADC, SUB, MUL, DIV, IDIV, INC, DEC,
	TEST, NOT, NEG, AND, OR, XOR, SHL, SHR, ROL, ROR,
	FADD, FSUB, FMUL, FDIV };

const char *regs[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
	"t0", "tcon0", "tcmpa0", "tcmpb0", "tx0", "rx0", "uconta0", "ucontb0", "baud0",
	"ustat0", "pin0", "dir0", "ire0", "ir_trg0", NULL };

const unsigned int reg_addrs[] = { R0, R1, R2, R3, R4, R5, R6, R7,
	T0, TCON0, TCMPA0, TCMPB0, TX0, RX0, UCONTA0, UCONTB0, BAUD0,
	USTAT0, PIN0, DIR0, IRE0, IR_TRG0 };

DECLDIR int sys_cmpl(char *cmd, var_t *vars, unsigned int var_num, char *params[], unsigned int param_num, unsigned int *cmpl, unsigned int *cmpl_num) {
	unsigned int i = 0, j = 0, cmdCmpl, parCmpl[8];
	int cmdIndex, paramIndex, varIndex;
	char cmdOk = 0, isPnt = 0, paramsOk = 0, memIndex = -1, conIndex = -1;
	// Return if too many parameters were supplied
	if (param_num > MAX_PARAMS) return ERR_INC_PARAM_NUM;
	// Check if the command is in the command list
	cmdIndex = getStringIndex(cmd, insts);
	// Return if the command is the list
	if (cmdIndex <= -1) return ERR_UNKNOWN_CMD;
	// Set compiled command
	cmdCmpl = inst_ops[cmdIndex];
	// Set default command length
	*cmpl_num = 1;
	// Return if only the command length was requested
	//if (cmpl == NULL) return 0;
	// Add OP code and sub OP code
	if (cmpl != NULL) {
		*cmpl = 0;
		SET_SUBOP(*cmpl, cmdCmpl);
	}
	// Compile parameters
	for (i = 0; i < param_num; i++) {
		// Check if the parameter is a pointer
		isPnt = isPointer(params[i]);
		if (isPnt <= -1) {
			return ERR_UNKNOWN_PARAM;
		}

		// Check if the parameter is a register
		paramIndex = getStringIndex(params[i], regs);
		if (paramIndex > -1 && !isPnt) {
			parCmpl[i] = reg_addrs[paramIndex];
			continue;
		}

		// Set number of compiled words
		*cmpl_num = 2;
		// Continue if only the command length was requested
		if (cmpl == NULL) continue;

		// Check if parameter is a variable
		varIndex = isVariableValid(vars, var_num, params[i]);
		// Clear compiled parameter
		parCmpl[i] = 0;
		// Set long instruciton flag
		*cmpl |= LONG;

		if (varIndex <= -1) {
			// Handle constants
			// Return if this isn't the first constant or memory parameter
			if (conIndex > -1 || memIndex > -1) return ERR_MULTI_CON_VAR;
			conIndex = i;
			if (isPnt) {
				if (paramIndex <= -1) {
					return ERR_UNKNOWN_PARAM;
				}
				*(cmpl + 1) = reg_addrs[paramIndex];
			}
			else {
				// Check if parameter is a number in HEX or DEC or BIN
				if (!strToNum(params[i], (cmpl + 1))) {
					return ERR_UNKNOWN_PARAM;
				}
			}
		}
		else {
			// Handle Variables
			// Return if this isn't the first memory or constant parameter
			if (memIndex > -1 || conIndex > -1) return ERR_MULTI_CON_VAR;
			// Set variable value for the next byte
			*(cmpl + 1) = (vars[varIndex].adr * BUS_WIDTH_BYTE) + vars[varIndex].slot_pos;

			if (!isPnt) {
				memIndex = i;
				// Set variable length
				SET_SIZE(*cmpl, vars[varIndex].size);
			}
			else {
				conIndex = i;
			}
		}
	}
	// Return if only the command length was requested
	if (cmpl == NULL) return ERR_OK;
	// Set unit flags
	// ALU arithmetical and logical instructions
	if (cmdCmpl >= 0x200 && cmdCmpl < 0x600) {
		// Set flag
		*cmpl |= ALU;
	}
	// FPU floating point instructions
	else if (cmdCmpl >= 0x600 && cmdCmpl < 0x800) {
		// Set flag
		*cmpl |= FPU;
	}
	// Add compiled parameters to instruction
	if (*cmpl & ALU && param_num < 3 && param_num > 0) {
		*cmpl |= parCmpl[0] << ((MAX_PARAMS - 1) * 4);
		*cmpl |= parCmpl[0] << ((MAX_PARAMS - 2) * 4);
		if (param_num > 1) {
			*cmpl |= parCmpl[1];
		}
	}
	else {
		for (i = 0; i < param_num; i++) *cmpl |= parCmpl[i] << ((MAX_PARAMS - 1 - i) * 4);
	}
	// Compile command
	switch (cmdCmpl >> 4) {
	case OP(NOP)://NOP
		if (param_num != 0) return ERR_INC_PARAM_NUM;
		break;
	case OP(MOV):// MOV
		if (param_num != 2) return ERR_INC_PARAM_NUM;
		if (conIndex <= -1 && memIndex <= -1) {
			SET_SUBOP(*cmpl, 0x00);
		}
		else if (conIndex == 1) {
			SET_SUBOP(*cmpl, 0x01);
		}
		else if (memIndex == 1) {
			SET_SUBOP(*cmpl, 0x02);
		}
		else if (memIndex == 0) {
			SET_SUBOP(*cmpl, 0x03);
		}
		else {
			return ERR_UNKNOWN_PARAM;
		}
		break;
	case OP(LEA):// LEA
		if (param_num != 2) return ERR_INC_PARAM_NUM;
		break;
	case OP(PUSH):// PUSH
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		// Add sub-op code
		if (memIndex > -1) SET_SUBOP(*cmpl, 0x01);
		break;
	case OP(POP):// POP
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		if (memIndex > -1) SET_SUBOP(*cmpl, 0x01);
		break;
	case OP(CMP):// CMP
		if (param_num != 2) return ERR_INC_PARAM_NUM;
		if (memIndex > -1) SET_SUBOP(*cmpl, 0x01);
		break;
	case OP(JMP):// JMP
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		break;
	case OP(CALL):// CALL
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		if (memIndex > -1) SET_SUBOP(*cmpl, 0x01);
		break;
	case OP(RET):// RET
		if (param_num != 0) return ERR_INC_PARAM_NUM;
		break;
	case OP(STI):// STI
		if (param_num != 0) return ERR_INC_PARAM_NUM;
		break;
	case OP(CLI):// CLI
		if (param_num != 0) return ERR_INC_PARAM_NUM;
		break;
	case OP(INC):
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		break;
	case OP(DEC):
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		break;
	case OP(NOT):
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		break;
	case OP(NEG):
		if (param_num != 1) return ERR_INC_PARAM_NUM;
		break;
	default:
		// Return if too little parameters were supplied
		if (param_num < 1) return ERR_INC_PARAM_NUM;
		// Set sub-op
		if (param_num == 2 && conIndex <= -1 && memIndex <= -1) {
			SET_SUBOP(*cmpl, 0x00);
		}
		else if (param_num == 3 && conIndex <= -1 && memIndex <= -1) {
			SET_SUBOP(*cmpl, 0x01);
		}
		else if (param_num == 2 && conIndex > -1) {
			// SHL SHR ROL ROR
			/*if (cmdCmpl >= 0x46 && cmdCmpl < 0x50 && *(cmpl + 1) < 0xFF) {
				*cmpl |= *(cmpl + 1);
				*cmpl_num = 1;
				SET_SUBOP(*cmpl, 0x06);
			}
			else {
				SET_SUBOP(*cmpl, 0x02);
			}*/
			SET_SUBOP(*cmpl, 0x02);
		}
		else if (param_num == 3 && conIndex > -1) {
			SET_SUBOP(*cmpl, 0x03);
		}
		else if (param_num == 2 && memIndex > -1) {
			SET_SUBOP(*cmpl, 0x04);
		}
		else if (param_num == 3 && memIndex > -1) {
			SET_SUBOP(*cmpl, 0x05);
		}
		else {
			// Unknown parameter configuration
			return ERR_UNKNOWN_PARAM;
		}
		break;
	}
	return ERR_OK;
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

DECLDIR int sys_mem_offset(){
	return ADR_SRAM;
}

static int isVariableValid(var_t * vars, unsigned int var_num, char * variable) {
	unsigned int i;
	for (i = 0; i < var_num; i++) {
		if (strcmp(vars[i].name, variable) == 0) return i;
	}
	return -1;
}