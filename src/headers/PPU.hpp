#ifndef PPU_H
#define PPU_H

#include "CPU.hpp"
#include "Memory.hpp"
#include "Console.hpp"

#include <stdint.h>
#include <vector>

// class CPU;
class Console;
class CPU;

class PPU
{
public:
	// PPU();
	PPU(Console *);
	~PPU();

	uint8_t get_ppu_ctrl();
	uint8_t get_ppu_mask();
	uint8_t get_ppu_status();
	uint8_t get_oam_addr();
	uint8_t get_oam_data();
	uint8_t get_ppu_scroll();
	uint8_t get_ppu_addr();
	uint8_t get_ppu_data();
	uint8_t get_oam_dma();

	uint16_t get_address();
	uint16_t get_temp_address();


	// Public mutators for when the registers change in CPU memory
	void set_ppu_ctrl(uint8_t);
	void set_ppu_mask(uint8_t);
	void set_ppu_status(uint8_t);
	void set_oam_addr(uint8_t);
	void set_oam_data(uint8_t);
	void set_ppu_scroll(uint8_t);
	void set_ppu_addr(uint8_t);
	void set_ppu_data(uint8_t);
	void set_oam_dma(uint8_t);

	// Updates each register in its corresponding address in CPU memory
	void update_ppu_ctrl();
	void update_ppu_mask();
	void update_ppu_status();
	void update_oam_addr();
	void update_oam_data();
	void update_ppu_scroll();
	void update_ppu_addr();
	void update_ppu_data();
	void update_oam_dma();

	uint8_t read_memory(uint16_t);
	uint8_t get_x_scroll();

	void print_state();
	void execute();
protected:

private:
	// Registers
	uint8_t PPU_CTRL;
	uint8_t PPU_MASK;
	uint8_t PPU_STATUS;
	uint8_t OAM_ADDR;
	uint8_t OAM_DATA;
	uint8_t PPU_SCROLL;
	uint8_t PPU_ADDR;
	uint8_t PPU_DATA;
	uint8_t OAM_DMA;
	uint8_t data_buffer;

	// uint16_t curr_tile;
	// uint16_t next_tile;
	// uint8_t curr_palette_attribute;
	// uint8_t next_palette_attribute;

	int cycles;
	int scanline;
	int frame;

	// Is used with PPU_DATA, used for nametable fetching, points to the 
	// nametable entry that is currently being rendered (drawn to the screen?)
	uint16_t current_address; // v - 15 bits
	uint16_t temp_address; // t - 15 bits
	uint8_t x_scroll; // fine x scroll - 3 bits
	bool write_toggle; // tells if a write is the first or second, 0 = first


	std::vector<uint8_t> memory;
	Console *console;

	uint16_t get_base_nametable_address();
	uint8_t get_vram_increment();
	uint16_t get_base_sprite_pattern_address();
	uint16_t get_base_tile_pattern_address();
	uint8_t get_sprite_size();
	bool is_master();
	bool is_nmi();

	bool is_grayscale();
	bool background_is_shown_to_left();
	bool sprites_are_shown_to_left();
	bool is_background_shown();
	bool are_sprites_shown();
	bool red_is_emphasized();
	bool green_is_emphasized();
	bool blue_is_emphasized();

	void set_vblank();
	void clear_vblank();
	void set_sprite_0_hit();
	void clear_sprite_0_hit();
	void set_sprite_overflow();
	void clear_sprite_overflow();
	void set_least_significant_bits(uint8_t);

	uint8_t get_nametable_entry();
	uint8_t get_attribute_entry();
	uint8_t get_low_tile_entry(uint8_t);
	uint8_t get_high_tile_entry(uint8_t);


	void render_pixel();
	void update_counters();
	void init_gui();

};


#endif