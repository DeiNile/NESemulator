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

BOOST_AUTO_TEST_SUITE_END()
