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

#define WIDTH 256
#define HEIGHT 240

#define MEMORY_SIZE 8192
#define NAMETABLE_SIZE 960
#define TILE_SIZE 64

#define NAMETABLE_START_ADDRESS 0x2000
#define NAMETABLE_BASE_MASK 0x03
#define EGG 10
#define VRAM_MASK 0x4
#define VERTICAL_INCREMENT 32
#define HORIZONTAL_INCREMENT 1
#define SPRITE_START_ADDRESS_MASK 0x8
#define SPRITE_START_ADDRESS_RIGHT 0x1000
#define SPRITE_START_ADDRESS_LEFT 0
#define TILE_START_ADDRESS_MASK 0x10
#define TILE_START_ADDRESS_RIGHT 0x1000
#define TILE_START_ADDRESS_LEFT 0
#define SPRITE_SIZE_MASK 0x20
#define SPRITE_SIZE_SMALL 64
#define SPRITE_SIZE_LARGE 128
#define MASTER_MASK 0x40
#define NMI_MASK 0x80
#define VBLANK_SET_MASK 0x80
#define VBLANK_CLEAR_MASK 0x7F
#define SPRITE_0_HIT_SET_MASK 0x40
#define SPRITE_0_HIT_CLEAR_MASK 0xBF
#define SPRITE_OVERFLOW_SET_MASK 0x20
#define SPRITE_OVERFLOW_CLEAR_MASK 0xDF
#define FIVE_LEAST_SIGNIFICANT_BITS 0x1F
#define THREE_MOST_SIGNIFICANT_BITS 0xE0
#define RED_EMPHASIS_MASK 0x20
#define GRAYSCALE_MASK 0x1
#define SHOW_BACKGROUND_LEFT_MASK 0x2
#define SHOW_SPRITES_TO_LEFT_MASK 0x4
#define SHOW_BACKGROUND_MASK 0x8
#define SHOW_SPRITES_MASK 0x10
#define GREEN_EMPHASIS_MASK 0x40
#define BLUE_EMPHASIS_MASK 0x80

PPU::PPU(Console *con)
{
	cycles = 0;
	scanline = 0;
	console = con;
	memory.resize(MEMORY_SIZE);
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
	uint8_t status = PPU_STATUS;
	clear_vblank();
	return status;
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

// Functions for all the various data in the PPU registers

// CTRL register values

uint16_t PPU::get_base_nametable_address()
{
	return NAMETABLE_START_ADDRESS | ((PPU_CTRL & NAMETABLE_BASE_MASK) << EGG);
}

uint8_t PPU::get_vram_increment()
{
	if (PPU_CTRL & VRAM_MASK) {
		return VERTICAL_INCREMENT;
	} else {
		return HORIZONTAL_INCREMENT;
	}
}

uint16_t PPU::get_base_sprite_pattern_address()
{
	if (PPU_CTRL & SPRITE_START_ADDRESS_MASK) {
		return SPRITE_START_ADDRESS_RIGHT;
	} else {
		return SPRITE_START_ADDRESS_LEFT;
	}
}

uint16_t PPU::get_base_tile_pattern_address()
{
	if (PPU_CTRL & TILE_START_ADDRESS_MASK) {
		return TILE_START_ADDRESS_RIGHT;
	} else {
		return TILE_START_ADDRESS_LEFT;
	}
}

uint8_t PPU::get_sprite_size()
{
	if (PPU_CTRL & SPRITE_SIZE_MASK) {
		return SPRITE_SIZE_LARGE;
	} else {
		return SPRITE_SIZE_SMALL;
	}
}

bool PPU::is_master()
{
	return (PPU_CTRL & MASTER_MASK);
}

bool PPU::is_nmi()
{
	return (PPU_CTRL & NMI_MASK);
}

// void PPU::set_nmi()
// {
// 	PPU_CTRL = (PPU_CTRL | 0x80);
// 	update_ppu_ctrl();
// }

// void PPU::clear_nmi()
// {
// 	PPU_CTRL = (PPU_CTRL & 7F);
// 	update_ppu_ctrl();
// }

// MASK register values

bool PPU::is_grayscale()
{
	return (PPU_MASK & 0x1);
}

bool PPU::background_is_shown_to_left()
{
	return (PPU_MASK & 0x2);
}

bool PPU::sprites_are_shown_to_left()
{
	return (PPU_MASK & 0x4);
}

bool PPU::is_background_shown()
{
	return (PPU_MASK & 0x8);
}

bool PPU::are_sprites_shown()
{
	return (PPU_MASK & 0x10);
}

bool PPU::red_is_emphasized()
{
	return (PPU_MASK & 0x20);
}

bool PPU::green_is_emphasized()
{
	return (PPU_MASK & 0x40);
}

bool PPU::blue_is_emphasized()
{
	return (PPU_MASK & 0x80);
}

// STATUS register values

void PPU::set_vblank()
{
	PPU_STATUS |= VBLANK_SET_MASK;
}

void PPU::clear_vblank()
{
	PPU_STATUS &= VBLANK_CLEAR_MASK;
}

void PPU::set_sprite_0_hit()
{
	PPU_STATUS |= SPRITE_0_HIT_SET_MASK;
}

void PPU::clear_sprite_0_hit()
{
	PPU_STATUS &= SPRITE_0_HIT_CLEAR_MASK;
}

void PPU::set_sprite_overflow()
{
	PPU_STATUS |= SPRITE_OVERFLOW_SET_MASK;
}

void PPU::clear_sprite_overflow()
{
	PPU_STATUS &= SPRITE_OVERFLOW_CLEAR_MASK;
}

void PPU::set_least_significant_bits(uint8_t reg_val)
{
	reg_val &= FIVE_LEAST_SIGNIFICANT_BITS;
	PPU_STATUS = (PPU_STATUS & THREE_MOST_SIGNIFICANT_BITS) | reg_val;
}


// ===========================================================

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
			WIDTH, 
			HEIGHT, 
			SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cerr << "SDL window could not be created. Error: " <<
			 SDL_GetError() << std::endl;
		} else {
			surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x0, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(3000);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();	
}

