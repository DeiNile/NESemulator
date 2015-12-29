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
#define MEMORY_SIZE 0x8000
#define BYTE_LENGTH 8

#define PPU_CTRL_MASK 0x00
#define PPU_MASK_MASK 0x01
#define PPU_STATUS_MASK 0x02
#define OAM_ADDR_MASK 0x03
#define OAM_DATA_MASK 0x04
#define PPU_SCROLL_MASK 0x05
#define PPU_ADDR_MASK 0x06
#define PPU_DATA_MASK 0x07
#define OAM_DMA_ADDR 0x4014


CPU_Memory::CPU_Memory(Console *con) : Memory()
{
	console = con;
	memory.resize(MEMORY_SIZE);
	prg_rom = new NROM_Mapper();
}

CPU_Memory::CPU_Memory(Cartridge *cart, Console *con) : Memory()
{
	memory.resize(MEMORY_SIZE);
	prg_rom = new NROM_Mapper(cart);
	console = con;
}

	CPU_Memory::~CPU_Memory()
{
	delete prg_rom;
}

/*
 * Retrieves a value from memory.
 * If the address is referencing the I/O mapped registers for the PPU,
 * the value will be retrieved from the PPU directly.
 *
 * @param address The address of the value to be read
 * @return A value from the CPU memory, or PPU register values
 */
uint8_t CPU_Memory::read(uint16_t address)
{
	// Check if the value is in the CPU's static memory
	if (address < MEMORY_SIZE) {
		// Check for reading of status, and data registers in the PPU
		if (address == 0x2007) {
			// Sets the value in memory to give the CPU an up to date view
			memory.at(0x2007) = console->get_ppu()->read_ppu_data();
			return memory.at(0x2007);
		} else if (address == 0x2002) {
			memory.at(0x2002) = console->get_ppu()->read_ppu_status();
			return memory.at(0x2002);
		} else {
			return memory.at(address);
		}
	// The value is in the CPU's program data
	} else {
		return prg_rom->read(address);
	}
}

/*
 * Retrieves an address from memory.
 * Does not check if it is attempted to read from PPU registers, or other I/O
 * mapped registers.
 * 
 * @param address The address of the value
 * @return ret The value stored at @address
 */
uint16_t CPU_Memory::read_address(uint16_t address)
{
	uint16_t ret = 0;
	if (address < MEMORY_SIZE) {
		ret = memory.at(address + 1) << BYTE_LENGTH;
		ret |= memory.at(address);
	} else {
		ret = prg_rom->read(address + 1) << BYTE_LENGTH;
		ret |= prg_rom->read(address);
	}
	return ret;
}

/*
 * Store a value in memory.
 * If the address references a PPU I/O mapped register, the associated 
 * setter is called directly in the PPU.
 * All values are properly mirrored if required.
 * 
 * @param address The destination address of the write
 * @param val The value that is to be stored
 */
void CPU_Memory::write(uint16_t address, uint8_t val)
{
	// Check if the value should be stored in program memory
	if (address > MEMORY_SIZE) {
		prg_rom->write(address, val);
	// Check if address is in RAM
	} else if (address >= 0 && address <= RAM_END_ADDRESS) {
		ram_write_mirror(address, val);
	// Check if address is in PPU's I/O mapped registers
	} else if (address >= PPU_START_ADDRESS && address <= PPU_END_ADDRESS) {
		ppu_write_mirror(address, val);
		
		// Set values in PPU
		switch(address & PPU_REGISTER_MASK) {
			case PPU_CTRL_MASK:
				console->get_ppu()->set_ppu_ctrl(val);
				break;
			case PPU_MASK_MASK:
				console->get_ppu()->set_ppu_mask(val);
				break;
			case OAM_ADDR_MASK:
				console->get_ppu()->set_oam_addr(val);
				break;
			case OAM_DATA_MASK:
				console->get_ppu()->set_oam_data(val);
				break;
			case PPU_SCROLL_MASK:
				console->get_ppu()->set_ppu_scroll(val);
				break;
			case PPU_ADDR_MASK:
				console->get_ppu()->set_ppu_addr(val);
				break;
			case PPU_DATA_MASK:
				console->get_ppu()->set_ppu_data(val);
				break;
			default:
				std::cerr << "Invalid PPU register address given <" <<
					std::hex << (int)address << ">" << std::endl;
		}
	} else if (address == OAM_DMA_ADDR) {
		memory.at(address) = val;
		console->get_ppu()->set_oam_dma(val);
	// Address is in CPU's static memory: no mirroring needed
	} else {
		memory.at(address) = val;
	}
}

/*
 * Mirrors value at 4 different sections of RAM. Each section is 2KB big.
 *
 * @param address The original address that was provided with the write. It
 *	may or may not be in the first RAM section (0x000 through 0x7FF)
 * @param val The value to be mirrored
 */
inline void CPU_Memory::ram_write_mirror(const uint16_t address, const uint8_t val)
{
	memory.at(address & RAM_ORIGIN)   = val;
	memory.at(address & RAM_MIRROR_1) = val;
	memory.at(address & RAM_MIRROR_2) = val;
	memory.at(address & RAM_MIRROR_3) = val;
}

/*
 * Mirrors the values of a PPU I/O mapped register value at 8 byte from 0x2000
 * through 0x3FFF.
 *
 * @param address The address of the original write
 * @param val The value to be mirrored
 */
inline void CPU_Memory::ppu_write_mirror(const uint16_t address, const uint8_t val)
{
	memory.at(address) = val;
	// Find the register that is being written to
	uint8_t reg = (address & PPU_REGISTER_MASK);
	for (uint16_t i = PPU_START_ADDRESS + reg; i < PPU_END_ADDRESS; 
			i += PPU_MIRROR_INTERVAL) {
		memory.at(i) = val;
	}
}