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
	uint16_t address_2;
	uint8_t value;
	uint8_t value_2;
	uint8_t value_negative;

	CPU_fixture() {
		srand(time(NULL));
		address        = (rand() + 1) % (UINT16_MAX + 1);
		address_2      = (rand() + 1) % (UINT16_MAX + 1);
		value          = (rand() + 1) % (UINT8_MAX + 1);
		value_2        = (rand() + 1) % (UINT8_MAX + 1);
		value_negative = -value;
		cpu.write_memory(address, value);
	};

	~CPU_fixture() {

	};
};

struct CPU_instructin_fixture {
	CPU cpu;
	uint8_t low;
	uint8_t high;
	uint8_t opcode;

	CPU_instructin_fixture() {
		srand(time(NULL));
		low    = (rand() % (UINT8_MAX + 1)) + 1;
		high   = (rand() % (UINT8_MAX + 1)) + 1;
		opcode = 0;
		cpu.write_memory(cpu.get_PC(), opcode);
		cpu.write_memory(cpu.get_PC() + 1, low);
		cpu.write_memory(cpu.get_PC() + 2, high);
	};

	~CPU_instructin_fixture() {

	};
};

BOOST_FIXTURE_TEST_SUITE(Load_and_store, CPU_fixture)

BOOST_AUTO_TEST_CASE(lda_from_memory_test)
{
	cpu.lda(address, true);
	BOOST_CHECK(cpu.get_A() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(lda_from_value_test)
{
	cpu.lda(value, false);
	BOOST_CHECK(cpu.get_A() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(ldx_from_memory_test)
{
	cpu.ldx(address, true);
	BOOST_CHECK(cpu.get_X() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(ldx_from_value_test)
{
	cpu.ldx(value, false);
	BOOST_CHECK(cpu.get_X() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(ldy_from_memory_test)
{
	cpu.ldy(address, true);
	BOOST_CHECK(cpu.get_Y() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(ldy_from_value_test)
{
	cpu.ldy(value, false);
	BOOST_CHECK(cpu.get_Y() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
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
	BOOST_CHECK(cpu.is_Z() == (value + 1 == 1));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value + 1 < 0));
}

BOOST_AUTO_TEST_CASE(inc_overflow_test)
{
	cpu.write_memory(address, UINT8_MAX);
	cpu.inc(address);
	BOOST_CHECK(cpu.read_memory(address) == 0);
	BOOST_CHECK(cpu.is_Z() == ((uint8_t)(UINT8_MAX + 1) == 0));
	BOOST_CHECK(cpu.is_N() == ((uint8_t)(UINT8_MAX + 1) < 0));
}

BOOST_AUTO_TEST_CASE(inx_test)
{
	cpu.set_X(value);
	cpu.inx();
	BOOST_CHECK(cpu.get_X() == value + 1);
	BOOST_CHECK(cpu.is_Z() == (value + 1 == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value + 1) < 0));
}

BOOST_AUTO_TEST_CASE(iny_test)
{
	cpu.set_Y(value);
	cpu.iny();
	BOOST_CHECK(cpu.get_Y() == value + 1);
	BOOST_CHECK(cpu.is_Z() == (value + 1 == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value - 1) < 0));
}

BOOST_AUTO_TEST_CASE(dec_test)
{
	cpu.dec(address);
	BOOST_CHECK(cpu.read_memory(address) == value - 1);
	BOOST_CHECK(cpu.is_Z() == (value - 1 == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value - 1) < 0));
}

BOOST_AUTO_TEST_CASE(dec_underflow_test)
{
	cpu.write_memory(address, 0);
	cpu.dec(address);
	// BOOST_CHECK(cpu.read_memory(address) == 0 - 1);
	BOOST_CHECK(cpu.is_Z() == (-1 == 0));
	BOOST_CHECK(cpu.is_N() == (-1 < 0));
}

BOOST_AUTO_TEST_CASE(dex_test)
{
	cpu.set_X(value);
	cpu.dex();
	BOOST_CHECK(cpu.get_X() == value - 1);
	BOOST_CHECK(cpu.is_Z() == (value - 1 == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value - 1) < 0));
}

BOOST_AUTO_TEST_CASE(dey_test)
{
	cpu.set_Y(value);
	cpu.dey();
	BOOST_CHECK(cpu.get_Y() == value - 1);
	BOOST_CHECK(cpu.is_Z() == (value - 1 == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value - 1) < 0));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Logical, CPU_fixture)

BOOST_AUTO_TEST_CASE(and_from_memory_test)
{
	cpu.set_A(value_2);
	cpu._and(address, true);
	BOOST_CHECK(cpu.get_A() == (value_2 & value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 & value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 & value)) < 0) );
}

BOOST_AUTO_TEST_CASE(and_from_value_test)
{
	cpu.set_A(value_2);
	cpu._and(value, false);
	BOOST_CHECK(cpu.get_A() == (value_2 & value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 & value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 & value)) < 0) );
}

