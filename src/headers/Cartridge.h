#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#define KB_8 8192
#define KB_16 16384
#define CART_HEADER_SIZE 16
#define PRG_INDEX 4
#define CHR_INDEX 5
#define NES_NAME_INDEX 4

class Cartridge
{
public:
	Cartridge(string);
	~Cartridge();
	bool read_header();
	int get_size_of_PRG_16KB();
	int getSizeOfCHR();
	int getSizeOfPRGram();
	int get_size_of_PRG_8KB();
protected:
private:
	ifstream rom; // stream for the ROM
	char *header; // cartridge header

	bool validate_header();
};

#endif