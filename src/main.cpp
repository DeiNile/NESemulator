#include <iostream>
#include "headers/Utils.hpp"
#include "headers/Cartridge.hpp"
#include "headers/CPU.hpp"
#include "headers/NROM_Mapper.hpp"
#include "headers/Console.hpp"
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
	Console console(&cart);
	console.get_cpu()->set_PC(0xC000);
	console.power_on();
}

