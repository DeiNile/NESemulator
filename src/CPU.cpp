#include "headers/CPU.h"

void CPU::clc()
{
	C = false;
}

void CPU::cld()
{
	D = false;
}

void CPU::cli()
{
	I = false;
}

void CPU::clv()
{
	V = false;
}

void CPU::sec()
{
	C = true;
}

void CPU::sed()
{
	D = true;
}

void CPU::sei()
{
	I = true;
}

void CPU::inx()
{
	X++;
}

void CPU::iny()
{
	Y++;
}

void CPU::dex()
{
	X--;
}

void CPU::dey()
{
	Y--;
}