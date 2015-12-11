#ifndef PPU_H
#define PPU_H

#include "CPU.hpp"
#include "Memory.hpp"
#include "Console.hpp"

#include <stdint.h>

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

	void print_state();
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

	// int cycles;
	// int scanlines;
	// Memory *memory;
	Console *console;	

	// void render_frame();
	// void render_scanline();
	// void render_pixel();
	void init_gui();

};


#endif