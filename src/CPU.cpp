#include "headers/CPU.hpp"
#include "headers/Cartridge.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include <boost/format.hpp>

#define PPU_CRTL_ADDRESS 0x2000

using namespace std;

CPU::CPU(Console *con)
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
	console = con;
	memory = new CPU_Memory(console);
}

CPU::CPU(Cartridge *cart, Console *con)
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
	console = con;
	memory = new CPU_Memory(cart, console);
}


// Stores the width of an instruction in bytes
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

// Stores how many cycles each individual instruction consumes
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

// Stores the addressing mode that the associated opcode has
const unsigned char CPU::opcode_addressing_mode[UINT8_MAX + 1] = {
	6,  7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	1,  7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	6,  7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	6,  7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 8, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	5,  7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
	5,  7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
	5,  7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
	10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
	5,  7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
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

/*
 * Stores a value in memory, or an immediate value in the A register.
 * Z and N flags are set accordingly.
 *
 * @param address The address where the value is located, or the value itself
 * @param read_from_memory Determines if the value is to be read from memory
 */
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

/*
 * Stores a value in memory, or an immediate value in the X register.
 * Z and N flags are set accordingly.
 *
 * @param address The address where the value is located, or the value itself
 * @param read_from_memory Determines if the value is to be read from memory
 */
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

/*
 * Stores a value in memory, or an immediate value in the Y register.
 * Z and N flags are set accordingly.
 *
 * @param address The address where the value is located, or the value itself
 * @param read_from_memory Determines if the value is to be read from memory
 */
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

/*
 * Stores the value in the A register in memory at the given address.
 *
 * @param memory_address Address where the value should be stored
 */
void CPU::sta(uint16_t memory_address)
{
	memory->write(memory_address, A);
}

/*
 * Stores the value in the X register in memory at the given address.
 *
 * @param memory_address Address where the value should be stored
 */
void CPU::stx(uint16_t memory_address)
{
	memory->write(memory_address, X);
}

/*
 * Stores the value in the Y register in memory at the given address.
 *
 * @param memory_address Address where the value should be stored
 */
void CPU::sty(uint16_t memory_address)
{
	memory->write(memory_address, Y);
}

// Arithmetic

/*
 * Performs addition with the value stored in the A register, and another 
 * operand. Uses carry bits to essentially have 9-bit arithmetic. Overflow
 * is stored in V and C flags. N and Z flags are set accordingly to the sum.
 *
 * Signed overflow is indicated in the V flag.
 * Unsigned overflow is indicated in the C flag.
 *
 * The formulas can be found here:
 * http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
 *
 * @param address Address of the operand, or the operand value itself
 * @param read_from_memory Determines if @address is a literal value, 
 *	or an address
 */
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
	// Check for unsigned overflow
	if ((val + prev_A + C_flag) > UINT8_MAX) {
		sec();
	} else {
		clc();
	}

	// Check for signed overflow
	if ((prev_A ^ sum) & (val ^ sum) & BYTE_SIGN_BIT_SET_MASK) {
		V_flag = true;
	} else {
		V_flag = false;
	}
	A = sum;
	set_Z_flag(A);
	set_N_flag(A);
}

/*
 * Performs subtraction with the value stored in the A register, and another 
 * operand. Because the flags behave identically when subtracting compared 
 * to addition, subtraction can be implemented with adc. This requires
 * the operand to be in one's complement. We can do this because subtracting
 * 5 from a value is the same as adding -5 to a value.
 *
 * Thorough explanations can be found here:
 * http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
 * http://forums.nesdev.com/viewtopic.php?t=8703
 *
 * @param address Address of the operand, or the operand value itself
 * @param read_from_memory Determines if @address is a literal value, 
 *	or an address
 */
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

/*
 * Increments the value in in memory.
 * Z and N flags are set accordingly.
 *
 * @param address The address of the value to be incremented
 */
void CPU::inc(uint16_t address)
{
	int value = (memory->read(address) + 1) & UINT8_MAX;
	memory->write(address, value);
	set_Z_flag(value);
	set_N_flag(value);
}

/*
 * Increment the value stored in the X register.
 * Z and N flags are set accordingly.
 */
void CPU::inx()
{
	X++;
	set_Z_flag(X);
	set_N_flag(X);
}

