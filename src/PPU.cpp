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

#define MEMORY_SIZE 0x4000
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

#define VRAM_ADDRESS_MASK 0x7FFF
#define BYTE_WIDTH 8

PPU::PPU(Console *con)
{
	cycles = 0;
	scanline = 0;
	console = con;
	write_toggle = false;

	memory.resize(MEMORY_SIZE);
	// init_gui();
}

PPU::~PPU()
{
}

void PPU::print_state()
{
	// std::cerr << boost::format("PPU_Data:%02X PPU_MASK:%02X PPU_STATUS:%02X OAM_ADDR:%02X OAM_DATA:%02X PPU_SCROLL:%02X PPU_ADDR:%02X PPU_DATA:%02X OAM_DMA:%02X") % 
	// 	(int)PPU_DATA % (int)PPU_MASK % (int)PPU_STATUS % (int)OAM_ADDR % (int)OAM_DATA % (int)PPU_SCROLL % (int)PPU_ADDR % (int)PPU_DATA % (int)OAM_DMA;
}

// Getters used for unit testing


uint8_t PPU::read_ppu_status()
{
	uint8_t ret = vblank_flag;
	ret <<= 1;
	ret |= sprite_hit_flag;
	ret <<= 1;
	ret |= sprite_overflow_flag;
	ret <<= 5;
	ret |= last_register;
	clear_vblank();
	write_toggle = false;
	return ret;
}

uint8_t PPU::read_oam_data()
{
	return 0;
}

uint8_t PPU::read_ppu_data()
{
	// Save temporary return value
	uint8_t ret = curr_data;
	// Set the next return value
	curr_data = next_data;
	// Store the current read
	next_data = memory.at(current_address);
	current_address += get_vram_increment();
	
	return ret;
}

uint16_t PPU::get_address()
{
	return current_address;
}

uint16_t PPU::get_temp_address()
{
	return temp_address;
}

void PPU::set_ppu_ctrl(uint8_t val)
{
	// console->get_cpu()->write_memory(PPU_CTRL_ADDR, val);
	// PPU_CTRL = val;
	temp_address &= 0x33FF; // set bits to 0
	temp_address |= (val & 0x3) << 10; // set bits again
}

void PPU::set_ppu_mask(uint8_t val)
{
	// console->get_cpu()->write_memory(PPU_MASK_ADDR, val);
	// PPU_MASK = val;
}

void PPU::set_oam_addr(uint8_t val)
{
	// OAM_ADDR = val;
}

void PPU::set_oam_data(uint8_t val)
{
	// OAM_DATA = val;
}

void PPU::set_ppu_scroll(uint8_t val)
{
	// PPU_SCROLL = val;
	// console->get_cpu()->write_memory(PPU_SCROLL_ADDR, val);
	if (!write_toggle) {
		x_scroll = (val & 0x7);
		temp_address = ((val & 0xF8) >> 3);
	} else {
		uint16_t temp = (val & 0xF8) << 2;
		temp |= (val & 0x7) << 12;
		temp_address |= temp;
	}
	write_toggle = !write_toggle;
}

void PPU::set_ppu_addr(uint8_t val)
{
	// console->get_cpu()->write_memory(PPU_ADDR_ADDR, val);
	// PPU_ADDR = val;
	if (!write_toggle) {
		temp_address = ((uint16_t)val & 0x3F) << BYTE_WIDTH;
	} else {
		temp_address |= val;
		current_address = temp_address;
	}
	// Flip write_toggle
	write_toggle = !write_toggle;

}

void PPU::set_ppu_data(uint8_t val)
{
	// PPU_DATA = val;
	memory.at(current_address) = val;
	// Should the lower byte of PPU_ADDR be updates too?
	current_address += get_vram_increment();
	// console->get_cpu()->write_memory(PPU_ADDR_ADDR, current_address);
	// PPU_ADDR += get_vram_increment();
	// update_ppu_addr(); // BREAKS THE CURRENT ADDRESS, GETS RE-WRITTEN TO BY UPDATING
}	

void PPU::set_oam_dma(uint8_t val)
{
	// OAM_DMA = val;
}

uint8_t PPU::read_memory(uint16_t address)
{
	return memory.at(address);
}

uint8_t PPU::get_x_scroll()
{
	return x_scroll;
}


// Functions for all the various data in the PPU registers

// CTRL register values

