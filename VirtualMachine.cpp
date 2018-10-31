#include "VirtualMachine.h"
#include <iostream>
#include <iomanip>
#include <bitset>

void VirtualMachine::incInstPointer() {
	++instPointer;
	instReg1 = instPointer->cop1 & 0x0F;
	instReg2 = instPointer->cop1 & 0x0F >> 4;
}

void VirtualMachine::decodeAndExecuteInst(const char instCode, std::ifstream &dataFile) {
	switch (instCode) {
	case 0x01: INC();        break;
	case 0x02: DEC();        break;
	case 0x03: MOV();		 break;
	case 0x04: MOVC();		 break;
	case 0x05: LSL();		 break;
	case 0x06: LSR();		 break;
	case 0x07: JMP();		 break;
	case 0x0A: JFE();		 break;
	case 0x0B: RET();		 break;
	case 0x0C: ADD();		 break;
	case 0x0D: SUB();        break;
	case 0x0E: XOR();        break;
	case 0x0F: OR();         break;
	case 0x10: IN(dataFile); break;
	case 0x11: OUT();        break;
	default:                 break;
	}
}

VirtualMachine::VirtualMachine()
	: instPointer{ nullptr }, registers{ nullptr }, instructionSet{ nullptr },
	instReg1{ 0 }, instReg2{ 0 },
	jumpInstExecuted{ false }, VM_Stop{ false } {
	registers = new unsigned char[REGISTER_COUNT];
	instructionSet = new char[PROGRAM_MEMORY_SIZE];
}



VirtualMachine::~VirtualMachine() {
	delete[] registers;
	delete[] instructionSet;
	registers = nullptr;
	instructionSet = nullptr;
}



void VirtualMachine::readProgram(const char filePath[]) {

	std::ifstream instFile(filePath, std::ios_base::binary);
	
	if (!instFile) {
		std::cerr << "Failed to open instruction file" << std::endl;
	}
	else {
		int i{ 0 };
		while (instFile.get(*(instructionSet + i)) && i++ < PROGRAM_MEMORY_SIZE);
		instFile.close();
	}
}



void VirtualMachine::runProgram(const char instFilePath[], const char dataFilePath[]) {
	readProgram(instFilePath);

	instPointer = reinterpret_cast<VMInstruction*>(instructionSet);
	instReg1 = instPointer->cop1 & 0x0F;
	instReg2 = instPointer->cop1 & 0x0F >> 4;

	std::ifstream dataFile(dataFilePath);
	if (!dataFile) {
		std::cerr << "Failed to open data file" << std::endl;
	}
	else {
		while (!VM_Stop) {
			decodeAndExecuteInst(instPointer->code, dataFile);
			if (!jumpInstExecuted) {
				incInstPointer();
			}
			jumpInstExecuted = false;
		}
	}
}



void VirtualMachine::IN(std::ifstream &dataFile) {
	char tempVal{};

	if (dataFile.eof()) {
		eofFlag = 1;
	}
	else {
		dataFile.get(tempVal);
		*(registers + instReg1) = static_cast<unsigned char>(tempVal);
		eofFlag = 0;
	}
}



void VirtualMachine::INC() {

}



void VirtualMachine::DEC() {

}



void VirtualMachine::MOV() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	auto yReg = std::bitset<4>(((cop1Bits >> 4) & divider).to_ulong());

	registers[xReg.to_ulong()] = registers[yReg.to_ulong()];
}



void VirtualMachine::MOVC() {
	registers[0] = instPointer->cop1;
}



void VirtualMachine::LSL() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	registers[xReg.to_ulong()] <<= 1;
}



void VirtualMachine::LSR() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	registers[xReg.to_ulong()] >>= 1;
}



void VirtualMachine::JMP() {
	instPointer = (VMInstruction*)((char*)instPointer + instPointer->cop1);
	instReg1 = instPointer->cop1 & 0x0F;
	instReg2 = instPointer->cop1 & 0x0F >> 4;
	jumpInstExecuted = true;
}



void VirtualMachine::JFE() {
	if (eofFlag) { 
		JMP(); 
	}
}



void VirtualMachine::RET() {
	VM_Stop = true;
}



void VirtualMachine::ADD() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	auto yReg = std::bitset<4>(((cop1Bits >> 4) & divider).to_ulong());

	registers[xReg.to_ulong()] += registers[yReg.to_ulong()];
}



void VirtualMachine::SUB() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	auto yReg = std::bitset<4>(((cop1Bits >> 4) & divider).to_ulong());

	registers[xReg.to_ulong()] -= registers[yReg.to_ulong()];
}



void VirtualMachine::XOR() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	auto yReg = std::bitset<4>(((cop1Bits >> 4) & divider).to_ulong());

	registers[xReg.to_ulong()] ^= registers[yReg.to_ulong()];
}



void VirtualMachine::OR() {
	std::bitset<8> cop1Bits(instPointer->cop1);
	std::bitset<8> divider(0b00001111);

	auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());
	auto yReg = std::bitset<4>(((cop1Bits >> 4) & divider).to_ulong());

	registers[xReg.to_ulong()] |= registers[yReg.to_ulong()];
}



void VirtualMachine::OUT() {
	if (!eofFlag) {
		std::bitset<8> cop1Bits(instPointer->cop1);
		std::bitset<8> divider(0b00001111);

		auto xReg = std::bitset<4>((cop1Bits & divider).to_ulong());

		std::cout << registers[xReg.to_ulong()];
	}
}