/*
 * Increment the value stored in the Y register.
 * Z and N flags are set accordingly.
 */
void CPU::iny()
{
	Y++;
	set_Z_flag(Y);
	set_N_flag(Y);
}

/*
 * Decrements the value in in memory.
 * Z and N flags are set accordingly.
 *
 * @param address The address of the value to be decremented
 */
void CPU::dec(uint16_t address)
{

	uint8_t value = memory->read(address) - 1;
	memory->write(address, value);
	set_Z_flag(value);
	set_N_flag(value);
}

/*
 * Decrement the value stored in the X register.
 * Z and N flags are set accordingly.
 */
void CPU::dex()
{
	X--;
	set_Z_flag(X);
	set_N_flag(X);
}

/*
 * Decrement the value stored in the Y register.
 * Z and N flags are set accordingly.
 */
void CPU::dey()
{
	Y--;
	set_Z_flag(Y);
	set_N_flag(Y);
}

// Logical

/*
 * Performs a logical AND with the provided operand and the A register.
 * The oeprand is either givende immediately, or found at the address in memory.
 * Z and N flags are set accordingly.
 *
 * @param address Address of the operand in memory, or its literal value
 * @aparam read_from_memory Determines if @address is an operand or address
 */
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

/*
 * Performs a logical OR with the provided operand and the A register.
 * The operand is either givende immediately, or found at the address in memory.
 * Z and N flags are set accordingly.
 *
 * @param address Address of the operand in memory, or its literal value
 * @aparam read_from_memory Determines if @address is an operand or address
 */
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

/*
 * Performs a logical XOR with the provided operand and the A register.
 * The operand is either givende immediately, or found at the address in memory.
 * Z and N flags are set accordingly.
 *
 * @param address Address of the operand in memory, or its literal value
 * @aparam read_from_memory Determines if @address is an operand or address
 */
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

/*
 * Changes the value of the program counter, causing the program to jump.
 *
 * @param address The address of the next instruction to be executed
 */
void CPU::jmp(uint16_t address)
{
	PC = address;
}

/*
 * Compares the A register with the provided operand. The C flag is set to
 * denote that A is larger or qeual than the operand.
 * N and Z flags are set accoringly.
 *
 * @param address The address of the operand, or the operand itself
 * @param read_from_memory Determines if @address is an operand or an address
 */
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

/*
 * Compares the X register with the provided operand. The C flag is set to
 * denote that X is larger or qeual than the operand.
 * N and Z flags are set accoringly.
 *
 * @param address The address of the operand, or the operand itself
 * @param read_from_memory Determines if @address is an operand or an address
 */
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

/*
 * Compares the Y register with the provided operand. The C flag is set to
 * denote that Y is larger or qeual than the operand.
 * N and Z flags are set accoringly.
 *
 * @param address The address of the operand, or the operand itself
 * @param read_from_memory Determines if @address is an operand or an address
 */
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

/*
 * Branches if the C flag is clear.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bcc(int8_t displacement)
{
	if (!C_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the C flag is set.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bcs(int8_t displacement)
{
	if (C_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the Z flag is set.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::beq(int8_t displacement) 
{
	if (Z_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the Z flag is clear.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bne(int8_t displacement)
{
	if (!Z_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the N flag is set.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bmi(int8_t displacement)
{
	if (N_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the N flag is clear.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bpl(int8_t displacement)
{
	if (!N_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the V flag is set.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bvs(int8_t displacement) 
{
	if (V_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Branches if the V flag is clear.
 *
 * @param displacement The value that we want to branch to
 */
void CPU::bvc(int8_t displacement) 
{
	if (!V_flag) {
		clock_cycle++;
		increment_on_page_crossing();
		PC += displacement;
	}
}

/*
 * Tests the bits in the A register with the value stored in memory.
 * This function does not change any of the values of the registers, or
 * values in memory, only the flags.
 *
 * Bits 6 and 7 if the memory value is moved to the V and N flags, respectively.
 *
 * @param address The address of the value we are comparing with
 */
void CPU::bit(uint16_t address)
{
	uint8_t mem_val = memory->read(address);
	V_flag = ((mem_val >> 6) & 1);
	set_N_flag(mem_val);
	mem_val = (A & mem_val);
	set_Z_flag(mem_val);
}

