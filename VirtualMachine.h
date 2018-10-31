#pragma once
#include <stdio.h>
#include <fstream>
#include "VMInstruction.h"

class VirtualMachine {
private:
	unsigned char *registers;
	char *instructionSet;
	int eofFlag;
	int instReg1, instReg2;
	VMInstruction *instPointer;
	void incInstPointer();
	void decodeAndExecuteInst(const char instCode, std::ifstream &dataFile);
	bool jumpInstExecuted;
	bool VM_Stop;
public:
	static const int REGISTER_COUNT{ 16 };
	static const int PROGRAM_MEMORY_SIZE{ 256 };

	VirtualMachine();
	~VirtualMachine();

	void readProgram(const char filePath[]);
	void runProgram(const char instFilePath[], const char dataFilePath[]);
	void IN(std::ifstream &dataFile);
	void INC();
	void DEC();
	void MOV();
	void MOVC();
	void LSL();
	void LSR();
	void JMP();
	void JFE();
	void RET();
	void ADD();
	void SUB();
	void XOR();
	void OR();
	void OUT();
};

