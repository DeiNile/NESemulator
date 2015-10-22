#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <iostream>

#define NES_NAME_LENGTH 4
#define HEADER_LENGTH 16
#define ZERO_FIELD_INDEX 11
#define MIRRORING_INDEX_BIT 0x8
#define NES2_FORMAT_MASK 0x8
#define NYBBLE_LENGTH 4
#define UPPER_NYBBLE_MASK 0xF0
#define NES_NAME "NES\x1A"
#define TRAINER_SIZE 512


class Header {
public:
	Header(uint8_t[]);
	int get_chr_size_8KB();
	int get_prg_ram_size_8KB(); // 8 KB
	int get_prg_rom_size_16KB(); // 16 KB
	uint8_t get_flag_6();
	uint8_t get_flag_7();
	uint8_t get_flag_9();
	uint8_t get_nybble();
	bool valid_header();
	bool is_vertical();
	bool is_horizontal();
	bool is_four_screen_VRAM();
	bool is_battery_present();
	bool is_trainer_present();
	bool is_unisystem();
	bool is_playchoice();
	bool is_NTSC();
	bool is_PAL();
	bool is_NES2_format();
protected:
private:
	int prg_rom_size_16KB;
	int chr_size_8KB;
	int prg_ram_size_8KB;
	uint8_t flag_6;
	uint8_t flag_7;
	uint8_t flag_9;
	uint8_t nybble;
	std::string nes_name;
	bool nes2;

	// Flags 6
	bool arrangement_mirroring; // 0 vertical, 1 horizontal
	bool four_screen_VRAM;
	bool battery_packed_prg_ram;
	bool trainer;

	// Flags 7
	bool unisystem;
	bool playchoice;

	// Flags 9
	bool tv_system; // 0 NTSC, 1 PAL

	void update_flags();
};

#endif