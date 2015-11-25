#ifndef CPU_MEMORY_H
#define CPU_MEMORY_H

#include "Memory.hpp"
#include "Cartridge.hpp"
#include "Memory_Mapper.hpp"

class CPU_Memory: public Memory 
{
public:
	CPU_Memory();
	CPU_Memory(Cartridge *);
	virtual ~CPU_Memory();

	virtual uint8_t read(uint16_t);
	virtual void write(uint16_t, uint8_t);
protected:
private:
	std::vector<uint8_t> memory;
	Memory *prg_rom;
	// Memory *chr_rom;

	void ram_write_mirror(uint16_t, uint8_t);
	void ppu_write_mirror(uint16_t, uint8_t);
	// void io_write_mirror(uint16_t, uint8_t);
};

#endif