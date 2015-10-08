#include "../headers/CPU.hpp"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CPU_test
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdint.h>

using namespace std;

struct CPU_fixture {

	CPU cpu;
	uint16_t address;
	uint8_t value;
	uint8_t value_2;
	uint8_t value_negative;

	CPU_fixture() {
		srand(time(NULL));
		address = (rand() + 1) % MEM_SIZE;
		value = (rand() + 1) % (UINT8_MAX + 1);
		value_2 = (rand() + 1) % (UINT8_MAX + 1);
		value_negative = (~value) + 1;
		cpu.write_memory(address, value);
	};

	~CPU_fixture() {

	};
};

BOOST_FIXTURE_TEST_SUITE(Load_and_store, CPU_fixture)

BOOST_AUTO_TEST_CASE(lda_test)
{
	cpu.lda(address);
	BOOST_CHECK(cpu.get_A() == value);
	if (value == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(ldx_test)
{
	cpu.ldx(address);
	BOOST_CHECK(cpu.get_X() == value);
	if (value == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(ldy_test)
{
	cpu.ldy(address);
	BOOST_CHECK(cpu.get_Y() == value);
	if (value == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(sta_test)
{
	cpu.set_A(value);
	BOOST_CHECK(cpu.read_memory(address) == value);
}

BOOST_AUTO_TEST_CASE(stx_test)
{
	cpu.set_X(value);
	BOOST_CHECK(cpu.read_memory(address) == value);
}

BOOST_AUTO_TEST_CASE(sty_test)
{
	cpu.set_Y(value);
	BOOST_CHECK(cpu.read_memory(address) == value);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(increment_decrement, CPU_fixture)

BOOST_AUTO_TEST_CASE(inc_test)
{
	cpu.inc(address);
	BOOST_CHECK(cpu.read_memory(address) == value + 1);
	if (value + 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value + 1 < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(inc_overflow_test)
{
	cpu.write_memory(address, UINT8_MAX);
	cpu.inc(address);
	BOOST_CHECK(cpu.read_memory(address) == 0);
	if ((uint8_t)(UINT8_MAX + 1) == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)UINT8_MAX + 1 < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(inx_test)
{
	cpu.set_X(value);
	cpu.inx();
	BOOST_CHECK(cpu.get_X() == value + 1);
	if (value + 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value - 1) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(iny_test)
{
	cpu.set_Y(value);
	cpu.iny();
	BOOST_CHECK(cpu.get_Y() == value + 1);
	if (value + 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value - 1) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(dec_test)
{
	cpu.dec(address);
	BOOST_CHECK(cpu.read_memory(address) == value - 1);
	if (value - 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value - 1) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(dec_underflow_test)
{
	cpu.write_memory(address, 0);
	cpu.dec(address);
	// BOOST_CHECK(cpu.read_memory(address) == 0 - 1);
	if (0 - 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)0 - 1 < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(dex_test)
{
	cpu.set_X(value);
	cpu.dex();
	BOOST_CHECK(cpu.get_X() == value - 1);
	if (value - 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value - 1 < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(dey_test)
{
	cpu.set_Y(value);
	cpu.dey();
	BOOST_CHECK(cpu.get_Y() == value - 1);
	if (value - 1 == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)value - 1 < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Logical, CPU_fixture)

BOOST_AUTO_TEST_CASE(and_test)
{
	cpu.set_A(value_2);
	cpu._and(address);
	BOOST_CHECK(cpu.get_A() == (value_2 & value));
	if ((value_2 & value) == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value_2 & value) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(ora_test)
{
	cpu.set_A(value_2);
	cpu.ora(address);
	BOOST_CHECK(cpu.get_A() == (value_2 | value));
	if ((value_2 | value) == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value_2 | value) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_CASE(eor_test)
{
	cpu.set_A(value_2);
	cpu.eor(address);
	BOOST_CHECK(cpu.get_A() == (value_2 ^ value));
	if ((value_2 ^ value) == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value_2 ^ value) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Jump_branch_compare_test, CPU_fixture)

BOOST_AUTO_TEST_CASE(jmp_test)
{
	cpu.jmp(address);
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(cmp_with_carry_test)
{
	cpu.set_A(value + 1);
	cpu.cmp(address);
	BOOST_CHECK(cpu.is_C());
	// Z and N flag is true, trivially
}

BOOST_AUTO_TEST_CASE(cmp_without_carry_test)
{
	cpu.set_A(value - 1);
	cpu.cmp(address);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cmp_test)
{
	cpu.set_A(value_2);
	cpu.cmp(address);
	if (value_2 - value >= 0) {
		BOOST_CHECK(cpu.is_C());
	} else {
		BOOST_CHECK(!cpu.is_C());
	}
	if (value_2 - value == 0) {
		BOOST_CHECK(cpu.is_Z());
	} else {
		BOOST_CHECK(!cpu.is_Z());
	}
	if ((int8_t)(value_2 - value) < 0) {
		BOOST_CHECK(cpu.is_N());
	} else {
		BOOST_CHECK(!cpu.is_N());
	}
}

BOOST_AUTO_TEST_SUITE_END()