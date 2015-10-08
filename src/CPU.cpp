#include "headers/CPU.hpp"
#include <iostream>
#include <string>

using namespace std;

CPU::CPU()
{

}

uint8_t CPU::memory[MEM_SIZE];

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


// Load and store

void CPU::lda(uint16_t address)
{
	A = read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ldx(uint16_t address)
{
	X = read_memory(address);
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::ldy(uint16_t address)
{
	Y = read_memory(address);
	set_Z_flag(Y);
	set_N_flag(Y);
}

void CPU::sta(uint16_t memory_address)
{
	write_memory(memory_address, A);
}

void CPU::stx(uint16_t memory_address)
{
	write_memory(memory_address, X);
}

void CPU::sty(uint16_t memory_address)
{
	write_memory(memory_address, Y);
}

// Arithmetic

void CPU::adc(uint16_t address)
{
	uint8_t prev_A = A;
	uint8_t val = read_memory(address);
	uint8_t sum;
	if ((sum = val + prev_A + C_flag) > UINT8_MAX) {
		sec();
	} else {
		clc();
	}

	if ((prev_A ^ sum) & (val ^ sum) & BIT_8_MASK) {
		V_flag = true;
	} else {
		V_flag = false;
	}
	A = sum;
	set_Z_flag(A);
	set_N_flag(A);	
}

void CPU::sbc(uint16_t address)
{
	uint8_t prev_A = A;
	uint8_t val = read_memory(address);
	int sum = val - prev_A - !C_flag;
	if (sum > INT8_MAX || sum < INT8_MIN) {
		V_flag = true;
	} else {
		V_flag = false;
	}
	if (sum > UINT8_MAX) {
		sec();
	} else {
		clc();
	}

	A = (uint8_t)sum;
	set_Z_flag(A);
	set_N_flag(A);
}

// Increment and decrement


void CPU::inc(uint16_t address)
{
	int value = (read_memory(address) + 1) & UINT8_MAX;
	write_memory(address, value);
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

void CPU::dec(uint16_t address)
{
	int value = (read_memory(address) - 1) & UINT8_MAX;
	write_memory(address, value);
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

void CPU::_and(uint16_t address)
{
	A &= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ora(uint16_t address) 
{
	A |= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::eor(uint16_t address)
{
	A ^= read_memory(address);
	set_Z_flag(A);
	set_N_flag(A);
}

// Jump, branch, compare and test

// INDIRECT VECTOR SHOULD NOT BE AT PAGE END
void CPU::jmp(uint16_t address)
{
	PC = address;
}

void CPU::cmp(uint16_t address)
{
	int8_t val = A - read_memory(address);
	if (val >= 0) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpx(uint16_t address) 
{
	int val = X - read_memory(address);
	if (val >= 0) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpy(uint16_t address)
{
	int val = Y - read_memory(address);
	if (val >= 0) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::bcc(uint8_t displacement)
{
	if (!C_flag) {
		PC += displacement;
	}
}

void CPU::bcs(uint8_t displacement)
{
	if (C_flag) {
		PC += displacement;
	}
}

void CPU::beq(uint8_t displacement) 
{
	if (Z_flag) {
		PC += displacement;
	}
}

void CPU::bne(uint8_t displacement)
{
	if (!Z_flag) {
		PC += displacement;
	}
}

void CPU::bmi(uint8_t displacement)
{
	if (N_flag) {
		PC += displacement;
	}
}

void CPU::bpl(uint8_t displacement)
{
	if (!N_flag) {
		PC += displacement;
	}
}

void CPU::bvs(uint8_t displacement) 
{
	if (!V_flag) {
		PC += displacement;
	}
}

void CPU::bvc(uint8_t displacement) 
{
	if (V_flag) {
		PC += displacement;
	}
}

void CPU::bit(uint16_t address)
{
	int mem_val = (A & read_memory(address));
	set_Z_flag(mem_val);
	mem_val >>= 6;
	V_flag = 1 & mem_val;
	mem_val >>= 1;
	N_flag = 1 & mem_val;
}

// Shift and rotate

void CPU::asl(uint16_t address, bool read_from_memory)
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

void CPU::lsr(uint16_t address, bool read_from_memory) 
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

void CPU::rol(uint16_t address, bool read_from_memory)
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

void CPU::ror(uint16_t address, bool read_from_memory)
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

// Subroutine

void CPU::jsr(uint16_t address)
{
	push(read_memory(address - 1));
	PC = address;
}

void CPU::rts(uint16_t address)
{
	PC = pull() + 1;
}

// CHANGE NEEDED
void CPU::rti(uint16_t address)
{
	set_PS_flags(pull());
	PC = pull();
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

// Utils

/*
 * DEREFERENCE POINTER?
 *
 * Given an opcode and an operand, the function will resolve what the value
 * of the operand should be. This is achieved by looking at the addressing
 * mode for the instruction and thus retrieving the correct value.
 *
 * @return The address that the instruction needs to read values from
 */
int CPU::resolve_operand(int opcode, int operand)
{
	int resolved_operand = 0;
	unsigned int address = 0;
	unsigned char most_significant_byte = operand;
	unsigned char least_significant_byte = operand >> BYTE_LENGTH;
	address = most_significant_byte;
	address <<= BYTE_LENGTH;
	address |= least_significant_byte;

	switch(opcode) {
		case ABSOLUTE:
			resolved_operand = address;
			break;

		case ABSOLUTE_X:
			resolved_operand = read_memory(address) + X;
			break;

		case ABSOLUTE_Y:
			resolved_operand = read_memory(address) + Y;
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
			resolved_operand = (read_memory(operand) + X) % ZERO_PAGE_WRAPAROUND;
			break;

		case INDIRECT:
			// Read most significant byte of address
			resolved_operand = read_memory(address + 1); 
			resolved_operand <<= BYTE_LENGTH;
			// Read least significant byte of address
			resolved_operand |= read_memory(address);
			break;

		case INDIRECT_INDEXED:
			resolved_operand = read_memory(operand) + Y;
			break;

		case RELATIVE:
			resolved_operand = PC + (unsigned char)operand; // ?????????????????
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

inline uint8_t CPU::read_memory(uint16_t address)
{
	// if (address >= 0 && address <= MEM_SIZE) {
	// } else {
	// 	return -1;
	// }
	return memory[address];
}

inline void CPU::write_memory(uint16_t address, uint8_t value)
{
	// if (address >= 0 && address < MEM_SIZE) {
	// }
	memory[address] = value;
}

void CPU::set_Z_flag(uint8_t value)
{
	if (value == 0) {
		Z_flag = true;
	} else {
		Z_flag = false;
	}
}

void CPU::set_N_flag(uint8_t value)
{
	if (value & BIT_8_MASK) {
		N_flag = true;
	} else {
		N_flag = false;
	}
}

void CPU::set_PS_flags(uint8_t value)
{
	PS = value;
	C_flag = (value & 1) ? true : false;
	value >>= 1;
	Z_flag = (value & 1) ? true : false;
	value >>= 1;
	I_flag = (value & 1) ? true : false;
	value >>= 1;
	D_flag = (value & 1) ? true : false;
	value >>= 1;
	B_flag = (value & 1) ? true : false;
	value >>= 1;
	value >>= 1;
	V_flag = (value & 1) ? true : false;
	value >>= 1;
	N_flag = (value & 1) ? true : false;
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

void CPU::update_PS()
{
	PS = 0;
	PS |= N_flag;
	PS <<= 1;
	PS |= V_flag;
	PS <<= 1;
	PS <<= 1;
	PS |= B_flag;
	PS <<= 1;
	PS |= D_flag;
	PS <<= 1;
	PS |= I_flag;
	PS <<= 1;
	PS |= Z_flag;
	PS <<= 1;
	PS |= C_flag;
}

void CPU::print_state()
{
	cout << "A: " << hex << "0x" << A << endl
		<< "X: " << "0x" << X << endl
		<< "Y: " << "0x" << Y << endl
		<< "PC: " << "0x" << PC << endl
		<< "SP: " << "0x" << SP << endl << endl;
	cout << boolalpha << "N: " << N_flag << endl
		<< "C: " << C_flag << endl
		<< "Z: " << Z_flag << endl
		<< "I: " << I_flag << endl
		<< "D: " << D_flag << endl
		<< "B: " << B_flag << endl
		<< "V: " << V_flag << endl
		<< "S: " << S_flag << endl;
}

uint16_t CPU::get_PC()
{
	return PC;
}

uint16_t CPU::get_SP()
{
	return SP;
}

uint16_t CPU::get_PS()
{
	return PS;
}

bool CPU::is_N()
{
	return N_flag;
}

bool CPU::is_C()
{
	return C_flag;
}

bool CPU::is_Z()
{
	return Z_flag;
}

bool CPU::is_I()
{
	return I_flag;
}

bool CPU::is_D()
{
	return D_flag;
}

bool CPU::is_B()
{
	return B_flag;
}

bool CPU::is_V()
{
	return V_flag;
}

bool CPU::is_S()
{
	return S_flag;
}

uint8_t CPU::get_X()
{
	return X;
}

uint8_t CPU::get_Y()
{
	return Y;
}

uint8_t CPU::get_A()
{
	return A;
}

void CPU::set_X(uint8_t val)
{
	X = val;
}

void CPU::set_Y(uint8_t val)
{
	Y = val;
}

void CPU::set_A(uint8_t val)
{
	A = val;
}