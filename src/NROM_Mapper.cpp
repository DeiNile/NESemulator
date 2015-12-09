#include "headers/NROM_Mapper.hpp"
#include <cmath>
#include <iostream>
#include <stdio.h>

#define LOWEST_THREE_BYTES 0x0FFF
#define LOWEST_THREE_BITS 0x7
#define HIGHEST_BYTE_TO_LOWEST 12
#define BYTE_LENGTH 8

NROM_Mapper::NROM_Mapper() : Memory_Mapper()
{
    prg_banks.resize(NUM_BANKS);
    for (int i = 0; i < NUM_BANKS; i++) {
        prg_banks.at(i).resize(PRG_BANK_SIZE);
    }

    chr_banks.resize(NUM_BANKS);
    for (int i = 0; i < NUM_BANKS; i++) {
        chr_banks.at(i).resize(CHR_BANK_SIZE);
    }
}

NROM_Mapper::NROM_Mapper(Cartridge *cart) : Memory_Mapper(cart)
{
    prg_banks.resize(NUM_BANKS);
	set_mirrored(cart->get_header().get_prg_rom_size_16KB());
	load_prg(cart->get_prg_rom());
    cartridge = NULL;
}

NROM_Mapper::~NROM_Mapper()
{
}

inline uint8_t NROM_Mapper::read(uint16_t address) 
{
    int bank = calc_prg_bank(address);
    int addr = calc_prg_address(address);
	return prg_banks.at(bank).at(addr);
}

uint16_t NROM_Mapper::read_address(uint16_t address)
{
    uint16_t ret = read(address + 1) << BYTE_LENGTH;
    ret |= read(address);
    return ret;
}

inline void NROM_Mapper::write(uint16_t address, uint8_t val)
{
    int bank = calc_prg_bank(address);
    int addr = calc_prg_address(address);
	prg_banks.at(bank).at(addr) = val;
	if (mirrored) {
		prg_banks.at(!calc_prg_bank(address)).at(calc_prg_address(address)) = val;
	}
}

void NROM_Mapper::load_prg(const std::vector<uint8_t> &v)
{
    for (int i = 0; i < 4; i++) {
        prg_banks.at(i) = std::vector<uint8_t>(v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
    }
    if (mirrored) {
        for (int i = 0; i < 4; i++) {
            prg_banks.at(i + 4) = std::vector<uint8_t>(
                                        v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
        }
    } else {
        for (int i = 4; i < NUM_BANKS; i++) {
            prg_banks.at(i) = std::vector<uint8_t>(v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
        }
    }
}

void NROM_Mapper::set_mirrored(int num_banks)
{
    mirrored = (num_banks == 1);
}


int NROM_Mapper::calc_prg_bank(uint16_t address)
{
    return (address >> HIGHEST_BYTE_TO_LOWEST) & LOWEST_THREE_BITS;
}


int NROM_Mapper::calc_prg_address(uint16_t address)
{
    return address & LOWEST_THREE_BYTES;
}

void NROM_Mapper::load_chr(const std::vector<uint8_t> &v)
{
    for (int i = 0; i < NUM_BANKS; i++) {
        chr_banks.at(i) = std::vector<uint8_t>(v.begin() + (i * CHR_BANK_SIZE),
                                        v.begin() + ((i + 1) * CHR_BANK_SIZE));
    }
}

int NROM_Mapper::calc_chr_bank(uint16_t address)
{
    return (address >> 12) & LOWEST_THREE_BITS;
    // return 0;
}

int NROM_Mapper::calc_chr_address(uint16_t address)
{
    return (address & 0xFFF);
    // return 0;
}