// Shift and rotate

/*
 * Shift either the A register, or a memory value one bit to the left.
 * Carry bit is put in the C flag.
 *
 * @param address The address of the value in memory
 * @param read_from_memory Determines if we are shifting the A register, or a
 *	value in memory
 */
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

/*
 * Shifts either a value in memory, or the A register, on bit to the right.
 * The least significant bit - carry bit - is move to the C flag.
 *
 * @param address The address of the value in memory
 * @param read_from_memory Determines if we are shifting the A register, or a
 *	value in memory
 */
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

/*
 * Rotates either a value in memory, or the A register, one bit to the left.
 * The bit that is rotated in comes from the C flag.
 * 
 * @param address The address of the value in memory
 * @param read_from_memory Determines if we are shifting the A register, or a
 *	value in memory
 */
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

/*
 * Rotates either a value in memory, or the A register, one bit to the right.
 * The bit that is rotated in comes from the C flag.
 * 
 * @param address The address of the value in memory
 * @param read_from_memory Determines if we are shifting the A register, or a
 *	value in memory
 */
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

/*
 * Moves the A register to the X register
 */
void CPU::tax()
{
	X = A;
	set_Z_flag(X);
	set_N_flag(X);
}

/*
 * Moves the A register to the Y register
 */
void CPU::tay()
{
	Y = A;
	set_Z_flag(Y);
	set_N_flag(Y);
}

/*
 * Moves the X regsiter to the A register.
 */
void CPU::txa()
{
	A = X;
	set_Z_flag(A);
	set_N_flag(A);
}

/*
 * Moves the Y register to the A register.
 */
void CPU::tya()
{
	A = Y;
	set_Z_flag(A);
	set_N_flag(A);
}

// Stack

/*
 * Moves the stack pointer to the X register.
 */
void CPU::tsx()
{
	X = SP;
	set_Z_flag(X);
	set_N_flag(X);
}

/*
 * Replaces the stack pointer with the value in the X register.
 */
void CPU::txs()
{
	SP = X;
}

/*
 * Pushes the A register onto the stack.
 */
void CPU::pha()
{
	push(A);
}

/*
 * Pushes the status register onto the stack.
 * Bit 4 is always set on the stack.
 */
void CPU::php()
{
	// Move all flags into the P value
	update_P();
	push(P | B_BIT);
}

/*
 * Pull a value from the stack and assign it to the status register.
 */
void CPU::plp()
{
	// Bit 5 is always set, (bit 4 is always unset ?)
	set_P_flags((pull() | UNUSED_BIT) & ALL_P_FLAGS_SET_B_FLAG_UNSET);
}

/*
 * Pull a value from the stack and assign it to the A register.
 */
void CPU::pla()
{
	A = (pull());
	set_Z_flag(A);
	set_N_flag(A);
}

// Subroutine

/*
 * Jump to a subroutine.
 * This involves storing the previous program counter on the stack, and then
 * changing the program counter.
 *
 * @param address The new value for the program counter
 */
void CPU::jsr(uint16_t address)
{
	push_address(PC - 1);
	PC = address;
}

/*
 * Return from a subroutine.
 * Pull a previously stored program counter from the stack, and re-assign it
 * to the program counter.
 */
void CPU::rts()
{
	PC = pull_address() + 1;
}

/*
 * Return from an interrupt.
 * Restore the status register and the program counter from before the 
 * interrupt and continue execution.
 */
void CPU::rti()
{
	// Bit 5 is always set in the status register
	P = pull() | UNUSED_BIT;
	PC = pull_address();
	set_P_flags(P);
}


// Set and reset

/*
 * Clear the C flag.
 */
void CPU::clc()
{
	C_flag = false;
}

/*
 * Clear the D flag.
 */
void CPU::cld()
{
	D_flag = false;
}

/*
 * Clear the I flag.
 */
void CPU::cli()
{
	I_flag = false;
}

/*
 * Clear the V flag.
 */
void CPU::clv()
{
	V_flag = false;
}

/*
 * Set the C flag.
 */
void CPU::sec()
{
	C_flag = true;
}

/*
 * Set the D flag.
 */
