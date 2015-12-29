#include "headers/Cartridge_header.hpp"

Header::Header(std::vector<uint8_t> &bytes)
{
	int i = 0;
	for (; i < NES_NAME_LENGTH; i++) {
		nes_name += bytes.at(i);
	}
	prg_rom_size_16KB = bytes.at(i++);
	chr_size_8KB      = bytes.at(i++);
	flag_6            = bytes.at(i++);
	flag_7            = bytes.at(i++);
	prg_ram_size_8KB  = bytes.at(i++);
	flag_9            = bytes.at(i++);
	// update_flags();
	update_flag_6();
	update_flag_7();
	assemble_mapper_number();
}

int Header::get_chr_size_8KB() 
{
	return chr_size_8KB;
}

int Header::get_prg_ram_size_8KB()
{
	return prg_ram_size_8KB;	
}

int Header::get_prg_rom_size_16KB()
{
	return prg_rom_size_16KB;
}

uint8_t Header::get_flag_6()
{
	return flag_6;
}

uint8_t Header::get_flag_7()
{
	return flag_7;
}

uint8_t Header::get_flag_9()
{
	return flag_9;
}

uint8_t Header::get_mapper_num()
{
	return mapper_num;
}

bool Header::valid_header()
{
	if (nes_name == NES_NAME) {
		return true;
	} else {
		return false;
	}
}

bool Header::is_vertical()
{
	return arrangement_mirroring;
}

bool Header::is_horizontal()
{
	return !arrangement_mirroring;
}

bool Header::is_four_screen_VRAM()
{
	return four_screen_VRAM;
}

bool Header::is_battery_present()
{
	return battery_packed_prg_ram;
}

bool Header::is_trainer_present()
{
	return trainer;
}

bool Header::is_unisystem()
{
	return unisystem;
}

bool Header::is_playchoice()
{
	return playchoice;
}

bool Header::is_NTSC()
{
	return tv_system;
}

bool Header::is_PAL()
{
	return !tv_system;
}

bool Header::is_NES2_format()
{
	return nes2;
}

/*
 * Takes the flag_6 and flag_7 bytes and retrieves the flags from them.
 * In addition it assembles the mapper number that this cartridge is using.
 */
// void Header::update_flags()
// {
// 	uint8_t temp = flag_6;
// 	if ((temp & MIRRORING_INDEX_BIT) == 0) {
// 		arrangement_mirroring = (temp & 1);
// 	} else {
// 		four_screen_VRAM = true;
// 	}
// 	temp >>= 1;
// 	battery_packed_prg_ram = (temp & 1);
// 	temp >>= 1;
// 	trainer = (temp & 1);

// 	temp = flag_7;
// 	unisystem = (temp & 1);
// 	temp >>= 1;
// 	playchoice = (temp & 1);

// 	tv_system = (flag_9 & 1);
// 	if ((flag_7 & NES2_FORMAT_MASK) == NES2_FORMAT_MASK) {
// 		nes2 = true;
// 	} else {
// 		nes2 = false;
// 	}
// }

/*
 * Retrieves all the flags from the flag_6 byte and stores them.
 */
void Header::update_flag_6()
{
	uint8_t temp = flag_6;
	if ((temp & MIRRORING_INDEX_BIT) == 0) {
		arrangement_mirroring = (temp & 1);
	} else {
		four_screen_VRAM = true;
	}
	temp >>= 1;
	battery_packed_prg_ram = (temp & 1);
	temp >>= 1;
	trainer = (temp & 1);	
}

/*
 * Retrieves all the flags from the flag_7 byte and stores them.
 */
void Header::update_flag_7()
{
	uint8_t temp = flag_7;
	unisystem = (temp & 1);
	temp >>= 1;
	playchoice = (temp & 1);

	tv_system = (flag_9 & 1);
	if ((flag_7 & NES2_FORMAT_MASK) == NES2_FORMAT_MASK) {
		nes2 = true;
	} else {
		nes2 = false;
	}
}

/*
 * Takes the values of flag_6 and flag_7 to retrieve the mapper number for
 * this cartridge.
 */
void Header::assemble_mapper_number()
{
	mapper_num = (flag_7 & UPPER_NYBBLE_MASK) | (flag_6 >> NYBBLE_LENGTH);
}