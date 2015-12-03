#ifndef NROM_MAPPER_H
#define NROM_MAPPER_H

#include "Memory_Mapper.hpp"
#include "Cartridge.hpp"

#define PRG_SIZE 16384
#define NUM_BANKS 8

class NROM_Mapper : public Memory_Mapper
{
public:
    NROM_Mapper();
	NROM_Mapper(Cartridge *);
    virtual ~NROM_Mapper();

    virtual void write(uint16_t, uint8_t);
    virtual uint8_t read(uint16_t address);
    virtual uint16_t read_address(uint16_t);
protected:
private:
    std::vector<std::vector<uint8_t> > banks;
    std::vector<int> active_banks;
	bool mirrored;

    virtual void load_prg(const std::vector<uint8_t> &);
    virtual int calc_bank(uint16_t);
    virtual int calc_address(uint16_t);
	void set_mirrored(int);
};


#endif
