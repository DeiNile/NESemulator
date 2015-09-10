#include "headers/CPU.h"

void CPU::clc()
{
	C = false;
}

void CPU::cld()
{
	D = false;
}

void CPU::cli()
{
	I = false;
}

void CPU::clv()
{
	V = false;
}

void CPU::sec()
{
	C = true;
}

void CPU::sed()
{
	D = true;
}

void CPU::sei()
{
	I = true;
}

void CPU::inx()
{
	X++;
}

void CPU::iny()
{
	Y++;
}

void CPU::dex()
{
	X--;
}

void CPU::dey()
{
	Y--;
}


int CPU::read_memory(int memoryLocation)
{
	if (memoryLocation >= 0 && memoryLocation <= MEM_SIZE) {
		return memory[memoryLocation];
	} else {
		return 0;
	}
}

int CPU::resolve_operand(int opcode, int operand)
{
	int resolved_operand = 0;
	unsigned int address = 0;
	unsigned char most_significant_byte = operand;
	unsigned char least_significant_byte = operand >> BYTE_LENGTH;
	signed char relative_address = operand;
	address = most_significant_byte;
	address <<= BYTE_LENGTH;
	address |= least_significant_byte;

	switch(opcode) {
		case ABSOLUTE:
			resolved_operand = read_memory(address);
			break;

		case ABSOLUTE_X:
			resolved_operand = read_memory(address + X);
			break;

		case ABSOLUTE_Y:
			resolved_operand = read_memory(address + Y);
			break;
			
		case ACCUMULATOR:
			resolved_operand = A;
			break;
			
		case IMMEDIATE:
			resolved_operand = operand;
			break;
			
		case IMPLIED:
			break;
		case INDEXED_INDIRECT:
			address = read_memory((operand + X) % ZERO_PAGE_WRAPAROUND);
			resolved_operand = read_memory(address);
			break;
			
		case INDIRECT:
			// Read most significant byte of address
			resolved_operand = read_memory(address + 1); 
			resolved_operand <<= BYTE_LENGTH;
			// Read least significant byte of address
			resolved_operand |= read_memory(address);
			break;
			
		case INDIRECT_INDEXED:
			resolved_operand = read_memory(operand + Y);
			break;
			
		case RELATIVE:
			resolved_operand = PC + 2 + relative_address; // ?????????????????
			break;
				
		case ZERO_PAGE:
			resolved_operand = read_memory(operand);
			break;
			
		case ZERO_PAGE_X:
			resolved_operand = (X + read_memory(operand)) % ZERO_PAGE_WRAPAROUND;
			break;
			
		case ZERO_PAGE_Y:
			resolved_operand = (Y + read_memory(operand)) % ZERO_PAGE_WRAPAROUND;
			break;
	}

	return resolved_operand;
}