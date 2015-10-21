#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Cartridge_test

#include "../headers/Cartridge.hpp"
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdint.h>

struct Cartridge_fixture {
	
	Cartridge *cart;
	std::string filename;

	Cartridge_fixture() {
		filename = "";
		cart = new Cartridge(filename);
	};

	~Cartridge_fixture() {
		delete cart;
	};
};

BOOST_FIXTURE_TEST_SUITE(egg, Cartridge_fixture)

BOOST_AUTO_TEST_CASE(read_valid_header_test)
{
	BOOST_CHECK(cart->get_header().valid_header());
}

BOOST_AUTO_TEST_SUITE_END()