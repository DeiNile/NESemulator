#ifndef NROM_MAPPER_H
#define NROM_MAPPER_H

#include "Memory_Mapper.hpp"
#include "Cartridge.hpp"

#define PRG_SIZE 16384
#define NUM_BANKS 8

class NROM_Mapper : Memory_Mapper
{
public:
	NROM_Mapper(Cartridge &);

    void write(uint16_t, uint8_t);
    uint8_t read(uint16_t address);
    
protected:
private:
    std::vector<std::vector<uint8_t> > banks;
    std::vector<int> active_banks;
	bool mirrored;

    void load_prg(const std::vector<uint8_t> &);
    int calc_bank(uint16_t);
    int calc_address(uint16_t);
    void switch_bank(int, int, int);
	void set_mirrored(int);
};


#endif
