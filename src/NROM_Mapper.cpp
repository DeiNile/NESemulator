#include "headers/NROM_Mapper.hpp"
#include <cmath>
#include <iostream>
#include <stdio.h>

NROM_Mapper::NROM_Mapper(Cartridge &cart)
{
    banks.resize(NUM_BANKS);
	set_mirrored(cart.get_header().get_prg_rom_size_16KB());
	load_prg(cart.get_prg_rom());
}

uint8_t NROM_Mapper::read(uint16_t address) 
{
	return banks.at(calc_bank(address)).at(calc_address(address));
}

void NROM_Mapper::write(uint16_t address, uint8_t val)
{
	banks.at(calc_bank(address)).at(calc_address(address)) = val;
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
            banks.at(i + 4) = std::vector<uint8_t>(v.begin() + (i * PRG_BANK_SIZE),
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

inline int NROM_Mapper::calc_bank(uint16_t address)
{
    return ( address >> 12 ) & 0xF;
}

inline int NROM_Mapper::calc_address(uint16_t address)
{
    return address & 0x0FFF;
}
