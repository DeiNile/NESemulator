#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "Cartridge_header.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

#define KB_8 8192
#define KB_16 16384
#define CART_HEADER_SIZE 16
#define PRG_INDEX 4
#define chr_INDEX 5
#define NES_NAME_INDEX 4
#define TRAINER_SIZE 512
#define PRG_ROM_UNITS 16384 // 16 * 1024
#define CHR_ROM_UNITS 8192 // 8 * 1024
#define PRG_RAM_UNITS 8192
#define PLAYCHOICE_SIZE 8192

class Cartridge
{
public:
	Cartridge(std::string);
	~Cartridge();

	Header get_header();
	std::vector<uint8_t> get_trainer();
	std::vector<uint8_t> get_prg_rom();
	std::vector<uint8_t> get_chr_rom();
	std::vector<uint8_t> get_prg_ram();
	std::vector<uint8_t> get_playchoice();
	bool has_chr_rom();
protected:
private:
	std::ifstream rom; // stream for the ROM
	Header *header;
	std::vector<uint8_t> trainer_data;
	std::vector<uint8_t> prg_rom_data;
	std::vector<uint8_t> chr_rom_data;
	std::vector<uint8_t> prg_ram_data;
	std::vector<uint8_t> playchoice_data;

	void load_trainer();
	void load_prg_rom();
	void load_chr_rom();
	void load_prg_ram();
	void load_playchoice();
};

#endif