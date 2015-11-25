#include "headers/NROM_Mapper.hpp"
#include <cmath>
#include <iostream>
#include <stdio.h>

#define LOWEST_THREE_BYTES 0x0FFF
#define LOWEST_THREE_BITS 0x7
#define HIGHEST_BYTE_TO_LOWEST 12

NROM_Mapper::NROM_Mapper() : Memory_Mapper()
{
    banks.resize(NUM_BANKS);
    for (int i = 0; i < NUM_BANKS; i++) {
        banks.at(i).resize(PRG_BANK_SIZE);
    }
}

NROM_Mapper::NROM_Mapper(Cartridge *cart) : Memory_Mapper(cart)
{
    banks.resize(NUM_BANKS);
	set_mirrored(cart->get_header().get_prg_rom_size_16KB());
	load_prg(cart->get_prg_rom());
    cartridge = NULL;
}

NROM_Mapper::~NROM_Mapper()
{
}

inline uint8_t NROM_Mapper::read(uint16_t address) 
{
    int bank = calc_bank(address);
    int addr = calc_address(address);
	return banks.at(bank).at(addr);
}

inline void NROM_Mapper::write(uint16_t address, uint8_t val)
{
    int bank = calc_bank(address);
    int addr = calc_address(address);
	banks.at(bank).at(addr) = val;
	if (mirrored) {
		banks.at(!calc_bank(address)).at(calc_address(address)) = val;
	}
}

void NROM_Mapper::load_prg(const std::vector<uint8_t> &v)
{
    for (int i = 0; i < 4; i++) {
        banks.at(i) = std::vector<uint8_t>(v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
    }
    if (mirrored) {
        for (int i = 0; i < 4; i++) {
            banks.at(i + 4) = std::vector<uint8_t>(
                                        v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
        }
    } else {
        for (int i = 4; i < NUM_BANKS; i++) {
            banks.at(i) = std::vector<uint8_t>(v.begin() + (i * PRG_BANK_SIZE),
                                        v.begin() + ((i + 1) * PRG_BANK_SIZE));
        }
    }
}

void NROM_Mapper::set_mirrored(int num_banks)
{
    mirrored = (num_banks == 1);
}


int NROM_Mapper::calc_bank(uint16_t address)
{
    return (address >> HIGHEST_BYTE_TO_LOWEST) & LOWEST_THREE_BITS;
}


int NROM_Mapper::calc_address(uint16_t address)
{
    return address & LOWEST_THREE_BYTES;
}