void CPU::sed()
{
	D_flag = true;
}

/*
 * Set the I flag.
 */
void CPU::sei()
{
	I_flag = true;
}



// NOP and BRK

/*
 * No operation.
 */
void CPU::nop()
{
}

/*
 * Simulates and interrupt by pushing the program counter and the status 
 * register to the stack, and then executing the interrupt handler routine.
 */
void CPU::brk()
{
	push_address(PC);
	B_flag = true;
	update_P();
	push(P);
	sei();
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
	address <<= BYTE_LENGTH;
	address |= low;
	base_address = address;
	addressing_mode_t mode = 
			static_cast<addressing_mode_t>(opcode_addressing_mode[opcode]);

	switch(mode) {
		case ABSOLUTE:
			ret = address;
			break;

		case ABSOLUTE_X:
			ret = (address + X) & UINT16_MAX;
			base_address = address;
			break;

		case ABSOLUTE_Y:
			ret = (address + Y) & UINT16_MAX;
			base_address = address;
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
			base_address = ret;
			ret = ((ret + Y) & UINT16_MAX);
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
	final_address = ret;

	return ret;
}

/*
 * Addresses are incorrectly calculated when the lower byte of the address is of
 * the form 0xFF. This bug does not carry the overflow from the 0xFF over to
 * 0x00 like it should. 
 * This function should only be called with indirectly addressed JMP 
 * instructions.
 *
 * @param address The source address for the destination address
 */
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

/*
 * Reads a value from memory.
 *
 * @param address The address we want to read from
 * @return The value stored in memory
 */
uint8_t CPU::read_memory(uint16_t address)
{
	return memory->read(address);
}

/*
 * Write a value to memory.
 *
 * @param address The address where we want to store @value
 * @param value The value that we want to store in memory
 */
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

/*
 * Sets the Z flag if the value is 0, clears it otherwise.
 *
 * @param value The value that we are checking
 */
void CPU::set_Z_flag(uint8_t value)
{
	if (value == 0) {
		Z_flag = true;
	} else {
		Z_flag = false;
	}
}

/*
 * The N flag is set if the most significant bit is set. It is cleared 
 * otherwise.
 *
 * @param value The value we are checking
 */
void CPU::set_N_flag(uint8_t value)
{
	if (value & BYTE_SIGN_BIT_SET_MASK) {
		N_flag = true;
	} else {
		N_flag = false;
	}
}

/*
 * Assigns the status register to the new value, and sets the flags accordingly
 *
 * @param value The new value for the status register
 */
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
	unused_flag = true;
	value >>= 1;
	V_flag = (value & 1) ? true : false;
	value >>= 1;
	N_flag = (value & 1) ? true : false;
}

/*
 * Pushes the given value onto the stack.
 *
 * @param val The value that is to be pushed on the stack
 */
void CPU::push(uint8_t val)
{
    memory->write(STACK_START + (SP--), val);
}

/*
 * Pushes an address onto the stack. The high byte is pushed before the low.
 *
 * @param address The address that is to be pushed onto the stack.
 */
void CPU::push_address(uint16_t address)
{
	uint8_t low_byte = address;
	uint8_t high_byte = (address >> BYTE_LENGTH);
	push(high_byte);
	push(low_byte);
}

/*
 * Pulls a value from the stack.
 *
 * @return The value on top of the stack.
 */
uint8_t CPU::pull()
{
    return memory->read(STACK_START + (++SP));
}

/*
 * Pulls an address from the stack. The low byte of the address is pulled 
 * before the high byte.
 *
 * @return The address that was stored on the stack.
 */
uint16_t CPU::pull_address()
{
	uint8_t low_byte = pull();
	uint8_t high_byte = pull();
	uint16_t address = high_byte;
	address <<= BYTE_LENGTH;
	address |= low_byte;
	return address;
}

/*
 * Reflects all the value of the status flags in the status register byte.
 */
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
		(int)Y % (int)P % (int)SP % (int)((clock_cycle * 3) % 341);
}

void CPU::print_state()
{
	std::cout << boost::format("A:%02X X:%02X Y:%02X P:%02X SP:%02X\n") % 
		(int)A % (int)X % (int)Y % (int)P % (int)SP;
}


