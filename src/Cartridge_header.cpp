#include "headers/Cartridge_header.hpp"
#include <iostream>

Header::Header(uint8_t bytes[])
{
	int i = 0;
	for (; i < NES_NAME_LENGTH; i++) {
		nes_name += bytes[i];
	}
	PRG_ROM_size_16KB = bytes[i++];
	CHR_size_8KB      = bytes[i++];
	flag_6            = bytes[i++];
	flag_7            = bytes[i++];
	PRG_RAM_size_8KB  = bytes[i++];
	flag_9            = bytes[i++];
	update_flags();
}

int Header::get_CHR_size_8KB() 
{
	return CHR_size_8KB;
}

int Header::get_PRG_RAM_size_8KB()
{
	return PRG_RAM_size_8KB;	
}

int Header::get_PRG_ROM_size_16KB()
{
	return PRG_ROM_size_16KB;
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

uint8_t Header::get_nybble()
{
	return nybble;
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
	return battery_packed_PRG_RAM;
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


void Header::update_flags()
{
	uint8_t temp = flag_6;
	if ((temp & MIRRORING_INDEX_BIT) == 0) {
		arrangement_mirroring = (temp & 1);
	} else {
		four_screen_VRAM = true;
	}
	temp >>= 1;
	battery_packed_PRG_RAM = (temp & 1);
	temp >>= 1;
	trainer = (temp & 1);

	temp = flag_7;
	unisystem = (temp & 1);
	temp >>= 1;
	playchoice = (temp & 1);

	tv_system = (flag_9 & 1);
	if ((flag_7 & NES2_FORMAT_MASK) == NES2_FORMAT_MASK) {
		nes2 = true;
	} else {
		nes2 = false;
	}

	nybble = (flag_7 & UPPER_NYBBLE_MASK) | (flag_6 >> NYBBLE_LENGTH);
}

