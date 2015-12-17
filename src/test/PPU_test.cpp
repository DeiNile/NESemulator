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

BOOST_AUTO_TEST_CASE(write_to_ppu_ctrl_from_cpu_test)
{
	cpu->write_memory(0x2000, val);
	BOOST_CHECK(ppu->get_ppu_ctrl() == val);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_mask_from_cpu_test)
{
	cpu->write_memory(0x2001, val);
	BOOST_CHECK(ppu->get_ppu_mask() == val);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_status_from_cpu_test)
{
	cpu->write_memory(0x2002, val);
	BOOST_CHECK(ppu->get_ppu_status() == val);
}

BOOST_AUTO_TEST_CASE(write_to_oam_addr_from_cpu_test)
{
	cpu->write_memory(0x2003, val);
	BOOST_CHECK(ppu->get_oam_addr() == val);
}

BOOST_AUTO_TEST_CASE(write_to_oam_data_from_cpu_test)
{
	cpu->write_memory(0x2004, val);
	BOOST_CHECK(ppu->get_oam_data() == val);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_scroll_from_cpu_test)
{
	cpu->write_memory(0x2005, val);
	BOOST_CHECK(ppu->get_ppu_scroll() == val);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_addr_from_cpu_test)
{
	cpu->write_memory(0x2006, val);
	BOOST_CHECK(ppu->get_ppu_addr() == val);
}

BOOST_AUTO_TEST_CASE(write_to_ppu_data_from_cpu_test)
{
	cpu->write_memory(0x2007, val);
	BOOST_CHECK(ppu->get_ppu_data() == val);
}

BOOST_AUTO_TEST_CASE(write_to_oam_dma_from_cpu_test)
{
	cpu->write_memory(0x4014, val);
	BOOST_CHECK(ppu->get_oam_dma() == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_ctrl_in_cpu_memory_test)
{
	ppu->set_ppu_ctrl(val);
	ppu->update_ppu_ctrl();
	BOOST_CHECK(cpu->read_memory(0x2000) == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_mask_in_cpu_memory_test)
{
	ppu->set_ppu_mask(val);
	ppu->update_ppu_mask();
	BOOST_CHECK(cpu->read_memory(0x2001) == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_status_in_cpu_memory_test)
{
	ppu->set_ppu_status(val);
	ppu->update_ppu_status();
	BOOST_CHECK(cpu->read_memory(0x2002) == val);
}

BOOST_AUTO_TEST_CASE(update_oam_addr_in_cpu_memory_test)
{
	ppu->set_oam_addr(val);
	ppu->update_oam_addr();
	BOOST_CHECK(cpu->read_memory(0x2003) == val);
}

BOOST_AUTO_TEST_CASE(update_oam_data_in_cpu_memory_test)
{
	ppu->set_oam_data(val);
	ppu->update_oam_data();
	BOOST_CHECK(cpu->read_memory(0x2004) == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_scroll_in_cpu_memory_test)
{
	ppu->set_ppu_scroll(val);
	ppu->update_ppu_scroll();
	BOOST_CHECK(cpu->read_memory(0x2005) == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_addr_in_cpu_memory_test)
{
	ppu->set_ppu_addr(val);
	ppu->update_ppu_addr();
	BOOST_CHECK(cpu->read_memory(0x2006) == val);
}

BOOST_AUTO_TEST_CASE(update_ppu_data_in_cpu_memory_test)
{
	ppu->set_ppu_data(val);
	ppu->update_ppu_data();
	BOOST_CHECK(cpu->read_memory(0x2007) == val);
}

BOOST_AUTO_TEST_CASE(update_oam_dma_in_cpu_memory_test)
{
	ppu->set_oam_dma(val);
	ppu->update_oam_dma();
	BOOST_CHECK(cpu->read_memory(0x4014) == val);
}

BOOST_AUTO_TEST_CASE(buffered_ppu_data_read_test)
{
	cpu->write_memory(0x2006, 0x20);
	cpu->write_memory(0x2006, 0x00);
	cpu->write_memory(0x2007, 0x11); // is written to 0x2000
	cpu->write_memory(0x2007, 0xFF); // is written to 0x2001
	BOOST_CHECK(cpu->read_memory(0x2007) == 0);
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