// inline uint8_t CPU::rot_r(uint8_t value)
// {
// 	return (value << 7) | (value >> 1);
// }

uint8_t CPU::rot_l(uint8_t value)
{
	return (value >> 7) | (value << 1);
}


/*
 * UNFINISHED
 *
 * Fetches an instruction, along with any operands, determines the addressing 
 * mode and thus the effective address to be used, and executes the instruction.
 * This function does not emulate a cycle perfect instruction. This may have to
 * be changed depending on PPU emulation.
 *
 * 
 * The Function also writes the values of the opcode and operands to a log file.
 */
void CPU::fetch_and_execute() 
{
	std::ostringstream str;

	handle_interrupts();

	str << boost::format("%04X") % (int)PC;
	uint8_t opcode = memory->read(PC++);
	uint8_t low = 0;
	uint8_t high = 0;
	str << boost::format("  %02X ") % (int)opcode;

	// Instruction has only 1 operand
	if (instruction_length[opcode] == 2) {
		low = memory->read(PC++);
		str << boost::format("%02X%5s") % (int)low % "";
	// Instruction has 2 operands
	} else if (instruction_length[opcode] == 3){
		low = memory->read(PC++);
		high = memory->read(PC++);
		str << boost::format("%02X %02X  ") % (int)low % (int)high;
	// No operands for instruction
	} else {
		str << boost::format("%7s") % "";
	}
	uint16_t address = calculate_address(opcode, high, low);
	
	str << boost::format("%s\t") % instruction_names[opcode];
	f << str.str();

	write_state();
	
	execute(opcode, address);
	clock_cycle += execution_time[opcode];
	update_P();
	// Hard coded for NESTEST 
	if (PC == 0xC66E || linenum == 8992) {
		f.close();
		exit(0);
	}
	linenum++;
}

/*
 * Determines if two address are from different pages, i.e. if the high byte
 * of the addresses differ.
 *
 * @param address_1 The original address
 * @param address_2 The current address being used
 * @return True if the high byte of the addresses differ
 */
bool CPU::pages_differ(uint16_t address_1, uint16_t address_2)
{
	const uint16_t high_mask = ((UINT8_MAX << BYTE_LENGTH) | 0);
	return (address_1 & high_mask) != (address_2 & high_mask);
}

/*
 * Increments the clock if an addresses crossed a page.
 */
void CPU::increment_on_page_crossing()
{
	if (pages_differ(final_address, base_address)) {
		clock_cycle++;
	}
}

/*
 * Handles the 3 different types of interrupts that the NES has.
 */
void CPU::handle_interrupts()
{
	// check for RESET here

	// Execute NMI when NMI is enabled by PPU
	if ((memory->read(PPU_CRTL_ADDRESS) & BYTE_SIGN_BIT_SET_MASK)) {
		nmi();
	}
	// Execute IRQ when Interrupt disable flag is clear
	if (!I_flag) {
		irq();
	}
}

/*
 * The NMI interrupt routine. 
 * Pushes the program counter and the status register onto the stack,
 * and then retrieves the NMI vector and continues execution from this 
 * point.
 */
void CPU::nmi()
{
	push_address(PC);
	push(P & ALL_P_FLAGS_SET_B_FLAG_UNSET);
	sei();
	PC = memory->read_address(NMI_VECTOR);
	clock_cycle += INTERRUPT_LATENCY;
	// if ((memory->read(0x2000) & 0x80)) {
	// }
}

/*
 * The RESET interrupt routine.
 * Changes the program counter to that of the reset vector, and continues
 * execution.
 */
void CPU::reset()
{
	PC = memory->read_address(RESET_VECTOR);
	SP -= 3;
	clock_cycle += 7;
}

/*
 * The Interrupt request - IRQ - routine.
 * Pushes the status program counter and the status register onto the stack.
 * The I flag is set ot indicate that an IRQ has happened.
 * The program continues to execute from the address stored at the IRQ vector.
 */
void CPU::irq()
{
	push_address(PC);
	php();
	sei();
	PC = memory->read_address(IRQ_VECTOR);
	clock_cycle += 7;
	// if (!I_flag) {
	// }
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

void CPU::set_I(bool val)
{
	I_flag = val;
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
	set_P_flags(P);
}
