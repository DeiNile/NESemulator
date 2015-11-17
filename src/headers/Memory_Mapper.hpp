#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H

#include "Cartridge.hpp"
#include <vector>
#include <stdint.h>

#define PRG_BANK_SIZE 4096
#define CHR_BANK_SIZE 1024


class Memory_Mapper
{
public:
    //	Memory_Mapper();
	// ~Memory_Mapper();

    void write(uint16_t, uint8_t);
    uint8_t read(uint16_t);
protected:
	std::vector<std::vector<uint8_t> > banks;
    std::vector<int> active_banks;
    
    void load_prg(const std::vector<uint8_t> &);
    int calc_bank(uint16_t);
    int calc_address(uint16_t);
    void switch_bank(int, int, int);
};

#endif
