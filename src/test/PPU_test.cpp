#include <boost/test/unit_test.hpp>
#include "../headers/CPU.hpp"
#include "../headers/PPU.hpp"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PPU_test
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdint.h>
#include <vector>

struct PPU_fixture {
	Console console;
	CPU *cpu;
	PPU *ppu;
	uint8_t val;

	PPU_fixture() {
		cpu = console.get_cpu();
		ppu = console.get_ppu();
		val = (rand() + 1) % (UINT8_MAX + 1);
		srand(time(NULL));
	};
};

struct PPU_register_fixture {
	Console console;
	CPU *cpu;
	PPU *ppu;
	uint8_t val;

	PPU_register_fixture() {
		cpu = console.get_cpu();
		ppu = console.get_ppu();
		val = (rand() + 1) % (UINT8_MAX + 1);
		srand(time(NULL));
	};
};

BOOST_FIXTURE_TEST_SUITE(cpu_io_write_reflected_in_ppu_register_tests, PPU_fixture)

BOOST_AUTO_TEST_CASE(nmi_enabled_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->is_nmi());
}

BOOST_AUTO_TEST_CASE(master_enabled_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->is_master());
}

BOOST_AUTO_TEST_CASE(sprite_size_128_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->get_sprite_size());
}

BOOST_AUTO_TEST_CASE(background_address_is_right_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->get_base_tile_pattern_address() == 0x1000);
}

BOOST_AUTO_TEST_CASE(sprite_address_is_right_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->get_base_sprite_pattern_address() == 0x1000);
}

BOOST_AUTO_TEST_CASE(vram_increment_is_32_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->get_vram_increment() == 32);
}

BOOST_AUTO_TEST_CASE(base_nametable_is_0x2c00_after_write_to_ctrl_test)
{
	cpu->write_memory(0x2000, 0xFF);
	BOOST_CHECK(ppu->get_base_nametable_address() == 0x2C00);

}

