#include <cstdint>

#ifndef CPU:H
#define CPU:H

#define MEM_SIZE 65536
#define MAX_BYTE_VAL 256
#define ZERO_PAGE_WRAPAROUND 0xFF
#define BYTE_LENGTH 8

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

	static inline int read_memory(int);
	static inline void write_memory(int, int);

	// Load and store
	void lda(int); 	// Load A with value at memory location
	void ldx(int); 	// Load X with value at memory location
	void ldy(int); 	// Load Y with value at memory location
	void sta(int); 	// Store A at memory location
	void stx(int); 	// Store X at memory location
	void sty(int); 	// Store Y at memory location

	// Arithmetic
	void adc(int); 	// Add to A with carry bit
	void sbc(int); 	// Subtract from A with carry bit

	// Increment and decrement
	void inc(int); 	// Increment value at memory location
	void inx(); 	// Increment X
	void iny(); 	// Increment Y
	void dec(int) 	// Decrement value at memory location
	void dex(); 	// Decrement X
	void dey(); 	// Decrement Y

	// Logical
	int _and(int); 	// AND value at memory location with A
	int ora(int); 	// OR value at memory location with A
	int eor(int); 	// XOR value at memory location with A

	// Jump, branch, compare and test
	void jmp(int);  // Jump to address
	bool cmp(int);  // Compare value at memory location with A
	bool cpx(int);  // Compare value at memory location with X
	bool cpy(int);  // Compare value at memory location with Y
	void bcc(int);  // Branch if C = 0
	void bcs(int);  // Branch if C = 1
	void beq(int);  // Branch if Z = 1
	void bne(int);  // Branch if Z = 0
	void bmi(int);  // Branch if N = 1
	void bpl(int);  // Branch if N = 0
	void bvs(int);  // Branch if V = 1
	void bvc(int);  // Branch if V = 0
	void bit(int);  // Tests bits of value with A, sets flags only

	// Shift and rotate
	void asl(); 	// Shift A left 1 bit
	void lsr(); 	// Shift A right 1 bit
	void rol(); 	// Rotate A left 1 bit
	void ror(); 	// Rotate A right 1 bit

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
	void jsr(int); 	// Jump to subroutine
	void rts(int); 	// Return from subroutine
	void rti(int); 	// Return from interrupt

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
	unsigned int switch_operand(int);
protected:
private:
	// Registers
	int pc; // Program counter, 16 bit
	int sp; // Stack pointer, 8 bit, offset from 0x0100
	int ps; // processor status / status register, 8 bit
	int A;  // accumulator, 8 bit
	int X;  // index register, 8 bit
	int Y;  // index register, 8 bit

	bool C_flag;
	bool Z_flag;
	bool I_flag;
	bool D_flag;
	bool B_flag;
	bool V_flag;
	bool S_flag;

	int opcode;
	int clock_cycle;
	int memory[MEM_SIZE];

	// Length of an instruction in bytes
	const unsigned char[MAX_BYTE_VAL] instruction_length {
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

	// How long each instruction takes to execute
	const unsigned char[MAX_BYTE_VAL] execution_time {
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

	const unsigned char[MAX_BYTE_VAL] opcode_addressing_mode {
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
};

#endif