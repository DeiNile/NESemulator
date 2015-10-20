#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#include "headers/Cartridge.hpp"

using namespace std;

Cartridge::Cartridge(string path)
{
	cerr << path << endl;
	if (!path.empty()) {
		rom.open(path, ios::in | ios::binary);
		if (!rom.is_open()) {
			cerr << "Could not open: " << path << endl;
			exit(1);
		}
	} else {
		cerr << "No file path" << endl;
		exit(1);
	}
}

Cartridge::~Cartridge()
{
	if (rom.is_open()) {
		rom.close();
	}
}


bool Cartridge::read_header()
{
	if (rom.is_open()) {
		header = new char[CART_HEADER_SIZE];
		rom.read(header, CART_HEADER_SIZE);
		return validate_header();
	} else {
		return false;
	}
}

bool Cartridge::validate_header()
{
	if (header != NULL) {
		if (0 == strncmp(header, "NES\x1A", NES_NAME_INDEX)) {
			return true;
		}
	}
	return false;
}

int Cartridge::get_size_of_PRG_16KB()
{
	if (header != NULL) {
		return (int)header[PRG_INDEX] * KB_16;
	} else {
		return -1;
	}
}

int Cartridge::getSizeOfCHR() 
{
	if (header != NULL) {
		return (int)header[CHR_INDEX] * KB_8;
	}
	return 0;
}

int Cartridge::get_size_of_PRG_8KB() 
{
	if (header != NULL) {
		return (int)header[PRG_INDEX] * KB_8;
	}
	return 0;
}