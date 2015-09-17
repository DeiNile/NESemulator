#include <cstdint>

#ifndef CPU_H
#define CPU_H

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
	int resolve_operand(int, int);

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
	void dec(int); 	// Decrement value at memory location
	void dex(); 	// Decrement X
	void dey(); 	// Decrement Y

	// Logical
	void _and(int); // AND value at memory location with A
	void ora(int); 	// OR value at memory location with A
	void eor(int); 	// XOR value at memory location with A

	// Jump, branch, compare and test
	void jmp(int);  // Jump to address
	void cmp(int);  // Compare value at memory location with A
	void cpx(int);  // Compare value at memory location with X
	void cpy(int);  // Compare value at memory location with Y
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
	void asl(int, bool); 	// Shift A left 1 bit
	void lsr(int, bool); 	// Shift A right 1 bit
	void rol(int, bool); 	// Rotate A left 1 bit
	void ror(int, bool); 	// Rotate A right 1 bit

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
	void set_Z_flag(int);
	void set_N_flag(int);
	void push(int);
	int pull();

protected:

private:
	// Registers
	int PC; // Program counter, 16 bit
	int SP; // Stack pointer, 8 bit, offset from 0x0100
	int PS; // processor status / status register, 8 bit
	int A;  // accumulator, 8 bit
	int X;  // index register, 8 bit
	int Y;  // index register, 8 bit

	bool N_flag;
	bool C_flag;
	bool Z_flag;
	bool I_flag;
	bool D_flag;
	bool B_flag;
	bool V_flag;
	bool S_flag;

	int opcode;
	int clock_cycle;
	static int memory[];

	// Length of an instruction in bytes
	static const unsigned char instruction_length[];
	// How long each instruction takes to execute
	static const unsigned char execution_time[];
	static const unsigned char opcode_addressing_mode[];

};

#endif