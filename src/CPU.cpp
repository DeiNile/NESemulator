#include "headers/CPU.h"

using namespace std;

CPU::CPU()
{

}

int CPU::memory[MEM_SIZE];

const unsigned char CPU::instruction_length[MAX_BYTE_VAL] = {
		1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
		2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	};

const unsigned char CPU::execution_time[MAX_BYTE_VAL] = {
		7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
		2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
		2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
		2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
		2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
		2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	};

const unsigned char CPU::opcode_addressing_mode[MAX_BYTE_VAL] = {
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		1, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 8, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	};

inline int CPU::read_memory(int address)
{
	if (address >= 0 && address <= MEM_SIZE) {
		return memory[address];
	} else {
		return -1;
	}
}

inline void CPU::write_memory(int address, int value)
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
	if (prev_A + value + (int)C_flag >= MAX_BYTE_VAL) {
		C_flag = true;
	} else {
		C_flag = false;
	}
	if (((prev_A ^ value) & BIT_8_MASK) != 0 && ((prev_A ^ A) & BIT_8_MASK) != 0) {
		V_flag = true;
	} else {
		V_flag = false;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::sbc(int value) 
{
	int prev_A = A;
	A = value - A - (1 - C_flag);
	if (value - prev_A - (1 - (int)C_flag) >= 0) {
		C_flag = true;
	} else {
		C_flag = false;
	}
	if (((prev_A ^ value) & BIT_8_MASK) != 0 && ((prev_A ^ A) & BIT_8_MASK) != 0) {
		V_flag = true;
	} else {
		V_flag = false;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

// Increment and decrement


void CPU::inc(int address)
{
	int value = read_memory(address);
	write_memory(address, ++value);
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
	write_memory(address, --value);
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

// INDIRECT VECTOR SHOULD NOT BE AT PAGE END
void CPU::jmp(int address)
{
	PC = address;
}

void CPU::cmp(int address)
{
	int val = A - read_memory(address);
	if (val >= 0) {
		C_flag = true;
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpx(int address) 
{
	int val = X - read_memory(address);
	if (val >= 0) {
		C_flag = true;
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpy(int address)
{
	int val = Y - read_memory(address);
	if (val >= 0) {
		C_flag = true;
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::bcc(int displacement)
{
	if (!C_flag) {
		PC += displacement;
	}
}

void CPU::bcs(int displacement)
{
	if (C_flag) {
		PC += displacement;
	}
}

void CPU::beq(int displacement) 
{
	if (Z_flag) {
		PC += displacement;
	}
}

void CPU::bne(int displacement)
{
	if (!Z_flag) {
		PC += displacement;
	}
}

void CPU::bmi(int displacement)
{
	if (N_flag) {
		PC += displacement;
	}
}

void CPU::bpl(int displacement)
{
	if (!N_flag) {
		PC += displacement;
	}
}

void CPU::bvs(int displacement) 
{
	if (!V_flag) {
		PC += displacement;
	}
}

void CPU::bvc(int displacement) 
{
	if (V_flag) {
		PC += displacement;
	}
}

void CPU::bit(int address)
{
	int mem_val = read_memory(address);
	set_Z_flag(A & mem_val);
	mem_val >>= 6;
	V_flag = 1 & mem_val;
	mem_val >>= 1;
	N_flag = 1 & mem_val;
}

// Shift and rotate

void CPU::asl(int address, bool read_from_memory)
{
	if (read_from_memory) {
		int mem_val = read_memory(address);
		if ((mem_val & BIT_8_MASK) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val <<= 1;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);

	} else {
		if ((A & BIT_8_MASK) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		A <<= 1;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

void CPU::lsr(int address, bool read_from_memory) 
{
	if (read_from_memory) {
		int mem_val = read_memory(address);
		if ((mem_val & 1) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val >>= 1;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);
	} else {
		if ((A & 1) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		A >>= 1;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

void CPU::rol(int address, bool read_from_memory)
{
	bool new_carry;
	if (read_from_memory) {
		int mem_val = read_memory(address);
		if ((mem_val & BIT_8_MASK) == 0) {
			new_carry = false;
		} else {
			new_carry = true;
		}
		mem_val <<= 1;
		mem_val |= C_flag;
		C_flag = new_carry;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);
	} else {
		if ((A & BIT_8_MASK) == 0) {
			new_carry = false;
		} else {
			new_carry = true;
		}
		A <<= 1;
		A |= C_flag;
		C_flag = new_carry;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

void CPU::ror(int address, bool read_from_memory)
{
	bool new_carry;
	int carry_in_bit_7 = C_flag << BIT_0_TO_7;
	if (read_from_memory) {
		int mem_val = read_memory(address);
		new_carry = (mem_val & 1);
		mem_val >>= 1;
		mem_val |= carry_in_bit_7;
		C_flag = new_carry;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);
	} else {
		new_carry = (A & 1);
		A >>= 1;
		A |= carry_in_bit_7;
		C_flag = new_carry;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

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

void CPU::pha()
{
	push(A);
}

void CPU::php()
{
	push(PS);
}

void CPU::plp()
{
	PS = pull();
}


// Set and reset

void CPU::clc()
{
	C_flag = false;
}

void CPU::cld()
{
	D_flag = false;
}

void CPU::cli()
{
	I_flag = false;
}

void CPU::clv()
{
	V_flag = false;
}

void CPU::sec()
{
	C_flag = true;
}

void CPU::sed()
{
	D_flag = true;
}

void CPU::sei()
{
	I_flag = true;
}



// NOP and BRK

void CPU::nop()
{
}

void CPU::brk()
{
	B_flag = true;
	push(PC);
	push(PS);
	PC = read_memory(INTERRUPT_VECTOR);
	PC <<= BYTE_LENGTH;
	PC |= read_memory(INTERRUPT_VECTOR + 1);
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
		Z_flag = true;
	} else {
		Z_flag = false;
	}
}

void CPU::set_N_flag(int value)
{
	if ((value & BIT_8_MASK) == 0) {
		N_flag = false;
	} else {
		N_flag = true;
	}
}

void CPU::push(int val)
{
	if (SP - 1 < 0) {
		SP = 0;
	}
	memory[STACK_START + (SP--)] = val;
}

int CPU::pull()
{
	if (SP + 1 > STACK_POINTER_WRAPAROUND) {
		SP = 0;
	}
	return memory[STACK_START + (SP++)];
}