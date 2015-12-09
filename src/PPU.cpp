#include "headers/PPU.hpp"
// #include <iostream>
#include <boost/format.hpp>

PPU::PPU(Console *con)
{
	console = con;
}

// PPU::PPU()
// {

// };

PPU::~PPU()
{
	
}

void PPU::print_state()
{
	std::cerr << boost::format("PPU_Data:%02X PPU_MASK:%02X PPU_STATUS:%02X OAM_ADDR:%02X OAM_DATA:%02X PPU_SCROLL:%02X PPU_ADDR:%02X PPU_DATA:%02X OAM_DMA:%02X") % 
		(int)PPU_DATA % (int)PPU_MASK % (int)PPU_STATUS % (int)OAM_ADDR % (int)OAM_DATA % (int)PPU_SCROLL % (int)PPU_ADDR % (int)PPU_DATA % (int)OAM_DMA;
}

// Getters used for unit testing

uint8_t PPU::get_ppu_ctrl()
{
	return PPU_CTRL;
}

uint8_t PPU::get_ppu_mask()
{
	return PPU_MASK;
}

uint8_t PPU::get_ppu_status()
{
	return PPU_STATUS;
}

uint8_t PPU::get_oam_addr()
{
	return OAM_ADDR;
}

uint8_t PPU::get_oam_data()
{
	return OAM_DATA;
}

uint8_t PPU::get_ppu_scroll()
{
	return PPU_SCROLL;
}

uint8_t PPU::get_ppu_addr()
{
	return PPU_ADDR;
}

uint8_t PPU::get_ppu_data()
{
	return PPU_DATA;
}

uint8_t PPU::get_oam_dma()
{
	return OAM_DMA;
}

void PPU::set_ppu_ctrl(uint8_t val)
{
	PPU_CTRL = val;
}

void PPU::set_ppu_mask(uint8_t val)
{
	PPU_MASK = val;
}

void PPU::set_ppu_status(uint8_t val)
{
	PPU_STATUS = val;
}

void PPU::set_oam_addr(uint8_t val)
{
	OAM_ADDR = val;
}

void PPU::set_oam_data(uint8_t val)
{
	OAM_DATA = val;
}

void PPU::set_ppu_scroll(uint8_t val)
{
	PPU_SCROLL = val;
}

void PPU::set_ppu_addr(uint8_t val)
{
	PPU_ADDR = val;
}

void PPU::set_ppu_data(uint8_t val)
{
	PPU_DATA = val;
}

void PPU::set_oam_dma(uint8_t val)
{
	OAM_DMA = val;
}




void PPU::update_ppu_ctrl()
{
	console->get_cpu()->write_memory(0x2000, PPU_CTRL);
}

void PPU::update_ppu_mask()
{
	console->get_cpu()->write_memory(0x2001, PPU_MASK);
}

void PPU::update_ppu_status()
{
	console->get_cpu()->write_memory(0x2002, PPU_STATUS);
}

void PPU::update_oam_addr()
{
	console->get_cpu()->write_memory(0x2003, OAM_ADDR);
}

void PPU::update_oam_data()
{
	console->get_cpu()->write_memory(0x2004, OAM_DATA);
}

void PPU::update_ppu_scroll()
{
	console->get_cpu()->write_memory(0x2005, PPU_SCROLL);
}

void PPU::update_ppu_addr()
{
	console->get_cpu()->write_memory(0x2006, PPU_ADDR);
}

void PPU::update_ppu_data()
{
	console->get_cpu()->write_memory(0x2007, PPU_DATA);
}

void PPU::update_oam_dma()
{
	console->get_cpu()->write_memory(0x4014, OAM_DMA);
}
