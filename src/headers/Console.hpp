#ifndef CONSOLE_H
#define CONSOLE_H

#include "Cartridge.hpp"
#include "CPU.hpp"
#include "PPU.hpp"

class CPU;
class PPU;


class Console
{
public:
	Console();
	Console(Cartridge *);
	~Console();

	void load_cartridge(Cartridge *);
	void reset();
	void power_on();

	CPU* get_cpu();
	PPU *get_ppu();
protected:
private:
	Cartridge *cart;
	CPU *cpu;
	PPU *ppu;
};

#endif