/*
 * Fetches the tile index for the current pixel.
 */
uint8_t PPU::get_nametable_entry()
{
	// Figure out which nametable to look in
	uint16_t base = get_base_nametable_address();
	// The address for the nametable entry
	uint16_t offset = (scanline * 32) + cycles;
	return memory.at(base + offset);
}

uint8_t PPU::get_attribute_entry()
{
	uint16_t base = get_base_nametable_address() + 960;
	// ?????
	return 0;
}

uint8_t PPU::get_low_tile_entry(uint8_t pattern_index)
{
	uint16_t pattern_base = get_base_tile_pattern_address();
	return memory.at(pattern_base + (pattern_index * TILE_SIZE));
}

uint8_t PPU::get_high_tile_entry(uint8_t pattern_index)
{
	uint16_t pattern_base = get_base_tile_pattern_address();
	return memory.at(pattern_base + (pattern_index * TILE_SIZE));
}

/*
 * Takes two patterns and calculates the values to be used for the 
 * colour table.
 * Could be made more efficient by using a UINT16_T instead of a vector
 */
// std::vector<uint8_t> PPU::combine_patterns(uint8_t low, uint8_t high)
// {
// 	std::vector<uint8_t> ret(8);
// 	uint8_t pixel = 0;
// 	for (int i = 7; i >= 0; i--) {
// 		ret.at(i) = ((high & 0x1) << 1) | (low & 0x1);
// 		low >>= 1;
// 		high >>= 1;
// 	}
// 	return ret;
// }

void PPU::render_pixel()
{
	uint8_t nametable_entry;
	uint8_t attribute_entry;
	uint8_t low;
	uint8_t high;
	switch(cycles % 8) {
		case 0:
			// Very first cycle should do nothing
			// Push values into shift registers
			// combine patterns 
			// create colour for pixels
			// push pixels to frame buffer
			break;
		case 1:
			// Get nametable entry
			nametable_entry = get_nametable_entry();
			break;
		case 3:
			// Get attribute table entry
			attribute_entry = get_attribute_entry();
			break;
		case 5:
			low = get_low_tile_entry(nametable_entry);
			break;
		case 7: 
			high = get_high_tile_entry(nametable_entry);
			// increment SOMETHING
			break;

	}
	// Combine into a rendered pixel
	
}

/*
 * Executes a single cycle of PPU instructions.
 */
void PPU::execute()
{
	if (is_background_shown() || are_sprites_shown()) {
		if ((scanline >= 0 && scanline < 240) && 
			(cycles >= 0 && cycles < 256)) {
			render_pixel();
		}
		else if (scanline == 240) {
			// DO nothing?
		} 
		else if (scanline > 240) {
			if (scanline == 241 && cycles == 2) {
				set_vblank();
			}
			// change VRAM 
		}
	}

	cycles++;
	if (cycles == 341) {
		cycles = 0;
		scanline++;
	}
	if (scanline == 262) {
		scanline = 0;
	}
}