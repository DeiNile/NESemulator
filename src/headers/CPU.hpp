#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <stdint.h>

#define MEM_SIZE 65536
#define MAX_BYTE_VAL 256
#define MAX_SIGNED_BYTE_VAL 127
#define MIN_SIGNED_BYTE_VAL -127
#define ZERO_PAGE_WRAPAROUND 0xFF
#define STACK_POINTER_WRAPAROUND 0xFF
#define BIT_8_MASK 0x80
#define BYTE_LENGTH 8
#define BIT_0_TO_7 7


#define STACK_START 0x0100
#define STACK_END 0x0200

#define INTERRUPT_VECTOR 0xFFFE

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

	uint8_t read_memory(uint16_t); // Should be private and inline
	void write_memory(uint16_t, uint8_t); // Should be private and inline
	int resolve_operand(int, int); // should be private

	// Load and store
	void lda(uint16_t); 	// Load A with value at memory location
	void ldx(uint16_t); 	// Load X with value at memory location
	void ldy(uint16_t); 	// Load Y with value at memory location
	void sta(uint16_t); 	// Store A at memory location
	void stx(uint16_t); 	// Store X at memory location
	void sty(uint16_t); 	// Store Y at memory location

	// Arithmetic
	void adc(uint16_t); 	// Add to A with carry bit
	void sbc(uint16_t); 	// Subtract from A with carry bit

	// Increment and decrement
	void inc(uint16_t); 	// Increment value at memory location
	void inx(); 	// Increment X
	void iny(); 	// Increment Y
	void dec(uint16_t); 	// Decrement value at memory location
	void dex(); 	// Decrement X
	void dey(); 	// Decrement Y

	// Logical
	void _and(uint16_t); // AND value at memory location with A
	void ora(uint16_t); 	// OR value at memory location with A
	void eor(uint16_t); 	// XOR value at memory location with A

	// Jump, branch, compare and test
	void jmp(uint16_t);  // Jump to address
	void cmp(uint16_t);  // Compare value at memory location with A
	void cpx(uint16_t);  // Compare value at memory location with X
	void cpy(uint16_t);  // Compare value at memory location with Y
	void bcc(uint8_t);  // Branch if C = 0
	void bcs(uint8_t);  // Branch if C = 1
	void beq(uint8_t);  // Branch if Z = 1
	void bne(uint8_t);  // Branch if Z = 0
	void bmi(uint8_t);  // Branch if N = 1
	void bpl(uint8_t);  // Branch if N = 0
	void bvs(uint8_t);  // Branch if V = 1
	void bvc(uint8_t);  // Branch if V = 0
	void bit(uint16_t);  // Tests bits of value with A, sets flags only

	// Shift and rotate
	void asl(uint16_t, bool); 	// Shift A left 1 bit
	void lsr(uint16_t, bool); 	// Shift A right 1 bit
	void rol(uint16_t, bool); 	// Rotate A left 1 bit
	void ror(uint16_t, bool); 	// Rotate A right 1 bit

	// Transfer
	void tax(); 	// Move A to X
	void tay(); 	// Move A to Y
	void txa(); 	// Move X to A
	void tya(); 	// Move Y to A

	// Stack
	void tsx(); 	// Move stack pointer to X
	void txs(); 	// Move X to stack pointer
	void pha(); 	// Push A onto stack
	void php(); 	// Pulls A from stack
	void plp(); 	// Pulls PS from stack

	// Subroutine
	void jsr(uint16_t); 	// Jump to subroutine
	void rts(uint16_t); 	// Return from subroutine
	void rti(uint16_t); 	// Return from interrupt

	// Set and reset
	void clc(); 	// Clear C flag
	void cld(); 	// Clear D mode
	void cli(); 	// Clear I
	void clv(); 	// Clear V
	void sec(); 	// Set C
	void sed(); 	// Set D
	void sei(); 	// Set I

	// NOP and BRK
	void nop(); 	// No operation
	void brk(); 	// Break

	// Utils
	void set_Z_flag(uint8_t);
	void set_N_flag(uint8_t);
	void set_PS_flags(uint8_t);
	void push(int);
	int pull();
	void update_PS();
	void print_state();
	string hello_world();

	// Functions to be used with unit tests
	uint16_t get_PC();
	uint16_t get_SP();
	uint16_t get_PS();
	uint8_t get_X();
	uint8_t get_Y();
	uint8_t get_A();
	bool is_N();
	bool is_C();
	bool is_Z();
	bool is_I();
	bool is_D();
	bool is_B();
	bool is_V();
	bool is_S();

	void set_X(uint8_t);
	void set_Y(uint8_t);
	void set_A(uint8_t);
	void set_PC(uint16_t);
	void set_C(bool);
	void set_Z(bool);
	void set_N(bool);
	void set_V(bool);

protected:

private:
	// Registers
	uint16_t PC; // Program counter, 16 bit
	uint16_t SP; // Stack pointer, 8 bit, offset from 0x0100
	uint8_t PS; // processor status / status register, 8 bit
	uint8_t A;  // accumulator, 8 bit
	uint8_t X;  // index register, 8 bit
	uint8_t Y;  // index register, 8 bit

	bool N_flag;
	bool C_flag;
	bool Z_flag;
	bool I_flag;
	bool D_flag;
	bool B_flag;
	bool V_flag;
	bool S_flag;

	int clock_cycle;
	static uint8_t memory[];

	// Length of an instruction in bytes
	static const unsigned char instruction_length[];
	// How long each instruction takes to execute
	static const unsigned char execution_time[];
	static const unsigned char opcode_addressing_mode[];

};

#endif