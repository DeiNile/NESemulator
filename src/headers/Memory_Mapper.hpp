#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H

#include "Cartridge.hpp"
#include "Memory.hpp"
#include <vector>
#include <stdint.h>

#define PRG_BANK_SIZE 4096
#define CHR_BANK_SIZE 1024


class Memory_Mapper : public Memory
{
public:
	Memory_Mapper();
	Memory_Mapper(Cartridge *);
	virtual ~Memory_Mapper() = 0;

    virtual void write(uint16_t, uint8_t) = 0;
    virtual uint8_t read(uint16_t) = 0;
    virtual void set_cartridge(Cartridge *);
protected:
	Cartridge *cartridge;

    virtual void load_prg(const std::vector<uint8_t> &) = 0;
    virtual int calc_prg_bank(uint16_t) = 0;
    virtual int calc_prg_address(uint16_t) = 0;

    virtual void load_chr(const std::vector<uint8_t> &) = 0;
    virtual int calc_chr_bank(uint16_t) = 0;
    virtual int calc_chr_address(uint16_t) = 0;

private:
};

#endif