BOOST_AUTO_TEST_CASE(buffered_ppu_data_read_test)
{
	cpu->write_memory(0x2000, 0x0); // Ensure vram increment is 1
	cpu->write_memory(0x2006, 0x20); // write high byte of address
	cpu->write_memory(0x2006, 0x00); // write low byte of address
	cpu->write_memory(0x2007, 0x11); // is written to 0x2000
	cpu->write_memory(0x2007, 0xFF); // is written to 0x2001
	// Reset the address to point to 0x2000
	cpu->write_memory(0x2006, 0x20); // write high byte of address
	cpu->write_memory(0x2006, 0x00); // write low byte of address
	BOOST_CHECK(cpu->read_memory(0x2007) == 0); // The first read just fills the temporary buffer
	BOOST_CHECK(cpu->read_memory(0x2007) == 0); // The second read is genuine
	BOOST_CHECK(cpu->read_memory(0x2007) == 0x11);
	BOOST_CHECK(cpu->read_memory(0x2007) == 0xFF);
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(write_to_ppu_memory_from_cpu_tests, PPU_fixture)

BOOST_AUTO_TEST_CASE(write_address_bytes_to_scroll_test)
{
	cpu->write_memory(0x2005, 0x7D);
	cpu->write_memory(0x2005, 0x5E);
	BOOST_CHECK(ppu->get_temp_address() == 0x616F);
}

BOOST_AUTO_TEST_CASE(x_scroll_updated_after_first_scroll_write_test)
{
	cpu->write_memory(0x2005, 0x7D);
	BOOST_CHECK(ppu->get_x_scroll() == 0x5);
}

BOOST_AUTO_TEST_CASE(write_address_bytes_to_ppu_test)
{
	cpu->write_memory(0x2006, 0x7D);
	cpu->write_memory(0x2006, 0xF0);
	BOOST_CHECK(ppu->get_address() == 0x3DF0);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_memory_test)
{
	cpu->write_memory(0x2006, 0x20);
	cpu->write_memory(0x2006, 0x00);
	cpu->write_memory(0x2007, 0xFF);
	BOOST_CHECK(ppu->read_memory(0x2000) == 0xFF);
}

BOOST_AUTO_TEST_CASE(increment_by_one_after_write_test)
{
	// Make sure that the increment bit is clear
	cpu->write_memory(0x2000, 0x0);
	cpu->write_memory(0x2006, 0x20);
	cpu->write_memory(0x2006, 0x00);
	cpu->write_memory(0x2007, 0xFF);
	BOOST_CHECK(ppu->get_address() == 0x2001);
}

BOOST_AUTO_TEST_CASE(increment_by_32_after_write_test)
{
	// Set the increment bit
	cpu->write_memory(0x2000, 0x4);
	cpu->write_memory(0x2006, 0x20);
	cpu->write_memory(0x2006, 0x00);
	cpu->write_memory(0x2007, 0xFF);
	BOOST_CHECK(ppu->get_address() == 0x2020);
}

BOOST_AUTO_TEST_CASE(changing_nametable_base_in_existing_address_test)
{
	cpu->write_memory(0x2006, 0xFF);
	cpu->write_memory(0x2006, 0xFF);
	cpu->write_memory(0x2000, 0);
	BOOST_CHECK(ppu->get_temp_address() == 0x33FF);

}

BOOST_AUTO_TEST_CASE(greyscale_after_write_test)
{
	cpu->write_memory(0x2001, 0x1);
	BOOST_CHECK(ppu->is_colour());
}

BOOST_AUTO_TEST_CASE(show_left_background_after_write_test)
{
	cpu->write_memory(0x2001, 0x2);
	BOOST_CHECK(ppu->background_is_shown_to_left());
}

BOOST_AUTO_TEST_CASE(show_left_sprite_after_write_test)
{
	cpu->write_memory(0x2001, 0x4);
	BOOST_CHECK(ppu->sprites_are_shown_to_left());
}

BOOST_AUTO_TEST_CASE(show_backgorund_after_write_test)
{
	cpu->write_memory(0x2001, 0x8);
	BOOST_CHECK(ppu->is_background_shown());
}

BOOST_AUTO_TEST_CASE(show_sprites_after_write_test)
{
	cpu->write_memory(0x2001, 0x10);
	BOOST_CHECK(ppu->are_sprites_shown());
}

BOOST_AUTO_TEST_CASE(red_is_emphasized_after_write_test)
{
	cpu->write_memory(0x2001, 0x20);
	BOOST_CHECK(ppu->red_is_emphasized());
}

BOOST_AUTO_TEST_CASE(green_is_emphasized_after_write_test)
{
	cpu->write_memory(0x2001, 0x40);
	BOOST_CHECK(ppu->green_is_emphasized());
}

BOOST_AUTO_TEST_CASE(blue_is_emphasized_after_write_test)
{
	cpu->write_memory(0x2001, 0x80);
	BOOST_CHECK(ppu->blue_is_emphasized());
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_FIXTURE_TEST_SUITE(execution_tests, PPU_fixture)

BOOST_AUTO_TEST_CASE(detect_vblank_in_cpu_test)
{
	// Enable NMI from CPU
	cpu->write_memory(0x2000, 0x80);
	// Execute all the rendering cycles leading up to v-blank
	for (int i = 0; i < (341 * 241) + 1; i++) {
		ppu->execute();
	}
	// Should start the vblank
	ppu->execute();
	BOOST_CHECK(cpu->read_memory(0x2002) & 0x80);
}

BOOST_AUTO_TEST_CASE(detect_end_of_vblank_in_cpu_test)
{
	cpu->write_memory(0x2000, 0x80);
	for (int i = 0; i < (341 * 261) + 1; i++) {
		ppu->execute();
	}
	ppu->execute();
	BOOST_CHECK(!(cpu->read_memory(0x2002) & 0x80));
}

BOOST_AUTO_TEST_SUITE_END()