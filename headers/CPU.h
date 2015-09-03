#ifndef CPU:H
#define CPU:H
#include <cstdint>

class CPU
{
public:
	CPU();
	inline int readPC();
	inline int readSP();
	inline int readPS();
	inline int readA();
	inline int readX();
	inline int readY();
	inline int writePC(int);
	inline int writeSP(int);
	inline int writePS(int);
	inline int writeA(int);
	inline int writeX(int);
	inline int writeY(int);
protected:
private:
	// Registers
	int pc; // Program counter, 16 bit
	int sp; // Stack pointer, 8 bit
	int ps; // processor status, 8 bit
	int A; // accumulator, 8 bit
	int X; // index register, 8 bit
	int Y; // index register, 8 bit

	int opcode;
};

#endif