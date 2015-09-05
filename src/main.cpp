#include <iostream>
#include "headers/Cartridge.h"

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
	Cartridge rom(argv[ROM_INDEX]);
	rom.read_header();

	cout << "Hello world" << endl;
}