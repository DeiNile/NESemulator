#include "headers/CPU_Memory.hpp"
#include "headers/NROM_Mapper.hpp"
#include <iostream>

#define PPU_START_ADDRESS 0x2000
#define PPU_END_ADDRESS 0x3FFF
#define RAM_END_ADDRESS 0x07FF
#define IO_START_ADDRESS 0x4000
#define IO_END_ADDRESS 0x4020

#define RAM_ORIGIN 0x07FF
#define RAM_MIRROR_1 0x0FFF
#define RAM_MIRROR_2 0x17FF
#define RAM_MIRROR_3 0x1FFF

#define PPU_MIRROR_INTERVAL 8
#define PPU_REGISTER_MASK 0x7
#define MOST_SIGNIFICANT_BIT 0x8000
#define MEM_SIZE 0x8000


CPU_Memory::CPU_Memory() : Memory()
{
	memory.resize(MEM_SIZE);
	prg_rom = new NROM_Mapper();
}

CPU_Memory::CPU_Memory(Cartridge *cart) : Memory()
{
	memory.resize(MEM_SIZE);
	prg_rom = new NROM_Mapper(cart);
}

	CPU_Memory::~CPU_Memory()
{
	delete prg_rom;
}

inline uint8_t CPU_Memory::read(uint16_t address)
{
	if (address < MEM_SIZE) {
		return memory.at(address);
	} else {
		return prg_rom->read(address);
	}
}

inline void CPU_Memory::write(uint16_t address, uint8_t val)
{
	if (address > MEM_SIZE) {
		prg_rom->write(address, val);
	} else if (address >= 0 && address <= RAM_END_ADDRESS) {
		ram_write_mirror(address, val);
	} else if (address >= PPU_START_ADDRESS && address <= PPU_END_ADDRESS) {
		ppu_write_mirror(address, val);
	} else {
		memory.at(address) = val;
	}
}

inline void CPU_Memory::ram_write_mirror(const uint16_t address, const uint8_t val)
{
	memory.at(address & RAM_ORIGIN)   = val;
	memory.at(address & RAM_MIRROR_1) = val;
	memory.at(address & RAM_MIRROR_2) = val;
	memory.at(address & RAM_MIRROR_3) = val;
}

inline void CPU_Memory::ppu_write_mirror(const uint16_t address, const uint8_t val)
{
	memory.at(address) = val;
	uint8_t reg = (address & PPU_REGISTER_MASK);
	for (uint16_t i = PPU_START_ADDRESS + reg; i < IO_START_ADDRESS; 
		i += PPU_MIRROR_INTERVAL) {
		memory.at(i) = val;
	}
}