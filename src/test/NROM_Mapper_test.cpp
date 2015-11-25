#include <boost/test/unit_test.hpp>
#include "../headers/NROM_Mapper.hpp"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE NROM_Mapper_test


#include <iostream>
#include <string>

struct NROM_fixture {

    NROM_Mapper *mapper;
    Cartridge *cart;
    
    NROM_fixture() {
        cart = new Cartridge("nestest.nes");
        mapper = new NROM_Mapper(cart);
    };

    ~NROM_fixture() {
        delete mapper;
        delete cart;
    };    
};

BOOST_FIXTURE_TEST_SUITE( NROM_tests, NROM_fixture)

BOOST_AUTO_TEST_CASE( read_from_first_bank_test)
{
     BOOST_CHECK(mapper->read(0x0000) == 0x4C);
}

BOOST_AUTO_TEST_CASE( read_from_second_bank_test )
{
    BOOST_CHECK(mapper->read(0x1378) == 0x85);
}

BOOST_AUTO_TEST_CASE( read_from_last_byte_test )
{
    BOOST_CHECK(mapper->read(0x3FFF) == 0xC5);
}

BOOST_AUTO_TEST_SUITE_END()
