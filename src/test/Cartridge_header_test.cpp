#include "../headers/Cartridge_header.hpp"
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdint.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Cartridge_header_test

struct Header_fixture {
	Header *header;
	uint8_t raw_header[16];

	Header_fixture() {
		int i = 0;
		raw_header[i++] = 'N';
		raw_header[i++] = 'E';
		raw_header[i++] = 'S';
		raw_header[i++] = '\x1A';
		raw_header[i++] = (uint8_t)64; // PRG ROM
		raw_header[i++] = (uint8_t)64; // chr ROM
		raw_header[i++] = 0x17; // Flags 6
		raw_header[i++] = 0x13; // flags 7
		raw_header[i++] = (uint8_t)64; // PRG RAM
		raw_header[i++] = 0x1; // Flags 9
		raw_header[i++] = 0;
		raw_header[i++] = 0;
		raw_header[i++] = 0;
		raw_header[i++] = 0;
		raw_header[i++] = 0;
		raw_header[i++] = 0;

		header = new Header(raw_header);
	};

	~Header_fixture() {
		delete header;
	};
};

BOOST_FIXTURE_TEST_SUITE(update_fields_tests, Header_fixture)

BOOST_AUTO_TEST_CASE(valid_header_after_update_test)
{
	BOOST_CHECK(header->valid_header());
}

BOOST_AUTO_TEST_CASE(size_of_PRG_in_16KB_test)
{
	BOOST_CHECK(header->get_prg_rom_size_16KB() == 64);
}

BOOST_AUTO_TEST_CASE(size_of_PRG_in_8KB_test)
{
	BOOST_CHECK(header->get_prg_ram_size_8KB() == 64);
}

BOOST_AUTO_TEST_CASE(size_of_chr_in_8KB_test)
{
	BOOST_CHECK(header->get_chr_size_8KB() == 64);
}

BOOST_AUTO_TEST_CASE(nybble_test)
{
	BOOST_CHECK(header->get_nybble() == 0x11);
}

BOOST_AUTO_TEST_CASE(NTSC_system_test)
{
	BOOST_CHECK(header->is_NTSC());
}

BOOST_AUTO_TEST_CASE(is_playchoice_true_test)
{
	BOOST_CHECK(header->is_playchoice());
}

BOOST_AUTO_TEST_CASE(is_unisystem_true_test)
{
	BOOST_CHECK(header->is_unisystem());
}

BOOST_AUTO_TEST_CASE(is_trainer_present_test)
{
	BOOST_CHECK(header->is_trainer_present());
}

BOOST_AUTO_TEST_CASE(is_battery_used_test)
{
	BOOST_CHECK(header->is_battery_present());
}

BOOST_AUTO_TEST_CASE(no_four_screen_VRAM_test)
{
	BOOST_CHECK(!header->is_four_screen_VRAM());
}

BOOST_AUTO_TEST_CASE(vertical_mirroring_test)
{
	BOOST_CHECK(header->is_vertical());
}

BOOST_AUTO_TEST_CASE(no_horizontal_mirroring_test)
{
	BOOST_CHECK(!header->is_horizontal());
}

BOOST_AUTO_TEST_CASE(is_not_NES2_format_test)
{
	BOOST_CHECK(!header->is_NES2_format());
}

BOOST_AUTO_TEST_CASE(is_NES2_format_test)
{
	raw_header[7] = 0x1B;
	delete header;
	header = new Header(raw_header);
	BOOST_CHECK(header->is_NES2_format());
}

BOOST_AUTO_TEST_SUITE_END()