#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MASK 0x0000FFFF
#define REG_A 19
#define REG_B 16
#define MAXLINELENGTH 1000
#define MAXMEM 1<<16

//opcode in binary
#define OP_ADD  0x00000000
#define OP_NOR  0x00400000
#define OP_LW   0x00800000
#define OP_SW   0x00C00000
#define OP_BEQ  0x01000000
#define OP_JALR 0x01400000
#define OP_HALT 0x01800000
#define OP_NOOP 0x01C00000

void printMemory(FILE *);
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int getLabelAddress(char *);
int encodeRType(char *, char *, char *);
int encodeIType(char *, char *, char *, int);
int encodeJType(char *, char *);
int encodeFType(char *);

char Label_Map[MAXMEM][7];
int Memory[MAXMEM];
int PC = 0;
int MEMEND;
