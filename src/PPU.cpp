#include "headers/PPU.hpp"
#include <boost/format.hpp>

#include <SDL.h>
#include <stdio.h>

#define PPU_CTRL_ADDR 0x2000
#define PPU_MASK_ADDR 0x2001
#define PPU_STATUS_ADDR 0x2002
#define OAM_ADDR_ADDR 0x2003
#define OAM_DATA_ADDR 0x2004
#define PPU_SCROLL_ADDR 0x2005
#define PPU_ADDR_ADDR 0x2006
#define PPU_DATA_ADDR 0x2007
#define OAM_DMA_ADDR 0x4014

PPU::PPU(Console *con)
{
	console = con;
	init_gui();
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
	console->get_cpu()->write_memory(PPU_CTRL_ADDR, PPU_CTRL);
}

void PPU::update_ppu_mask()
{
	console->get_cpu()->write_memory(PPU_MASK_ADDR, PPU_MASK);
}

void PPU::update_ppu_status()
{
	console->get_cpu()->write_memory(PPU_STATUS_ADDR, PPU_STATUS);
}

void PPU::update_oam_addr()
{
	console->get_cpu()->write_memory(OAM_ADDR_ADDR, OAM_ADDR);
}

void PPU::update_oam_data()
{
	console->get_cpu()->write_memory(OAM_DATA_ADDR, OAM_DATA);
}

void PPU::update_ppu_scroll()
{
	console->get_cpu()->write_memory(PPU_SCROLL_ADDR, PPU_SCROLL);
}

void PPU::update_ppu_addr()
{
	console->get_cpu()->write_memory(PPU_ADDR_ADDR, PPU_ADDR);
}

void PPU::update_ppu_data()
{
	console->get_cpu()->write_memory(PPU_DATA_ADDR, PPU_DATA);
}

void PPU::update_oam_dma()
{
	console->get_cpu()->write_memory(OAM_DMA_ADDR, OAM_DMA);
}

void PPU::init_gui()
{
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDl could not initialize. Error: " << 
			SDL_GetError() << std::endl;
	} else {
		window = SDL_CreateWindow(
			"NES gui",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640, 
			480, 
			SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cerr << "SDL window could not be created. Error: " <<
			 SDL_GetError() << std::endl;
		} else {
			surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(3000);
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}