BOOST_AUTO_TEST_CASE(ora_from_memory_test)
{
	cpu.set_A(value_2);
	cpu.ora(address, true);
	BOOST_CHECK(cpu.get_A() == (value_2 | value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 | value) == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 | value) < 0));
}

BOOST_AUTO_TEST_CASE(ora_from_value_test)
{
	cpu.set_A(value_2);
	cpu.ora(value, false);
	BOOST_CHECK(cpu.get_A() == (value_2 | value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 | value) == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 | value) < 0));
}

BOOST_AUTO_TEST_CASE(eor_from_memory_test)
{
	cpu.set_A(value_2);
	cpu.eor(address, true);
	BOOST_CHECK(cpu.get_A() == (value_2 ^ value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 ^value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 ^ value)) < 0));
}

BOOST_AUTO_TEST_CASE(eor_from_value_test)
{
	cpu.set_A(value_2);
	cpu.eor(value, false);
	BOOST_CHECK(cpu.get_A() == (value_2 ^value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 ^value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 ^ value)) < 0));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Jump_branch_compare_test, CPU_fixture)

BOOST_AUTO_TEST_CASE(jmp_test)
{
	cpu.jmp(address);
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(cmp_from_memory_with_carry_test)
{
	cpu.set_A(value + 1);
	cpu.cmp(address, true);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cmp_from_value_carry_test)
{
	cpu.set_A(value + 1);
	cpu.cmp(value, false);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cmp_from_memory_without_carry_test)
{
	cpu.set_A(value - 1);
	cpu.cmp(address, true);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cmp_from_value_without_carry_test)
{
	cpu.set_A(value - 1);
	cpu.cmp(value, false);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cmp_from_memory_test)
{
	cpu.set_A(value_2);
	cpu.cmp(address, true);
	BOOST_CHECK(cpu.is_Z() == (value_2 - value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 - value) < 0));
}

BOOST_AUTO_TEST_CASE(cmp_from_value_test)
{
	cpu.set_A(value_2);
	cpu.cmp(value, false);
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
	BOOST_CHECK(cpu.is_Z() == (value_2 - value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 - value) < 0));
}

BOOST_AUTO_TEST_CASE(cpx_from_memory_with_carry_test)
{
	cpu.set_X(value + 1);
	cpu.cpx(address, true);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpx_from_value_with_carry_test)
{
	cpu.set_X(value + 1);
	cpu.cpx(value, false);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpx_without_carry_test)
{
	cpu.set_X(value - 1);
	cpu.cpx(address, true);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpx_from_value_without_carry_test)
{
	cpu.set_X(value - 1);
	cpu.cpx(value, false);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpx_from_memory_test)
{
	cpu.set_X(value_2);
	cpu.cpx(address, true);
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
	BOOST_CHECK(cpu.is_Z() == (value_2 - value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 - value) < 0));
}

BOOST_AUTO_TEST_CASE(cpx_from_value_test)
{
	cpu.set_X(value_2);
	cpu.cpx(value, false);
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
	BOOST_CHECK(cpu.is_Z() == (value_2 - value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)(value_2 - value) < 0));
}

BOOST_AUTO_TEST_CASE(cpy_from_memory_with_carry_test)
{
	cpu.set_Y(value + 1);
	cpu.cpy(address, true);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpy_from_value_with_carry_test)
{
	cpu.set_Y(value + 1);
	cpu.cpy(value, false);
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpy_from_memory_without_carry_test)
{
	cpu.set_Y(value - 1);
	cpu.cpy(address, true);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpy_from_value_without_carry_test)
{
	cpu.set_Y(value - 1);
	cpu.cpy(value, false);
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cpy_from_memory_test)
{
	cpu.set_Y(value_2);
	cpu.cpy(address, true);
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 - value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 - value)) < 0));
}

