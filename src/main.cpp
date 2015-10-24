#include <iostream>
#include "headers/Utils.hpp"
#include "headers/Cartridge.hpp"
#include "headers/CPU.hpp"
#include <stdlib.h>
#include <time.h>
#include <vector>

#define NUM_ARGS 2
#define ROM_INDEX 1

using namespace std;

int main(int argc, char *argv[]) 
{
	if (argc != NUM_ARGS) {
		cerr << "Invalid number of arguments. Please include a ROM file." 
			<< endl;
		return 1;
	}
	Cartridge cart(argv[1]);
	std::vector<uint8_t> rom = cart.get_prg_rom();
	CPU cpu;
	cpu.load_prg_bank_upper(rom);
	cpu.set_PC(0xC000);
	for (;;) {
		cpu.fetch_and_execute();
	}
}

