#include "headers/CPU.h"

static inline int read_memory(int address)
{
	if (address >= 0 && address <= MEM_SIZE) {
		return memory[address];
	} else {
		return -1;
	}
}

static inline void write_memory(int address, int value)
{
	if (address >= 0 && address <= MEM_SIZE) {
		memory[address] = value;
	}
}

// Load and store

void CPU::lda(int value)
{
	A = value;
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ldx(int value)
{
	X = value;
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::ldy(int value)
{
	Y = value;
	set_Z_flag(Y);
	set_N_flag(Y);
}

void CPU::sta(int memory_address)
{
	write_memory(memory_address, A);
}

void CPU::stx(int memory_address)
{
	write_memory(memory_address, X);
}

void CPU::sty(int memory_address)
{
	write_memory(memory_address, Y);
}

// Arithmetic

void CPU::adc(int value)
{
	int prev_A = A;
	A = (A + value) % MAX_BYTE_VAL;
	if (prev_A + value + (int)C >= MAX_BYTE_VAL) {
		C = true;
	} else {
		C = false;
	}
	if ((prev_A ^ value) & BIT_8 != 0 && (prev_A ^ A) & BIT_8 != 0) {
		V = true;
	} else {
		V = false;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::sbc(int value) 
{
	int prev_A = A;
	A = value - A - (1 - C);
	if (value - prev_A - (1 - (int)C) >= 0) {
		C = true;
	} else {
		C = false;
	}
	if ((prev_A ^ value) & BIT_8 != 0 && (prev_A ^ A) & BIT_8 != 0) {
		V = true;
	} else {
		V = false;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

// Increment and decrement


void CPU::inc(int address)
{
	int value = read_memory(address);
	write_memory(++value);
	set_Z_flag(value);
	set_N_flag(value);
}

void CPU::inx()
{
	X++;
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::iny()
{
	Y++;
	set_Z_flag(Y);
	set_N_flag(Y);
}

void CPU::dec(int address)
{
	int value = read_memory(address);
	write_memory(--value);
	set_Z_flag(value);
	set_N_flag(value);
}

void CPU::dex()
{
	X--;
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::dey()
{
	Y--;
	set_Z_flag(Y);
	set_N_flag(Y);
}

// Logical

void CPU::_and(int address)
{
	A &= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ora(int address) 
{
	A |= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::eor(int address)
{
	A ^= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

// Jump, branch, compare and test
// Shift and rotate
// Transfer

void CPU::tax()
{
	X = A;
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::tay()
{
	Y = A;
	set_Z_flag(Y);
	set_N_flag(Y);
}

void CPU::txa()
{
	A = X;
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::tya()
{
	A = Y;
	set_Z_flag(A);
	set_N_flag(A);
}

// Stack

void CPU::tsx()
{
	X = SP;
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::txs()
{
	SP = X;
	set_Z_flag(SP);
	set_N_flag(SP);
}


// Set and reset

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



// NOP and BRK




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

void CPU::set_Z_flag(int value)
{
	if (value == 0) {
		Z = true;
	} else {
		Z == false;
	}
}

void CPU::set_N_flag(int value)
{
	if (value & BIT_8 == 0) {
		N = false;
	} else {
		N = true;
	}
}