uint16_t PPU::get_base_nametable_address()
{
	return NAMETABLE_START_ADDRESS | ( 
		(console->get_cpu()->read_memory(PPU_CTRL_ADDR) & NAMETABLE_BASE_MASK)
		<< EGG);
}

uint8_t PPU::get_vram_increment()
{
	if (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & VRAM_MASK) {
		return VERTICAL_INCREMENT;
	} else {
		return HORIZONTAL_INCREMENT;
	}
}

uint16_t PPU::get_base_sprite_pattern_address()
{
	if (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & SPRITE_START_ADDRESS_MASK) {
		return SPRITE_START_ADDRESS_RIGHT;
	} else {
		return SPRITE_START_ADDRESS_LEFT;
	}
}

uint16_t PPU::get_base_tile_pattern_address()
{
	if (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & TILE_START_ADDRESS_MASK) {
		return TILE_START_ADDRESS_RIGHT;
	} else {
		return TILE_START_ADDRESS_LEFT;
	}
}

uint8_t PPU::get_sprite_size()
{
	if (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & SPRITE_SIZE_MASK) {
		return SPRITE_SIZE_LARGE;
	} else {
		return SPRITE_SIZE_SMALL;
	}
}

bool PPU::is_master()
{
	return (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & MASTER_MASK);
}

bool PPU::is_nmi()
{
	return (console->get_cpu()->read_memory(PPU_CTRL_ADDR) & NMI_MASK);
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

bool PPU::is_colour()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x1);
}

bool PPU::background_is_shown_to_left()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x2);
}

bool PPU::sprites_are_shown_to_left()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x4);
}

bool PPU::is_background_shown()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x8);
}

bool PPU::are_sprites_shown()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x10);
}

bool PPU::red_is_emphasized()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x20);
}

bool PPU::green_is_emphasized()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x40);
}

bool PPU::blue_is_emphasized()
{
	return (console->get_cpu()->read_memory(PPU_MASK_ADDR) & 0x80);
}

// STATUS register values

void PPU::set_vblank()
{
	vblank_flag = true;
	// PPU_STATUS |= VBLANK_SET_MASK;
}

void PPU::clear_vblank()
{
	vblank_flag = false;
	// PPU_STATUS &= VBLANK_CLEAR_MASK;
}

void PPU::set_sprite_0_hit()
{
	sprite_hit_flag = true;
	// PPU_STATUS |= SPRITE_0_HIT_SET_MASK;
}

void PPU::clear_sprite_0_hit()
{
	sprite_hit_flag = false;
	// PPU_STATUS &= SPRITE_0_HIT_CLEAR_MASK;
}

void PPU::set_sprite_overflow()
{
	sprite_overflow_flag = true;
	// PPU_STATUS |= SPRITE_OVERFLOW_SET_MASK;
}

void PPU::clear_sprite_overflow()
{
	sprite_overflow_flag = false;
	// PPU_STATUS &= SPRITE_OVERFLOW_CLEAR_MASK;
}

void PPU::set_least_significant_bits(uint8_t reg_val)
{
	last_register = (reg_val & FIVE_LEAST_SIGNIFICANT_BITS);
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
	if (cycles == 0) {
		// Do nothing?
		update_counters();
		return;
	}
	// Check if V blank should start
	if (scanline == 241 && cycles == 1) {
		// If NMI is enabled, set it
		if (is_nmi()) {
			set_vblank();
		}
	} else if (scanline == 261 && cycles == 1) { 
		// V blank is now over
		clear_vblank();
		// update_ppu_status();
	}

	// if (is_background_shown() || are_sprites_shown()) {
	// 	if ((scanline >= 0 && scanline < 240) && 
	// 		(cycles >= 0 && cycles < 256)) {
	// 		render_pixel();
	// 	}
	// 	else if (scanline == 240) {
	// 		// DO nothing?
	// 	} 
	// 	else if (scanline > 240) {
	// 		if (scanline == 241 && cycles == 2) {
	// 			set_vblank();
	// 		}
	// 		if (scanline == 251) {
	// 			// Increment V
	// 		}
	// 		// change VRAM 
	// 	}
	// }

	update_counters();
}

void PPU::update_counters()
{
	cycles++;
	if (cycles == 341) {
		cycles = 0;
		scanline++;
	}
	if (scanline == 262) {
		scanline = 0;
	}
}