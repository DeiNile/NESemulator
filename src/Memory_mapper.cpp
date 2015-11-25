#include "Headers/Memory_Mapper.hpp"


Memory::~Memory() {};

Memory_Mapper::Memory_Mapper() {};

Memory_Mapper::Memory_Mapper(Cartridge *cart)
{
    cartridge = cart;
}

Memory_Mapper::~Memory_Mapper() {}

void Memory_Mapper::set_cartridge(Cartridge *cart)
{
    cartridge = cart;
}