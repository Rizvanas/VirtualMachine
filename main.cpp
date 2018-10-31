#include <iostream>
#include "VirtualMachine.h"

const char PROGRAM_FILE_NAME[]{ "decryptor.bin" };
const char DATA_FILE_NAME[]{ "q1_encr.txt" };

int main(int argc, char *argv[]) {
	VirtualMachine VM;

	VM.runProgram(PROGRAM_FILE_NAME, DATA_FILE_NAME);
	std::cout << std::endl;
	return 0;
}