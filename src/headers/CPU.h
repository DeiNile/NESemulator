#include <cstdint>

#ifndef CPU:H
#define CPU:H


class CPU
{
public:
	CPU();
	inline int read_PC();
	inline int read_SP();
	inline int read_PS();
	inline int read_A();
	inline int read_X();
	inline int read_Y();
	inline int write_PC(int);
	inline int write_SP(int);
	inline int write_PS(int);
	inline int write_A(int);
	inline int write_X(int);
	inline int write_Y(int);

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
	int AND(int); 	// AND value at memory location with A
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

protected:
private:
	// Registers
	int pc; // Program counter, 16 bit
	int sp; // Stack pointer, 8 bit
	int ps; // processor status / status register, 8 bit
	int A; // accumulator, 8 bit
	int X; // index register, 8 bit
	int Y; // index register, 8 bit

	bool C_flag;
	bool Z_flag;
	bool I_flag;
	bool D_flag;
	bool B_flag;
	bool V_flag;
	bool S_flag;

	int opcode;
	int clock_cycle;
};

#endif