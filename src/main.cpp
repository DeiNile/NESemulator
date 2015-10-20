#include <iostream>
#include "headers/Utils.hpp"
#include "headers/Cartridge.hpp"
#include "headers/CPU.hpp"
#include <stdlib.h>
#include <time.h>

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

	std::cout << "hello\n";
	CPU cpu;
	uint8_t low;
	uint8_t high;
	uint8_t opcode;
	uint8_t value;
	uint8_t value_2;
	uint16_t pc;
	uint16_t address;

	srand(time(NULL));
	opcode  = 0;
	low     = (rand() % (UINT8_MAX + 1)) + 1;
	high    = (rand() % (UINT8_MAX + 1)) + 1;
	address = ((high << 8) | low);
	value   = (rand() % (UINT8_MAX + 1)) + 1;
	value_2 = (rand() % (UINT8_MAX + 1)) + 1;
	pc      = cpu.get_PC();
	cpu.write_memory(pc, opcode);
	cpu.write_memory(pc + 1, low);
	cpu.write_memory(pc + 2, high);

	cpu.write_memory(pc, ASL_ABSOLUTE);
	cpu.write_memory(address, value);
	cpu.execute_instruction();
}