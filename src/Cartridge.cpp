#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#include "headers/Cartridge.hpp"

using namespace std;

Cartridge::Cartridge(string path)
{
	if (!path.empty()) {
		rom.open(path, ios::in | ios::binary);
		if (!rom.is_open()) {
			cerr << "Could not open: " << path << endl;
			exit(1);
		}
		std::vector<uint8_t> bytes(HEADER_LENGTH);
		rom.read((char *)&(bytes.at(0)), HEADER_LENGTH);
		header = new Header(bytes);
		load_trainer();
		load_prg_rom();
		load_chr_rom();
		load_prg_ram();
		load_playchoice();
	} else {
		cerr << "No file path" << endl;
		exit(1);
	}
}

Cartridge::~Cartridge()
{
	delete header;
	if (rom.is_open()) {
		rom.close();
	}
}

Header Cartridge::get_header()
{
	return *header;
}

std::vector<uint8_t> Cartridge::get_trainer()
{
	return trainer_data;
	// if (header != NULL && header->is_trainer_present()) {
	// } else {
	// 	return NULL;
	// }
}

std::vector<uint8_t> Cartridge::get_prg_rom()
{
	return prg_rom_data;
}

std::vector<uint8_t> Cartridge::get_chr_rom()
{
	return chr_rom_data;
}

std::vector<uint8_t> Cartridge::get_prg_ram()
{
	return prg_ram_data;
}

std::vector<uint8_t> Cartridge::get_playchoice()
{
	return playchoice_data;
}

bool Cartridge::has_chr_rom()
{
	if (header->get_chr_size_8KB() == 0) {
		return false;
	} else {
		return true;
	}
}

void Cartridge::load_trainer()
{
	if (header->is_trainer_present()) {
		trainer_data.resize(TRAINER_SIZE);
		rom.read((char *)&(trainer_data[0]), TRAINER_SIZE);
		// rom.read((char *)trainer_data, TRAINER_SIZE);
	}
}

void Cartridge::load_prg_rom()
{
	int size = header->get_prg_rom_size_16KB() * PRG_ROM_UNITS;
	if (header != NULL && size > 0) {
		// if ((prg_rom_data = (uint8_t *)malloc(sizeof(uint8_t) * size)) == NULL) {
		// 	std::cerr << "Memory allocation failed: PRG ROM" << std::endl;
		// }
		prg_rom_data.resize(size);
		rom.read((char *)&(prg_rom_data[0]), size);
		// rom.read((char *)prg_rom_data, size);
	}
}

void Cartridge::load_chr_rom()
{
	int size = header->get_chr_size_8KB() * CHR_ROM_UNITS;
	if (header != NULL && size > 0) {
		// if ((chr_rom_data = (uint8_t *)malloc(sizeof(uint8_t) * size)) == NULL) {
		// 	std::cerr << "Memory allocation failed: CHR ROM" << std::endl;
		// }
		chr_rom_data.resize(size);
		rom.read((char *)&(chr_rom_data[0]), size);
		// rom.read((char *)chr_rom_data, size);
	}
}

void Cartridge::load_prg_ram()
{
	int size = header->get_prg_ram_size_8KB() * PRG_RAM_UNITS;
	if (header != NULL && size > 0) {
		// if ((prg_ram_data = (uint8_t *)malloc(sizeof(uint8_t) * size)) == NULL) {
		// 	std::cerr << "Memory allocation failed: PRG RAM" << std::endl;
		// }
		prg_ram_data.resize(size);
		rom.read((char *)&(prg_ram_data[0]), size);
		// rom.read((char *)prg_ram_data, size);
	}
}

void Cartridge::load_playchoice()
{
	if (header != NULL && header->is_playchoice()) {
		// if ((playchoice_data = (uint8_t *)malloc(sizeof(uint8_t) * 
		// 		PLAYCHOICE_SIZE)) == NULL) {
		// 	std::cerr << "Memory allocation failed: PLAYCHOICE" << std::endl;
		// }
		// rom.read((char *)playchoice_data, PLAYCHOICE_SIZE);
		playchoice_data.resize(PLAYCHOICE_SIZE);
		rom.read((char *)&(playchoice_data[0]), PLAYCHOICE_SIZE);
	}
}