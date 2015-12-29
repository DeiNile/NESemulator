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

	uint8_t read_ppu_status();
	uint8_t read_ppu_data();
	uint8_t read_oam_data();
	
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
	
	uint16_t get_base_nametable_address();
	uint8_t get_vram_increment();
	uint16_t get_base_sprite_pattern_address();
	uint16_t get_base_tile_pattern_address();
	uint8_t get_sprite_size();
	bool is_master();
	bool is_nmi();

	bool is_colour();
	bool background_is_shown_to_left();
	bool sprites_are_shown_to_left();
	bool is_background_shown();
	bool are_sprites_shown();
	bool red_is_emphasized();
	bool green_is_emphasized();
	bool blue_is_emphasized();

	uint8_t read_memory(uint16_t);
	uint8_t get_x_scroll();

	void print_state();
	void execute();
protected:

private:
	uint8_t curr_data;
	uint8_t next_data;

	bool vblank_flag;
	bool sprite_hit_flag;
	bool sprite_overflow_flag;
	uint8_t last_register;

	int cycles;
	int scanline;
	int frame;

	// Is used with PPU_DATA, used for nametable fetching, points to the 
	// nametable entry that is currently being rendered (drawn to the screen?)
	uint16_t current_address; // v - 15 bits
	uint16_t temp_address; // t - 15 bits
	uint8_t x_scroll; // fine x scroll - 3 bits
	bool write_toggle; // tells if a write is the first or second, 0 = first

	uint8_t nametable_entry;
	uint8_t attribute_entry;
	uint8_t high_tile_entry;
	uint8_t low_tile_entry;

	// Temporary variables for rendering
	uint16_t high_background_shift_register;
	uint16_t low_background_shift_register;
	uint8_t first_tile;
	uint8_t second_tile;


	std::vector<uint8_t> memory;
	Console *console;



	void set_vblank();
	void clear_vblank();
	void set_sprite_0_hit();
	void clear_sprite_0_hit();
	void set_sprite_overflow();
	void clear_sprite_overflow();
	void set_least_significant_bits(uint8_t);

	uint8_t get_nametable_entry();
	uint8_t get_attribute_entry();
	uint8_t get_low_tile_entry();
	uint8_t get_high_tile_entry();

	void move_temps_to_shift_registers();
	void get_rendering_variables();
	void render_pixel();
	void update_counters();
	void init_gui();

};


#endif