BOOST_AUTO_TEST_CASE(cpy_from_value_test)
{
	cpu.set_Y(value_2);
	cpu.cpy(value, false);
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
	BOOST_CHECK(cpu.is_Z() == ((value_2 - value) == 0));
	BOOST_CHECK(cpu.is_N() == (((int8_t)(value_2 - value)) < 0));
}

BOOST_AUTO_TEST_CASE(bcc_with_carry_test)
{
	cpu.set_PC(value_2);
	cpu.set_C(true);
	cpu.bcc(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bcc_without_carry_test)
{
	cpu.set_PC(value_2);
	cpu.set_C(false);
	cpu.bcc(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bcs_with_carry_test)
{
	cpu.set_PC(value_2);
	cpu.set_C(true);
	cpu.bcs(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bcs_without_carry_test)
{
	cpu.set_PC(value_2);
	cpu.set_C(false);
	cpu.bcs(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(beq_with_zero_test)
{
	cpu.set_Z(true);
	cpu.set_PC(value_2);
	cpu.beq(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(beq_without_zero_test)
{
	cpu.set_Z(false);
	cpu.set_PC(value_2);
	cpu.beq(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bne_with_zero_test)
{
	cpu.set_Z(true);
	cpu.set_PC(value_2);
	cpu.bne(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bne_without_zero_test)
{
	cpu.set_Z(false);
	cpu.set_PC(value_2);
	cpu.bne(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bmi_negative_test)
{
	cpu.set_N(true);
	cpu.set_PC(value_2);
	cpu.bmi(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bmi_positive_test)
{
	cpu.set_N(false);
	cpu.set_PC(value_2);
	cpu.bmi(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bpl_negative_test)
{
	cpu.set_N(true);
	cpu.set_PC(value_2);
	cpu.bpl(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bpl_positive_test)
{
	cpu.set_N(false);
	cpu.set_PC(value_2);
	cpu.bpl(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bvs_with_overflow_test)
{
	cpu.set_V(true);
	cpu.set_PC(value_2);
	cpu.bvs(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bvs_without_overflow_test)
{
	cpu.set_V(false);
	cpu.set_PC(value_2);
	cpu.bvs(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bvc_with_overflow_test)
{
	cpu.set_V(true);
	cpu.set_PC(value_2);
	cpu.bvc(value);
	BOOST_CHECK(cpu.get_PC() == value_2);
}

BOOST_AUTO_TEST_CASE(bvc_without_overflow_test)
{
	cpu.set_V(false);
	cpu.set_PC(value_2);
	cpu.bvc(value);
	BOOST_CHECK(cpu.get_PC() == value + value_2);
}

BOOST_AUTO_TEST_CASE(bit_test)
{
	cpu.set_A(value_2);
	cpu.bit(address);
	bool val = ((value_2 & value) == 0) ? true : false;
	BOOST_CHECK(cpu.is_Z() == val);
	BOOST_CHECK(cpu.is_V() == (((value_2 & value) >> 6) & 1));
	BOOST_CHECK(cpu.is_N() == (((value_2 & value) >> 7) & 1));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(Shift_rotate_tests, CPU_fixture)

BOOST_AUTO_TEST_CASE(asl_from_memory_test)
{
	cpu.asl(address, true);
	uint8_t temp = ((value << 1) & FE);
	BOOST_CHECK(cpu.read_memory(address) == temp);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (temp == 0));
	BOOST_CHECK(cpu.is_N() == ((temp & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(asl_from_A_test)
{
	cpu.set_A(value);
	cpu.asl(address, false);
	uint8_t shift = ((value << 1) & FE);
	BOOST_CHECK(cpu.get_A() == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(lsr_from_memory_test)
{
	cpu.lsr(address, true);
	uint8_t shift = (value >> 1) & BYTE_SIGN_UNSET_MAX;
	BOOST_CHECK(cpu.read_memory(address) == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(lsr_from_A_test)
{
	cpu.set_A(value);
	cpu.lsr(address, false);
	uint8_t shift = ((value >> 1) & BYTE_SIGN_UNSET_MAX);
	BOOST_CHECK(cpu.get_A() == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}


BOOST_AUTO_TEST_CASE(rol_from_memory)
{
	cpu.rol(address, true);
	uint8_t shift = cpu.rot_l(value);
	BOOST_CHECK(cpu.read_memory(address) == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(rol_from_A_test)
{
	cpu.set_A(value);
	cpu.rol(address, false);
	uint8_t shift = cpu.rot_l(value);
	BOOST_CHECK(cpu.get_A() == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(ror_from_memory)
{
	cpu.ror(address, true);
	uint8_t shift = cpu.rot_r(value);
	BOOST_CHECK(cpu.read_memory(address) == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(ror_from_A_test)
{
	cpu.set_A(value);
	cpu.ror(address, false);
	uint8_t shift = cpu.rot_r(value);
	BOOST_CHECK(cpu.get_A() == shift);
	BOOST_CHECK(cpu.is_C() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(Transfer_tests, CPU_fixture)

BOOST_AUTO_TEST_CASE(tax_test)
{
	cpu.set_A(value);
	cpu.tax();
	BOOST_CHECK(cpu.get_X() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(tay_test)
{
	cpu.set_A(value);
	cpu.tay();
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(txa_test)
{
	cpu.set_X(value);
	cpu.txa();
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_CASE(tya_test)
{
	cpu.set_Y(value);
	cpu.tya();
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((int8_t)value < 0));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(Stack_tests, CPU_fixture)

BOOST_AUTO_TEST_CASE(tsx_test)
{
	cpu.set_SP(value);
	cpu.tsx();
	BOOST_CHECK(cpu.get_X() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((value & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(txs_test)
{
	cpu.set_X(value);
	cpu.txs();
	BOOST_CHECK(cpu.get_SP() == value);
}

BOOST_AUTO_TEST_CASE(pha_test)
{
	cpu.set_A(value);
	cpu.pha();
	BOOST_CHECK(cpu.pull() == value);
}

BOOST_AUTO_TEST_CASE(php_test)
{
	cpu.set_PS(value);
	cpu.php();
	BOOST_CHECK(cpu.pull() == value);
}

BOOST_AUTO_TEST_CASE(plp_test)
{
	cpu.push(value);
	cpu.plp();
	BOOST_CHECK(cpu.get_PS() == value);
	BOOST_CHECK(cpu.is_C() == (value & 1));
	BOOST_CHECK(cpu.is_Z() == ((value >> 1) & 1));
	BOOST_CHECK(cpu.is_I() == ((value >> 2) & 1));
	BOOST_CHECK(cpu.is_D() == ((value >> 3) & 1));
	BOOST_CHECK(cpu.is_B() == ((value >> 4) & 1));
	BOOST_CHECK(cpu.is_V() == ((value >> 6) & 1));
	BOOST_CHECK(cpu.is_N() == ((value >> 7) & 1));
}

BOOST_AUTO_TEST_CASE(pla_test)
{
	cpu.push(value);
	cpu.pla();
	BOOST_CHECK(cpu.get_A() == value);
	BOOST_CHECK(cpu.is_Z() == (value == 0));
	BOOST_CHECK(cpu.is_N() == ((value >> 7) & 1));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(Subroutine_tests, CPU_fixture)

BOOST_AUTO_TEST_CASE(jsr_test)
{
	cpu.set_PC(address);
	cpu.jsr(address_2);
	BOOST_CHECK(cpu.get_PC() == address_2);
	uint16_t temp = cpu.pull_address();
	BOOST_CHECK(temp == address - 1);
}

BOOST_AUTO_TEST_CASE(rts_test)
{
	cpu.push_address(address);
	cpu.rts();
	BOOST_CHECK(cpu.get_PC() == address + 1);
}

BOOST_AUTO_TEST_CASE(rti_test)
{
	cpu.push_address(address);
	cpu.push(value);
	cpu.rti();
	BOOST_CHECK(cpu.get_PS() == value);
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(CPU_execution_test, CPU_fixture)

BOOST_AUTO_TEST_CASE(bcc_call_test)
{
	cpu.write_memory(cpu.get_PC(), 0x29);
}

BOOST_AUTO_TEST_SUITE_END()

