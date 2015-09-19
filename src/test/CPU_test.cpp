#include "../headers/CPU.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CPU_test
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

struct CPU_fixture {

	CPU cpu;
	int random;
	int random_neg;
	int address;
	CPU_fixture() {
		// CPU cpu;
		srand(time(NULL));
		random = rand () + 1;
		random_neg = random - random - random;
		address = (rand() + 1) & MEM_SIZE;
	};

	~CPU_fixture() {

	};
};

BOOST_AUTO_TEST_CASE(memory_write_underflow)
{
	CPU cpu;
	cpu.write_memory(-1, 10);
	BOOST_CHECK(cpu.read_memory(-1) == -1);
}

BOOST_AUTO_TEST_CASE(memory_write_overflow)
{
	CPU cpu;
	cpu.write_memory(65537, 10);
	BOOST_CHECK(cpu.read_memory(65537) == -1);
}

BOOST_AUTO_TEST_CASE(memory_write_test)
{
	CPU cpu;
	cpu.write_memory(100, 10);
	BOOST_CHECK(cpu.read_memory(100) == 10);
}

BOOST_AUTO_TEST_SUITE(Load_and_store)

BOOST_AUTO_TEST_CASE(load_A_positve)
{
	CPU cpu;
	srand(time(NULL));
	int random = rand() + 1;
	cpu.lda(random);
	BOOST_CHECK_MESSAGE(!cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_A_negative)
{
	CPU cpu;
	srand(time(NULL));
	int random = rand() + 1;
	random = random - random - random;
	cpu.lda(random);
	BOOST_CHECK_MESSAGE(cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_A_zero)
{
	CPU cpu;
	srand(time(NULL));
	cpu.lda(0);
	BOOST_CHECK(!cpu.is_N() && cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_X_positive)
{
	CPU cpu;
	srand(time(NULL));
	cpu.ldx(rand() + 1);
	BOOST_CHECK_MESSAGE(!cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_X_negative)
{
	CPU cpu;
	srand(time(NULL));
	int random = rand() + 1;
	random = random - random - random;
	cpu.ldx(random);
	BOOST_CHECK_MESSAGE(cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_X_zero)
{
	CPU cpu;
	srand(time(NULL));
	cpu.ldx(0);
	BOOST_CHECK(!cpu.is_N() && cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_Y_positive)
{
	CPU cpu;
	srand(time(NULL));
	cpu.ldy(rand() + 1);
	BOOST_CHECK_MESSAGE(!cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_Y_negative)
{
	CPU cpu;
	srand(time(NULL));
	int random = rand() + 1;
	random = random - random - random;
	cpu.ldy(random);
	BOOST_CHECK_MESSAGE(cpu.is_N() && !cpu.is_Z(), "ran: " << random << " N:" << cpu.is_Z() << " Z: " << cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(load_Y_zero)
{
	CPU cpu;
	srand(time(NULL));
	cpu.ldy(0);
	BOOST_CHECK(!cpu.is_N() && cpu.is_Z());
}

BOOST_AUTO_TEST_CASE(store_A_test)
{
	CPU cpu;
	srand(time(NULL));
	int address = (rand() + 1) % MEM_SIZE;
	int val = rand();
	cpu.lda(val);
	cpu.sta(address);
	BOOST_CHECK(cpu.read_memory(address) == val);
}

BOOST_AUTO_TEST_CASE(store_X_test)
{
	CPU cpu;
	srand(time(NULL));
	int address = (rand() + 1) % MEM_SIZE;
	int val = rand();
	cpu.ldx(val);
	cpu.stx(address);
	BOOST_CHECK(cpu.read_memory(address) == val);
}

BOOST_AUTO_TEST_CASE(store_Y_test)
{
	CPU cpu;
	srand(time(NULL));
	int address = (rand() + 1) % MEM_SIZE;
	int val = rand();
	cpu.ldy(val);
	cpu.sty(address);
	BOOST_CHECK(cpu.read_memory(address) == val);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(increment_decrement, CPU_fixture)

BOOST_AUTO_TEST_CASE(inc_test)
{
	int old_val = cpu.read_memory(address);
	cpu.inc(address);
	BOOST_CHECK(cpu.read_memory(address) == old_val + 1);
}

BOOST_AUTO_TEST_CASE(dec_test)
{
	int old_val = cpu.read_memory(address);
	cpu.dec(address);
	BOOST_CHECK(cpu.read_memory(address) == old_val - 1);
}



BOOST_AUTO_TEST_SUITE_END()