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

class Cartridge
{
public:
	Cartridge(string);
	void readHeader();
	int getSizeOfPRG();
	int getSizeOfCHR();
	int getSizeOfPRGram();
protected:
private:
	ifstream rom;
	unsigned char *header; // cartridge header
};

#endif