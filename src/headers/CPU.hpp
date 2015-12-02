#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Constants_opcodes.hpp"
#include "Memory.hpp"
#include "CPU_memory.hpp"
#include "NROM_Mapper.hpp"
#include "Cartridge.hpp"

#define MEM_SIZE 65536
#define ZERO_PAGE_WRAPAROUND 0xFF
#define STACK_POINTER_WRAPAROUND 0xFF
#define BYTE_SIGN_BIT_SET_MASK 0x80
#define BYTE_LENGTH 8
#define BIT_0_TO_7 7
#define FE 0xFE
#define BYTE_SIGN_UNSET_MAX 0x7F
#define PRG_LOWER_BANK_ADDRESS 0x8000
#define PRG_UPPER_BANK_ADDRESS 0xC000
#define IGNORED_BITS 0xCF
#define UNUSED_BIT 0x20
#define B_BIT 0x10
#define ALL_P_FLAGS_SET_B_FLAG_UNSET 0xEF

#define STACK_START 0x0100
#define STACK_END_OFFSET 0xFF
#define STATUS_REGISTER_POWER_UP_STATE 0x34

#define IRQ_VECTOR 0xFFFE
#define NMI_VECTOR 0xFFFA
#define RESET_VECTOR 0xFFFC

#define INTERRUPT_LATENCY 7

using namespace std;

class CPU
{
public:
	enum addressing_mode_t {
			NONE,
			ABSOLUTE,
			ABSOLUTE_X,
			ABSOLUTE_Y,
			ACCUMULATOR,
			IMMEDIATE,
			IMPLIED,
			INDEXED_INDIRECT,
			INDIRECT,
			INDIRECT_INDEXED,
			RELATIVE,
			ZERO_PAGE,
			ZERO_PAGE_X,
			ZERO_PAGE_Y
		};

	CPU();
    CPU(Cartridge *);

	uint8_t read_memory(uint16_t); // Should be private and inline
	void write_memory(uint16_t, uint8_t); // Should be private and inline
	uint16_t calculate_address(int, uint8_t, uint8_t); // should be private

	// Load and store
	void lda(uint16_t, bool); // Load A with value at memory location
	void ldx(uint16_t, bool); // Load X with value at memory location
	void ldy(uint16_t, bool); // Load Y with value at memory location
	void sta(uint16_t); // Store A at memory location
	void stx(uint16_t); // Store X at memory location
	void sty(uint16_t); // Store Y at memory location

	// Arithmetic
	void adc(uint16_t, bool); // Add to A with carry bit
	void sbc(uint16_t, bool); // Subtract from A with carry bit

	// Increment and decrement
	void inc(uint16_t); // Increment value at memory location
	void inx(); 		// Increment X
	void iny(); 		// Increment Y
	void dec(uint16_t); // Decrement value at memory location
	void dex(); 		// Decrement X
	void dey(); 		// Decrement Y

	// Logical
	void _and(uint16_t, bool);// AND value at memory location with A
	void ora(uint16_t, bool); // OR value at memory location with A
	void eor(uint16_t, bool); // XOR value at memory location with A

	// Jump, branch, compare and test
	void jmp(uint16_t); // Jump to address
	void cmp(uint16_t, bool); // Compare value at memory location with A
	void cpx(uint16_t, bool); // Compare value at memory location with X
	void cpy(uint16_t, bool); // Compare value at memory location with Y
	void bcc(int8_t);	// Branch if C = 0
	void bcs(int8_t);  // Branch if C = 1
	void beq(int8_t);  // Branch if Z = 1
	void bne(int8_t);  // Branch if Z = 0
	void bmi(int8_t);  // Branch if N = 1
	void bpl(int8_t);  // Branch if N = 0
	void bvs(int8_t);  // Branch if V = 1
	void bvc(int8_t);  // Branch if V = 0
	void bit(uint16_t);  // Tests bits of value with A, sets flags only

	// Shift and rotate
	void asl(uint16_t, bool); // Shift A left 1 bit
	void lsr(uint16_t, bool); // Shift A right 1 bit
	void rol(uint16_t, bool); // Rotate A left 1 bit
	void ror(uint16_t, bool); // Rotate A right 1 bit

	// Transfer
	void tax(); // Move A to X
	void tay(); // Move A to Y
	void txa(); // Move X to A
	void tya(); // Move Y to A

	// Stack
	void tsx(); // Move stack pointer to X
	void txs(); // Move X to stack pointer
	void pha(); // Push A onto stack
	void php(); // Pulls A from stack
	void plp(); // Pulls P from stack
	void pla();	// Pulls A from stack

	// Subroutine
	void jsr(uint16_t); // Jump to subroutine
	void rts(); 		// Return from subroutine
	void rti(); 		// Return from interrupt

	// Set and reset
	void clc(); // Clear C flag
	void cld(); // Clear D mode
	void cli(); // Clear I
	void clv(); // Clear V
	void sec(); // Set C
	void sed(); // Set D
	void sei(); // Set I

	// NOP and BRK
	void nop(); // No operation
	void brk(); // Break

	// Utils
	void set_Z_flag(uint8_t);
	void set_N_flag(uint8_t);
	void set_P_flags(uint8_t);
	void push(uint8_t);
	void push_address(uint16_t);
	uint8_t pull();
	uint16_t pull_address();
	void update_P();
	void print_state();
	string hello_world();

	// Functions to be used with unit tests
	uint16_t get_PC();
	uint8_t get_SP();
	uint16_t get_P();
	uint8_t get_X();
	uint8_t get_Y();
	uint8_t get_A();
	int get_clock_cycles();
	bool is_N();
	bool is_C();
	bool is_Z();
	bool is_I();
	bool is_D();
	bool is_B();
	bool is_V();
	bool is_unused();

	void set_X(uint8_t);
	void set_Y(uint8_t);
	void set_A(uint8_t);
	void set_PC(uint16_t);
	void set_SP(uint8_t);
	void set_P(uint8_t);
	void set_C(bool);
	void set_Z(bool);
	void set_N(bool);
	void set_V(bool);

	static uint8_t rot_l(uint8_t);
	static uint8_t rot_r(uint8_t);
	void fetch_and_execute();
	void load_prg_bank_lower(std::vector<uint8_t> &);
	void load_prg_bank_upper(std::vector<uint8_t> &);

protected:

private:
	// Registers
	uint16_t PC; // Program counter, 16 bit
	uint8_t SP; // Stack pointer, 8 bit, offset from 0x0100
	uint8_t P; // processor status / status register, 8 bit
	uint8_t A;  // accumulator, 8 bit
	uint8_t X;  // index register, 8 bit
	uint8_t Y;  // index register, 8 bit

	uint16_t base_address;
	uint16_t final_address;

	bool N_flag;
	bool C_flag;
	bool Z_flag;
	bool I_flag;
	bool D_flag;
	bool B_flag;
	bool V_flag;
	bool S_flag;
	bool unused_flag;

	int clock_cycle;
    Memory *memory;

	// Length of an instruction in bytes
	static const unsigned char instruction_length[];
	// How long each instruction takes to execute
	static const unsigned char execution_time[];
	static const unsigned char opcode_addressing_mode[];
	static const std::string instruction_names[];
	void execute(uint8_t, uint16_t);
	bool pages_differ(uint16_t, uint16_t);
	uint16_t calculate_address_buggy(uint16_t);
	void increment_on_page_crossing();

	std::ofstream f;
	int linenum;
	void write_state();

};

#endif
