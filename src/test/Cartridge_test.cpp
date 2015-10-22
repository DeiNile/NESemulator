#include "../headers/Cartridge.hpp"
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdint.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Cartridge_test

struct Cartridge_fixture {
	
	Cartridge *cart;
	std::string filename;

	Cartridge_fixture() {
		filename = "test_rom.ines";
		cart = new Cartridge(filename);
	};

	~Cartridge_fixture() {
		delete cart;
	};
};

BOOST_FIXTURE_TEST_SUITE(egg, Cartridge_fixture)

BOOST_AUTO_TEST_CASE(read_valid_header_test)
{
	BOOST_CHECK(true);
	Header h = cart->get_header();
	BOOST_CHECK(h.valid_header());
}

BOOST_AUTO_TEST_CASE(get_trainer_data_test)
{
	BOOST_CHECK(cart->get_trainer() != NULL);
}

BOOST_AUTO_TEST_CASE(get_prg_rom_data_test)
{
	BOOST_CHECK(cart->get_prg_rom() != NULL);
	BOOST_CHECK(cart->get_prg_rom()[0] == 0x02);
}

BOOST_AUTO_TEST_CASE(get_chr_rom_data_test)
{
	BOOST_CHECK(cart->get_chr_rom() != NULL);
}

BOOST_AUTO_TEST_CASE(get_prg_ram_data_test)
{
	BOOST_CHECK(cart->get_prg_ram() != NULL);
}

BOOST_AUTO_TEST_CASE(get_playchoice_data_test)
{
	BOOST_CHECK(cart->get_playchoice() != NULL);
}

BOOST_AUTO_TEST_SUITE_END()
