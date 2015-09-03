#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#include "Cartridge.h"

using namespace std;

Cartridge::Cartridge(string path)
{
	if (!path.empty()) {
		rom.open(path, ios::in | ios::binary);
	}
}

void Cartridge::readHeader()
{
	if (rom.is_open()) {
		header = new char[CART_HEADER_SIZE];
		rom.read(header, CART_HEADER_SIZE);
		if () {

		}
	}
}

bool Cartridge::validateHeader() 
{
	if (header != NULL) {
		if (0 == strncmp(header, "NES\x1A")) {
			return true;
		}
	}
	return false;
}

int Cartridge::getSizeOfPRG()
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
}

int Cartridge::get_size_of_PRG_8KB() 
{
	if (header != NULL) {
		return (int)header[PRG_INDEX] * KB_8
	}
}