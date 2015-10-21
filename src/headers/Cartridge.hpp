#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "Cartridge_header.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define KB_8 8192
#define KB_16 16384
#define CART_HEADER_SIZE 16
#define PRG_INDEX 4
#define CHR_INDEX 5
#define NES_NAME_INDEX 4

class Cartridge
{
public:
	Cartridge(std::string);
	~Cartridge();
protected:
private:
	std::ifstream rom; // stream for the ROM
	Header *header;
};

#endif