#include "headers/CPU.hpp"
#include "headers/Cartridge.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include <boost/format.hpp>

using namespace std;

CPU::CPU()
{
	// SP = STACK_END_OFFSET;
	P = STATUS_REGISTER_POWER_UP_STATE;
	set_P_flags(0x24);
	SP = 0xFD;
	P  = 0x24;
	PC = 0;
	A  = 0;
	X  = 0;
	Y  = 0;
	clock_cycle = 0;
	f.open("my_log.txt", ofstream::out);
	linenum = 1;
	memory = new CPU_Memory();
}

CPU::CPU(Cartridge *cart)
{
    P = STATUS_REGISTER_POWER_UP_STATE;
	set_P_flags(0x24);
	SP = 0xFD;
	P  = 0x24;
	PC = 0;
	A  = 0;
	X  = 0;
	Y  = 0;
	clock_cycle = 0;
	f.open("my_log.txt", ofstream::out);
	linenum = 1;
	memory = new CPU_Memory(cart);
}

// std::vector<uint8_t> CPU::memory(MEM_SIZE);

const unsigned char CPU::instruction_length[UINT8_MAX + 1] = {
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

const unsigned char CPU::execution_time[UINT8_MAX + 1] = {
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

const unsigned char CPU::opcode_addressing_mode[UINT8_MAX + 1] = {
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

const string CPU::instruction_names[UINT8_MAX + 1] = {
	"BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
	"BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
	"JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
	"PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
	"BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
	"SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
	"RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
	"BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
	"RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
	"BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
	"NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
	"DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
	"BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
	"TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
	"LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
	"CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
	"CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
	"INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
	"BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
	"INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
	"BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
	"SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC",
};

// Load and store

void CPU::lda(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		A = memory->read(address);
	} else {
		A = address;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ldx(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		X = memory->read(address);
	} else {
		X = address;
	}
	set_Z_flag(X);
	set_N_flag(X);
}

void CPU::ldy(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		Y = memory->read(address);
	} else {
		Y = address;
	}
	set_Z_flag(Y);
	set_N_flag(Y);
}

void CPU::sta(uint16_t memory_address)
{
	memory->write(memory_address, A);
}

void CPU::stx(uint16_t memory_address)
{
	memory->write(memory_address, X);
}

void CPU::sty(uint16_t memory_address)
{
	memory->write(memory_address, Y);
}

// Arithmetic

void CPU::adc(uint16_t address, bool read_from_memory)
{
	uint8_t prev_A = A;
	uint8_t val;
	if (read_from_memory) {
		val = memory->read(address);
	} else {
		val = address;
	}
	uint8_t sum = val + prev_A + C_flag;
	if ((val + prev_A + C_flag) > UINT8_MAX) {
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

void CPU::sbc(uint16_t address, bool read_from_memory)
{
	uint8_t val;
	if (read_from_memory) {
		val = memory->read(address);
	} else {
		val = address;
	}
	adc(~val, false);
}

// Increment and decrement


void CPU::inc(uint16_t address)
{
	int value = (memory->read(address) + 1) & UINT8_MAX;
	memory->write(address, value);
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

	uint8_t value = memory->read(address) - 1;
	memory->write(address, value);
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

void CPU::_and(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		A &= memory->read(address);
	} else {
		A &= address;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::ora(uint16_t address, bool read_from_memory) 
{
	if (read_from_memory) {
		A |= memory->read(address);
	} else {
		A |= address;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

void CPU::eor(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		A ^= memory->read(address);
	} else {
		A ^= (uint8_t)address;
	}
	set_Z_flag(A);
	set_N_flag(A);
}

// Jump, branch, compare and test

void CPU::jmp(uint16_t address)
{
	PC = address;
}

void CPU::cmp(uint16_t address, bool read_from_memory)
{
	uint8_t mem;
	if (read_from_memory) {
		mem = memory->read(address);	
	} else {
		mem = address;
	}
	uint8_t val = A - mem;
	if (A >= mem) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpx(uint16_t address, bool read_from_memory) 
{
	uint8_t mem; 
	if (read_from_memory) {
		mem = memory->read(address);
	} else {
		mem = address;
	}
	uint8_t val = X - mem;
	if (X >= mem) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::cpy(uint16_t address, bool read_from_memory)
{
	uint8_t mem;
	if (read_from_memory) {
		mem = memory->read(address);
	} else {
		mem = address;
	}
	uint8_t val = Y - mem;
	if (Y >= mem) {
		sec();
	} else {
		clc();
	}
	set_Z_flag(val);
	set_N_flag(val);
}

void CPU::bcc(int8_t displacement)
{
	if (!C_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bcs(int8_t displacement)
{
	if (C_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::beq(int8_t displacement) 
{
	if (Z_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bne(int8_t displacement)
{
	if (!Z_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bmi(int8_t displacement)
{
	if (N_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bpl(int8_t displacement)
{
	if (!N_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bvs(int8_t displacement) 
{
	if (V_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bvc(int8_t displacement) 
{
	if (!V_flag) {
		increment_on_page_boundary();
		PC += displacement;
	}
}

void CPU::bit(uint16_t address)
{
	uint8_t mem_val = memory->read(address);
	V_flag = ((mem_val >> 6) & 1);
	set_N_flag(mem_val);
	mem_val = (A & mem_val);
	set_Z_flag(mem_val);
}

// Shift and rotate

void CPU::asl(uint16_t address, bool read_from_memory)
{
	if (read_from_memory) {
		uint8_t mem_val = memory->read(address);
		if ((mem_val & BYTE_SIGN_BIT_SET_MASK) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val = (mem_val << 1) & FE;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		memory->write(address, mem_val);

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
		int mem_val = memory->read(address);
		if ((mem_val & 1) == 0) {
			C_flag = false;
		} else {
			C_flag = true;
		}
		mem_val = (mem_val >> 1) & BYTE_SIGN_UNSET_MAX;
		set_Z_flag(mem_val);
		set_N_flag(mem_val);
		memory->write(address, mem_val);
	} else {
		if ((A & 1) == 0) {
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
		new_val = memory->read(address);
	} else {
		new_val = A;
	}
	new_carry = ((new_val & BYTE_SIGN_BIT_SET_MASK) == 0) ? false : true;
	new_val = rot_l(new_val);
	C_flag = new_carry;
	set_Z_flag(new_val);
	set_N_flag(new_val);
	if (read_from_memory) {
		memory->write(address, new_val);
	} else {
		A = new_val;
	}
}

void CPU::ror(uint16_t address, bool read_from_memory)
{
	uint8_t new_val;
	if (read_from_memory) {
		new_val = memory->read(address);
	} else {
		new_val = A;
	}
	bool new_carry = ((new_val & 1) == 0) ? false : true;
	new_val >>= 1;
	new_val |= ((C_flag << 7));
	C_flag = new_carry;
	set_Z_flag(new_val);
	set_N_flag(new_val);
	if(read_from_memory) {
		memory->write(address, new_val);
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
	update_P();
	push(P | B_BIT);
}

void CPU::plp()
{
	// Bit 5 is always set, (bit 4 is always unset ?)
	set_P_flags((pull() | UNUSED_BIT) & ALL_P_FLAGS_SET_B_FLAG_UNSET);
}

void CPU::pla()
{
	A = (pull());
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
	P = pull() | UNUSED_BIT;
	PC = pull_address();
	set_P_flags(P);
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
	push((PC >> BYTE_LENGTH) & UINT8_MAX);
	push(PC & UINT8_MAX);
	B_flag = true;
	I_flag = true;
	push(P | B_BIT);
	PC = memory->read(IRQ_VECTOR);
	PC <<= BYTE_LENGTH;
	PC |= memory->read(IRQ_VECTOR + 1);
}

// Utils

/*
 * Given an opcode and an operand, the function will resolve what the value
 * of the operand should be. This is achieved by looking at the addressing
 * mode for the instruction and thus retrieving the correct value.
 *
 * @param opcode The opcode of the intruction
 * @param high The high byte of the address
 * @param low The low byte of the address
 * @return The address that the instruction needs to read values from
 */
uint16_t CPU::calculate_address(int opcode, uint8_t high, uint8_t low)
{
	uint16_t ret = 0;
	uint16_t address = high;
	bool differ = false;
	address <<= BYTE_LENGTH;
	address |= low;
	addressing_mode_t mode = 
			static_cast<addressing_mode_t>(opcode_addressing_mode[opcode]);

	switch(mode) {
		case ABSOLUTE:
			ret = address;
			break;

		case ABSOLUTE_X:
			ret = (address + X) & UINT16_MAX;
			differ = pages_differ(address, ret);
			break;

		case ABSOLUTE_Y:
			ret = (address + Y) & UINT16_MAX;
			differ = pages_differ(address, ret);
			break;

		case ACCUMULATOR:
			break;

		case IMMEDIATE:
			ret = low;
			break;

		case IMPLIED:
			break;

		case INDEXED_INDIRECT: // X
			// Get the HIGH byte for address
			ret = memory->read((uint8_t)(low + X + 1) & UINT8_MAX);
			ret <<= BYTE_LENGTH;
			// Get the LOW byte for the address
			ret |= memory->read((uint8_t)(low + X) & UINT8_MAX);
			break;

		case INDIRECT:
			ret = calculate_address_buggy(address);
			break;

		case INDIRECT_INDEXED: // Y
			ret = memory->read((low + 1) & UINT8_MAX);
			ret <<= BYTE_LENGTH;
			ret |= memory->read(low);
			ret = ((ret + Y) & UINT16_MAX);
			differ = pages_differ(address, ret);
			break;

		case RELATIVE:
			ret = address;
			// PAGE BOUNDARY
			break;

		case ZERO_PAGE:
			ret = low;
			break;

		case ZERO_PAGE_X:
			// All zero page addresses are 8 bits
			ret = (uint8_t)(low + X);
			break;

		case ZERO_PAGE_Y:
			ret = (uint8_t)(low + Y);
			break;

		default:
			std::cerr << "invalid opcode <0x" << std::hex << opcode << ">" << std::endl;
	}

	if (differ) {
		clock_cycle++;
	}
	return ret;
}

uint16_t CPU::calculate_address_buggy(uint16_t address) {
	uint16_t ret;
	if ((uint8_t)address == UINT8_MAX) {
		ret = memory->read(address & 0xFF00);
	} else {
		ret = memory->read(address + 1);
	}
	ret <<= BYTE_LENGTH;
	ret |= memory->read(address);
	return ret;
}


uint8_t CPU::read_memory(uint16_t address)
{
	return memory->read(address);
}


void CPU::write_memory(uint16_t address, uint8_t value)
{
	memory->write(address, value);
}

void CPU::load_prg_bank_lower(std::vector<uint8_t> &v)
{
	if (v.size() == PRG_ROM_UNITS) {
		int j = PRG_LOWER_BANK_ADDRESS;
		for (int i = 0; i < v.size(); i++, j++) {
			memory->write(j, v.at(i));
		}
	} else {
		std::cerr << "PRG ROM has illegal size: <" << v.size() << ">" << std::endl;
	}
}

void CPU::load_prg_bank_upper(std::vector<uint8_t> &v)
{
	if (v.size() == PRG_ROM_UNITS) {
		int j = PRG_UPPER_BANK_ADDRESS;
		for (int i = 0; i < v.size(); i++, j++) {
			memory->write(j, v.at(i));
		}
	} else {
		std::cerr << "PRG ROM has illegal size: <" << v.size() << ">" << std::endl;	
	}
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

void CPU::set_P_flags(uint8_t value)
{
	P = value;
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
	unused_flag = (value & 1) ? true : false;
	value >>= 1;
	V_flag = (value & 1) ? true : false;
	value >>= 1;
	N_flag = (value & 1) ? true : false;
}

void CPU::push(uint8_t val)
{
    memory->write(STACK_START + (SP--), val);
}

void CPU::push_address(uint16_t address)
{
	uint8_t low_byte = address;
	uint8_t high_byte = (address >> BYTE_LENGTH);
	push(high_byte);
	push(low_byte);
}

uint8_t CPU::pull()
{
    return memory->read(STACK_START + (++SP));
}

uint16_t CPU::pull_address()
{
	uint8_t low_byte = pull();
	uint8_t high_byte = pull();
	uint16_t address = high_byte;
	address <<= BYTE_LENGTH;
	address |= low_byte;
	return address;
}

void CPU::update_P()
{
	P = 0;
	P |= N_flag;
	P <<= 1;
	P |= V_flag;
	P <<= 1;
	P |= unused_flag;
	P <<= 1;
	P |= B_flag;
	P <<= 1;
	P |= D_flag;
	P <<= 1;
	P |= I_flag;
	P <<= 1;
	P |= Z_flag;
	P <<= 1;
	P |= C_flag;
}

void CPU::write_state()
{
	f << boost::format("\tA:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d\n") % (int)A % (int)X % 
		(int)Y % (int)P % (int)SP % ((clock_cycle * 3) % 341);
}

void CPU::print_state()
{
	std::cout << boost::format("A:%02X X:%02X Y:%02X P:%02X SP:%02X\n") % 
		(int)A % (int)X % (int)Y % (int)P % (int)SP;
}


inline uint8_t CPU::rot_r(uint8_t value)
{
	return (value << 7) | (value >> 1);
}

inline uint8_t CPU::rot_l(uint8_t value)
{
	return (value >> 7) | (value << 1);
}


/*
 * UNFINISHED
 */
void CPU::fetch_and_execute() 
{
	std::ostringstream str;
	str << boost::format("%04X") % (int)PC;
	uint8_t opcode = memory->read(PC++);
	uint8_t low = 0;
	uint8_t high = 0;
	str << boost::format("  %02X ") % (int)opcode;

	if (instruction_length[opcode] == 2) {
		low = memory->read(PC++);
		str << boost::format("%02X%5s") % (int)low % "";
	} else if (instruction_length[opcode] == 3){
		low = memory->read(PC++);
		high = memory->read(PC++);
		str << boost::format("%02X %02X  ") % (int)low % (int)high;
	} else {
		str << boost::format("%7s") % "";
	}
	uint16_t address = calculate_address(opcode, high, low);
	
	current_address = address;
	last_address = ((high << BYTE_LENGTH) | low);
	
	str << boost::format("%s\t") % instruction_names[opcode];
	f << str.str();

	write_state();
	execute(opcode, address);
	clock_cycle += execution_time[opcode];
	update_P();
	if (PC == 0xC66E || linenum == 8992) {
		f.close();
		exit(0);
	}
	linenum++;
}

bool CPU::pages_differ(uint16_t address_1, uint16_t address_2)
{
	const uint16_t high_mask = (UINT8_MAX << BYTE_LENGTH);
	return (address_1 & high_mask) != (address_2 & high_mask);
}

void CPU::increment_on_page_boundary()
{
	if (pages_differ(current_address, last_address)) {
		clock_cycle += 2;
	} else {
		clock_cycle++;
	}
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

uint16_t CPU::get_P()
{
	return P;
}

int CPU::get_clock_cycles() {
	return clock_cycle;
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

bool CPU::is_unused()
{
	return unused_flag;
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

void CPU::set_P(uint8_t val)
{
	P = val;
}
