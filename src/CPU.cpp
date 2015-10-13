#include "headers/CPU.hpp"
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

CPU::CPU()
{
	SP = STACK_END_OFFSET;
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

	if ((prev_A ^ sum) & (val ^ sum) & BYTE_SIGN_BIT_SET_MASK) {
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
	uint8_t mem = read_memory(address);
	uint8_t val = A - mem;
	if (A >= mem) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpx(uint16_t address) 
{
	uint8_t mem = read_memory(address);
	uint8_t val = X - mem;
	if (X >= mem) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpy(uint16_t address)
{
	uint8_t mem = read_memory(address);
	uint8_t val = Y - mem;
	if (Y >= mem) {
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
	if (V_flag) {
		PC += displacement;
	}
}

void CPU::bvc(uint8_t displacement) 
{
	if (!V_flag) {
		PC += displacement;
	}
}

void CPU::bit(uint16_t address)
{
	int mem_val = (A & read_memory(address));
	set_Z_flag(mem_val);
	mem_val >>= 6;
	V_flag = mem_val & 1;
	mem_val >>= 1;
	N_flag = mem_val & 1;
}

// Shift and rotate

void CPU::asl(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		uint8_t mem_val = read_memory(address);
		if ((mem_val & BYTE_SIGN_BIT_SET_MASK) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val = (mem_val << 1) & FE;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);

	} else {
		if ((A & BYTE_SIGN_BIT_SET_MASK) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		A = (A << 1) & FE;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

void CPU::lsr(uint16_t address, bool read_from_memory) 
{
	if (read_from_memory) {
		int mem_val = read_memory(address);
		if ((mem_val & BYTE_SIGN_UNSET_MAX) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val = (mem_val >> 1) & BYTE_SIGN_UNSET_MAX;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		write_memory(address, mem_val);
	} else {
		if ((A & BYTE_SIGN_UNSET_MAX) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		A = (A >> 1) & BYTE_SIGN_UNSET_MAX;
		set_Z_flag(A);
		set_N_flag(A);
	}
}

void CPU::rol(uint16_t address, bool read_from_memory)
{
	bool new_carry;
	uint8_t new_val;
	if (read_from_memory) {
		new_val = read_memory(address);
	} else {
		new_val = A;
	}
	new_carry = ((new_val & BYTE_SIGN_BIT_SET_MASK) == 0) ? false : true;
	new_val = rot_l(new_val);
	C_flag = new_carry;
	set_Z_flag(new_val);
	set_N_flag(new_val);
	if (read_from_memory) {
		write_memory(address, new_val);
	} else {
		A = new_val;
	}
}

void CPU::ror(uint16_t address, bool read_from_memory)
{
	bool new_carry;
	uint8_t new_val;
	if (read_from_memory) {
		new_val = read_memory(address);
	} else {
		new_val = A;
	}
	new_carry = ((new_val & BYTE_SIGN_BIT_SET_MASK) == 0) ? false : true;
	new_val = rot_r(new_val);
	C_flag = new_carry;
	set_Z_flag(new_val);
	set_N_flag(new_val);
	if (read_from_memory) {
		write_memory(address, new_val);
	} else {
		A = new_val;
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
	set_PS_flags(pull());
	// PS = pull();
}

void CPU::pla()
{
	A = pull();
	set_Z_flag(A);
	set_N_flag(A);
}

// Subroutine

void CPU::jsr(uint16_t address)
{
	push_address(PC - 1);
	PC = address;
}

void CPU::rts()
{
	PC = pull_address() + 1;
}

void CPU::rti()
{
	PS = pull();
	PC = pull_address();
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
uint16_t CPU::resolve_operand(int opcode, uint8_t high, uint8_t low)
{
	uint16_t ret = 0;
	uint16_t address = high;
	address <<= BYTE_LENGTH;
	address |= low;

	switch(opcode) {
		case ABSOLUTE:
			ret = read_memory(address);
			break;

		case ABSOLUTE_X:
			ret = (address + X) & UINT16_MAX;
			break;

		case ABSOLUTE_Y:
			ret = (address + Y) & UINT16_MAX;
			break;

		case ACCUMULATOR:
			break;

		case IMMEDIATE:
			ret = low;
			break;

		case IMPLIED:
			break;

		case INDEXED_INDIRECT:
			ret = read_memory((low + X + 1) & UINT8_MAX);
			ret <<= BYTE_LENGTH;
			ret |= read_memory((low + X) & UINT8_MAX);
			break;

		case INDIRECT:
			ret = read_memory(low + 1);
			ret <<= BYTE_LENGTH;
			ret |= read_memory(low);
			break;

		case INDIRECT_INDEXED:
			ret = read_memory((low + 1) & UINT8_MAX);
			ret <<= BYTE_LENGTH;
			ret |= read_memory(low);
			ret = ((ret + Y) & UINT16_MAX);
			break;

		case RELATIVE:
			break;

		case ZERO_PAGE:
			ret = low;
			break;

		case ZERO_PAGE_X:
			ret = (low + X) & UINT8_MAX;
			break;

		case ZERO_PAGE_Y:
			ret = (low + Y) & UINT8_MAX;
			break;
	}
	return ret;
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
	if (value & BYTE_SIGN_BIT_SET_MASK) {
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

void CPU::push(uint8_t val)
{
	memory[STACK_START + (SP--)] = val;
}

void CPU::push_address(uint16_t address)
{
	uint8_t low_byte = address;
	uint8_t high_byte = (address >> BYTE_LENGTH);
	push(low_byte);
	push(high_byte);
}

uint8_t CPU::pull()
{
	return memory[STACK_START + (++SP)];
}

uint16_t CPU::pull_address()
{
	uint8_t high_byte = pull();
	uint8_t low_byte = pull();
	uint16_t address = high_byte;
	address <<= BYTE_LENGTH;
	address |= low_byte;
	return address;
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


inline uint8_t CPU::rot_r(uint8_t value)
{
	// cout << bitset<8>(value) << " == " << bitset<8>((value << 7) | (value >> 1)) << endl;
	return (value << 7) | (value >> 1);
}

inline uint8_t CPU::rot_l(uint8_t value)
{
	// cout << bitset<8>(value) << " == " << bitset<8>((value >> 7) | (value << 1)) << endl;
	return (value >> 7) | (value << 1);
}

// Functions strictly for testing purposes

uint16_t CPU::get_PC()
{
	return PC;
}

uint8_t CPU::get_SP()
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

void CPU::set_PC(uint16_t val)
{
	PC = val;
}

void CPU::set_C(bool val)
{
	C_flag = val;
}

void CPU::set_Z(bool val)
{
	Z_flag = val;
}

void CPU::set_N(bool val)
{
	N_flag = val;
}

void CPU::set_V(bool val)
{
	V_flag = val;
}

void CPU::set_SP(uint8_t val)
{
	SP = val;
}

void CPU::set_PS(uint8_t val)
{
	PS = val;
}
