#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000
#define OPCODE 22
#define MASK 0x0000FFFF
#define REGA 19
#define REGB 16
#define MASK_REG 0x00000007

typedef struct stateStruct {
	int pc;
	int mem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
} stateType;

enum opCode {
	ADD = 0,
	NOR,
	LW,
	SW,
	BEQ,
	JALR,
	HALT,
	NOOP
};

void printState(stateType *statePtr);
int convertNum(int num);

int
main(int argc, char *argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	FILE *filePtr;
	int size, i = 0;

	if (argc != 2) {
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
	}

	filePtr = fopen(argv[1], "r");
	if (filePtr == NULL) {
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
	}

	/* read in the entire machine-code file into memory */
	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
		state.numMemory++) {

		if (sscanf(line, "%d", state.mem + state.numMemory) != 1) {
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
	}
	size = state.numMemory; //memory start 1;
	state.pc = 0; // init pc
	memset(state.reg, 0, sizeof(int)*NUMREGS);  //init register

	while (state.pc > MAXLINELENGTH || state.mem>NUMMEMORY) {
		int Opcode;
		int regA, regB;
		int offset = 0;
		int destReg;
		int result;
		int instr = state.mem[state.pc];
		Opcode = instr >> 22;
		if (Opcode > NOOP || Opcode < 0) {
			break;
		}

		printState(&state);

		if (Opcode == HALT || Opcode == NOOP) {
			if (Opcode == HALT) {
				state.pc++;
				i++;
				break;
			}
			else if (Opcode == NOOP) {
				state.pc = state.pc;
			}
		}

		regA = (instr >> REGA) & MASK_REG;
		regB = (instr >> REGB) & MASK_REG;
		destReg = instr & MASK_REG;
		offset = instr & MASK;
		switch (Opcode) {
		case ADD:
			result = state.reg[regA] + state.reg[regB];
			state.reg[destReg] = result;
			break;

		case NOR:
			result = ~(state.reg[regA] | state.reg[regB]);
			state.reg[destReg] = result;
			break;

		case LW:
			state.reg[regB] = state.mem[convertNum(offset + state.reg[regA])];
			break;

		case SW:
			result = convertNum(offset) + state.reg[regA];
			state.mem[result] = state.reg[regB];
			break;

		case BEQ:
			if (state.reg[regA] == state.reg[regB]) {
				state.pc = state.pc + convertNum(offset);
			}
			break;

		case JALR:
			state.reg[regB] = state.pc + 1;
			state.pc = state.reg[regA] - 1;

			break;
		}
		i++;
		state.pc++;

	}
	printf("\n\nmachine halted\ntotal of %d instructions executed", i);
	printf("\nfinal state of machine:\n\n");
	printState(&state);
	while (1);
	return(0);
}

void
printState(stateType *statePtr)
{
	int i;
	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");
	for (i = 0; i<statePtr->numMemory; i++) {
		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
	printf("\tregisters:\n");
	for (i = 0; i<NUMREGS; i++) {
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
	printf("end state\n");
}

int
convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return(num);
}
