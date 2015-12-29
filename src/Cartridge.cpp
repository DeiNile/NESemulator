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

/*
 * Retrieve the cartridge's header.
 *
 * @return header The header
 */
Header Cartridge::get_header()
{
	return *header;
}

/*
 * Retrieve all the trainer data for the cartridge.
 *
 * @return The trainer data
 */
std::vector<uint8_t> Cartridge::get_trainer()
{
	return trainer_data;
	// if (header != NULL && header->is_trainer_present()) {
	// } else {
	// 	return NULL;
	// }
}

/*
 * Retrive the program rom from the cartridge.
 *
 * @return prg_rom_data The program rom
 */
std::vector<uint8_t> Cartridge::get_prg_rom()
{
	return prg_rom_data;
}

/*
 * Retrives the character rom from the cartridge.
 *
 * @return chr_rom_data The cahracter data
 */
std::vector<uint8_t> Cartridge::get_chr_rom()
{
	return chr_rom_data;
}

/*
 * Retrive the program ram from the cartridge.
 *
 * @return Program ram
 */
std::vector<uint8_t> Cartridge::get_prg_ram()
{
	return prg_ram_data;
}

/*
 * Retrieve the Playchoice data from the cartridge.
 *
 * @return playchoice_data
 */
std::vector<uint8_t> Cartridge::get_playchoice()
{
	return playchoice_data;
}

/*
 * Check if the cartridge has character ROM.
 *
 * @return True if character ROM is present
 */
bool Cartridge::has_chr_rom()
{
	if (header->get_chr_size_8KB() == 0) {
		return false;
	} else {
		return true;
	}
}

/*
 * Reads the trainer from the file and stores it in memory.
 */
void Cartridge::load_trainer()
{
	if (header->is_trainer_present()) {
		trainer_data.resize(TRAINER_SIZE);
		rom.read((char *)&(trainer_data[0]), TRAINER_SIZE);
		// rom.read((char *)trainer_data, TRAINER_SIZE);
	}
}

/*
 * Reads the program data from the cartridge and stores it in memory.
 */
void Cartridge::load_prg_rom()
{
	int size = header->get_prg_rom_size_16KB() * PRG_ROM_UNITS;
	if (header != NULL && size > 0) {
		prg_rom_data.resize(size);
		rom.read((char *)&(prg_rom_data[0]), size);
	}
}

/*
 * Reads the character ROM from the cartrdige and stores it in memory.
 */
void Cartridge::load_chr_rom()
{
	int size = header->get_chr_size_8KB() * CHR_ROM_UNITS;
	if (header != NULL && size > 0) {
		chr_rom_data.resize(size);
		rom.read((char *)&(chr_rom_data[0]), size);
	}
}

/*
 * Reads teh program RAM from the cartridge and stores it in memory.
 */
void Cartridge::load_prg_ram()
{
	int size = header->get_prg_ram_size_8KB() * PRG_RAM_UNITS;
	if (header != NULL && size > 0) {
		prg_ram_data.resize(size);
		rom.read((char *)&(prg_ram_data[0]), size);
	}
}

/*
 * Reads the Playchoice from the cartridge and stores it in memmory.
 */
void Cartridge::load_playchoice()
{
	if (header != NULL && header->is_playchoice()) {
		playchoice_data.resize(PLAYCHOICE_SIZE);
		rom.read((char *)&(playchoice_data[0]), PLAYCHOICE_SIZE);
	}
}