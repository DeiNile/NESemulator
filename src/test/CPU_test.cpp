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
	uint8_t value;
	uint8_t value_2;
	uint16_t pc;
	uint16_t address;

	CPU_instructin_fixture() {
		srand(time(NULL));
		opcode  = 0;
		low     = (rand() % (UINT8_MAX + 1)) + 1;
		high    = (rand() % (UINT8_MAX + 1)) + 1;
		address = ((high << 8) | low);
		value   = (rand() % (UINT8_MAX + 1)) + 1;
		value_2 = (rand() % (UINT8_MAX + 1)) + 1;
		pc      = cpu.get_PC();
		cpu.write_memory(pc, opcode);
		cpu.write_memory(pc + 1, low);
		cpu.write_memory(pc + 2, high);
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

BOOST_AUTO_TEST_CASE(brk_test)
{
	cpu.set_PC(address);
	cpu.set_P(value);
	cpu.brk();
	BOOST_CHECK(cpu.is_B());
	BOOST_CHECK(cpu.pull() == value);
	BOOST_CHECK(cpu.pull() == (uint8_t)address);
	BOOST_CHECK(cpu.pull() == ((uint8_t)(address >> BYTE_LENGTH)));
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
	BOOST_CHECK(cpu.is_C() == ((value & 1) != 0));
	BOOST_CHECK(cpu.is_Z() == (shift == 0));
	BOOST_CHECK(cpu.is_N() == ((shift & BYTE_SIGN_BIT_SET_MASK) != 0));
}

BOOST_AUTO_TEST_CASE(lsr_from_A_test)
{
	cpu.set_A(value);
	cpu.lsr(address, false);
	uint8_t shift = ((value >> 1) & BYTE_SIGN_UNSET_MAX);
	BOOST_CHECK(cpu.get_A() == shift);
	BOOST_CHECK(cpu.is_C() == ((value & 1) != 0));
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
	cpu.set_P(value);
	cpu.php();
	BOOST_CHECK(cpu.pull() == value);
}

BOOST_AUTO_TEST_CASE(plp_test)
{
	cpu.push(value);
	cpu.plp();
	BOOST_CHECK(cpu.get_P() == value);
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
	BOOST_CHECK(cpu.get_P() == value);
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(CPU_execution_test, CPU_instructin_fixture)

BOOST_AUTO_TEST_CASE(and_immediate_call_test)
{
	cpu.write_memory(pc, 0x29);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == (value_2 & low));
}

BOOST_AUTO_TEST_CASE(and_memory_test)
{
	cpu.set_X(value_2); // Set up X's displacement
	cpu.set_A(value_2); // Set up original value of A
	cpu.write_memory(pc, AND_ABSOLUTE_X); // write opcode
	cpu.write_memory(address + value_2, value); // write & value at correct addr
	cpu.execute_instruction(); 
	BOOST_CHECK(cpu.get_A() == (value_2 & value));
}

BOOST_AUTO_TEST_CASE(asl_accumulator_call_test)
{

	cpu.write_memory(pc, ASL_ACCUMULATOR);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == ((value << 1) & FE));
}

BOOST_AUTO_TEST_CASE(asl_memory_call_test)
{
	cpu.write_memory(pc, ASL_ABSOLUTE);
	cpu.write_memory(address, value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory(address) == ((value << 1) & FE));
}

BOOST_AUTO_TEST_CASE(bcc_with_branching_call_test)
{
	cpu.write_memory(pc, BCC);
	cpu.set_C(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bcc_without_branching_call_test)
{
	cpu.write_memory(pc, BCC);
	cpu.set_C(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(bcs_with_branching_call_test)
{
	cpu.write_memory(pc, BCS);
	cpu.set_C(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bcs_without_branching_call_test)
{
	cpu.write_memory(pc, BCS);
	cpu.set_C(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(beq_with_branching_call_test)
{
	cpu.write_memory(pc, BEQ);
	cpu.set_Z(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(beq_without_branching_call_test)
{
	cpu.write_memory(pc, BEQ);
	cpu.set_Z(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(bit_zero_page_call_test)
{
	cpu.write_memory(pc, BIT_ZERO_PAGE);
	cpu.write_memory(low, value);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 & value) == 0));
	BOOST_CHECK(cpu.is_V() == (((value_2 & value) & (1 << 6)) != 0));
	BOOST_CHECK(cpu.is_N() == (((value_2 & value) & (1 << 7)) != 0));
}

BOOST_AUTO_TEST_CASE(bit_absolute_call_test)
{
	cpu.write_memory(pc, BIT_ABSOLUTE);
	cpu.write_memory(address, value);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 & value) == 0));
	BOOST_CHECK(cpu.is_V() == (((value_2 & value) & (1 << 6)) != 0));
	BOOST_CHECK(cpu.is_N() == (((value_2 & value) & (1 << 7)) != 0));
}

BOOST_AUTO_TEST_CASE(bmi_with_branching_call_test)
{
	cpu.write_memory(pc, BMI);
	cpu.set_N(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bmi_without_branching_call_test)
{
	cpu.write_memory(pc, BMI);
	cpu.set_N(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(bne_with_branching_call_test)
{
	cpu.write_memory(pc, BNE);
	cpu.set_Z(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bne_without_branching_call_test)
{
	cpu.write_memory(pc, BNE);
	cpu.set_Z(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(bpl_with_branching_call_test)
{
	cpu.write_memory(pc, BPL);
	cpu.set_N(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bpl_without_branching_call_test)
{
	cpu.write_memory(pc, BPL);
	cpu.set_N(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(brk_call_test)
{
	cpu.write_memory(pc, BRK);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_B());
}

BOOST_AUTO_TEST_CASE(bvc_with_branching_call_test)
{
	cpu.write_memory(pc, BVC);
	cpu.set_V(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bvc_without_branching_call_test)
{
	cpu.write_memory(pc, BVC);
	cpu.set_V(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));	
}

BOOST_AUTO_TEST_CASE(bvs_with_branching_call_test)
{
	cpu.write_memory(pc, BVS);
	cpu.set_V(true);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2 + (uint8_t)address));
}

BOOST_AUTO_TEST_CASE(bvs_without_branching_call_test)
{
	cpu.write_memory(pc, BVS);
	cpu.set_V(false);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == (pc + 2));
}

BOOST_AUTO_TEST_CASE(clc_call_test)
{
	cpu.write_memory(pc, CLC);
	cpu.execute_instruction();
	BOOST_CHECK(!cpu.is_C());
}

BOOST_AUTO_TEST_CASE(cld_call_test)
{
	cpu.write_memory(pc, CLD);
	cpu.execute_instruction();
	BOOST_CHECK(!cpu.is_D());
}

BOOST_AUTO_TEST_CASE(cli_call_test)
{
	cpu.write_memory(pc, CLI);
	cpu.execute_instruction();
	BOOST_CHECK(!cpu.is_I());
}

BOOST_AUTO_TEST_CASE(clv_call_test)
{
	cpu.write_memory(pc, CLV);
	cpu.execute_instruction();
	BOOST_CHECK(!cpu.is_V());
}

BOOST_AUTO_TEST_CASE(cmp_immediate_call_test)
{
	cpu.write_memory(pc, CMP_IMMEDIATE);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - low) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= low));
}

BOOST_AUTO_TEST_CASE(cmp_zero_page_call_test)
{
	cpu.write_memory(pc, CMP_ZERO_PAGE);
	cpu.write_memory(low, value);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - value) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
}

BOOST_AUTO_TEST_CASE(cpx_immediate_call_test)
{
	cpu.write_memory(pc, CPX_IMMEDIATE);
	cpu.set_X(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - low) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= low));
}

BOOST_AUTO_TEST_CASE(cpx_zero_page_call_test)
{
	cpu.write_memory(pc, CPX_ZERO_PAGE);
	cpu.write_memory(low, value);
	cpu.set_X(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - value) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
}

BOOST_AUTO_TEST_CASE(cpy_immediate_call_test)
{
	cpu.write_memory(pc, CPY_IMMEDIATE);
	cpu.set_Y(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - low) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= low));
}

BOOST_AUTO_TEST_CASE(cpy_absolute_call_test)
{
	cpu.write_memory(pc, CPY_ABSOLUTE);
	cpu.write_memory(address, value);
	cpu.set_Y(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value_2 - value) == 0));
	BOOST_CHECK(cpu.is_C() == (value_2 >= value));
}

BOOST_AUTO_TEST_CASE(dec_zero_page_x_call_test)
{
	cpu.write_memory(pc, DEC_ZERO_PAGE_X);
	cpu.set_X(value_2);
	cpu.write_memory((uint8_t)(low + value_2), value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory((uint8_t)(low + value_2)) == (uint8_t)(value - 1));
}

BOOST_AUTO_TEST_CASE(dex_call_test)
{
	cpu.write_memory(pc, DEX);
	cpu.set_X(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == value_2 - 1);
}

BOOST_AUTO_TEST_CASE(dey_call_test)
{
	cpu.write_memory(pc, DEY);
	cpu.set_Y(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_Y() == value_2 - 1);
}

BOOST_AUTO_TEST_CASE(eor_immediate_call_test)
{
	cpu.write_memory(pc, EOR_IMMEDIATE);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == (value_2 ^ low));
}

BOOST_AUTO_TEST_CASE(eor_indirect_x_call_test)
{
	cpu.write_memory(pc, EOR_INDIRECT_X);
	cpu.write_memory((uint8_t)(low + value), low);
	cpu.write_memory((uint8_t)(low + value + 1), high);
	cpu.set_A(value_2);
	cpu.set_X(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == (value_2 ^ value));
}

BOOST_AUTO_TEST_CASE(inc_zero_page_call_test)
{
	cpu.write_memory(pc, INC_ZERO_PAGE);
	cpu.write_memory(low, value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory(low) == value + 1);
}

BOOST_AUTO_TEST_CASE(inx_call_test)
{
	cpu.write_memory(pc, INX);
	cpu.set_X(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == value + 1);
}

BOOST_AUTO_TEST_CASE(iny_call_test)
{
	cpu.write_memory(pc, INY);
	cpu.set_Y(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_Y() == value + 1);
}

BOOST_AUTO_TEST_CASE(jmp_absolute_call_test)
{
	cpu.write_memory(pc, JMP_ABSOLUTE);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(jmp_indirect_call_test)
{
	cpu.write_memory(pc, JMP_INDIRECT);
	cpu.write_memory(address, value_2);
	cpu.write_memory(address + 1, value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == ((value << BYTE_LENGTH) | value_2));
}

BOOST_AUTO_TEST_CASE(jsr_call_test)
{
	cpu.write_memory(pc, JSR);
	cpu.set_SP(address);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(lda_immediate_call_test)
{
	cpu.write_memory(pc, LDA_IMMEDIATE);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == low);
}

BOOST_AUTO_TEST_CASE(lda_indirect_y_call_test)
{
	cpu.write_memory(pc, LDA_INDIRECT_Y);
	cpu.set_Y(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == cpu.read_memory(address + value));
}

BOOST_AUTO_TEST_CASE(ldx_immediate_call_test)
{
	cpu.write_memory(pc, LDX_IMMEDIATE);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == low);
}

BOOST_AUTO_TEST_CASE(ldx_zero_page_y_call_test)
{
	cpu.write_memory(pc, LDX_ZERO_PAGE_Y);
	cpu.set_Y(value);
	cpu.write_memory((uint8_t)(low + value), value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == value_2);
}

BOOST_AUTO_TEST_CASE(ldy_immediate_call_test)
{
	cpu.write_memory(pc, LDY_IMMEDIATE);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_Y() == low);
}

BOOST_AUTO_TEST_CASE(ldy_zero_page_x_call_test)
{
	cpu.write_memory(pc, LDY_ZERO_PAGE_X);
	cpu.set_X(value);
	cpu.write_memory((uint8_t)(low + value), value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_Y() == value_2);
}

BOOST_AUTO_TEST_CASE(ora_immediate_call_test)
{
	cpu.write_memory(pc, ORA_IMMEDIATE);
	cpu.set_A(value_2);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == (value_2 | low));
}

BOOST_AUTO_TEST_CASE(pha_call_test)
{
	cpu.write_memory(pc, PHA);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.pull() == value);
}

BOOST_AUTO_TEST_CASE(php_call_test)
{
	cpu.write_memory(pc, PHP);
	cpu.set_P(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.pull() == value);
}

BOOST_AUTO_TEST_CASE(pla_call_test)
{
	cpu.write_memory(pc, PLA);
	cpu.push(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == value);
}

BOOST_AUTO_TEST_CASE(plp_call_test)
{
	cpu.write_memory(pc, PLP);
	cpu.push(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_P() == value);
}

BOOST_AUTO_TEST_CASE(rol_accumulator_call_test)
{
	cpu.write_memory(pc, ROL_ACCUMULATOR);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value << 1) == 0));
}

BOOST_AUTO_TEST_CASE(rol_absolute_call_test)
{
	cpu.write_memory(pc, ROL_ABSOLUTE);
	cpu.write_memory(address, value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_Z() == ((value << 1) == 0));
}

BOOST_AUTO_TEST_CASE(rti_call_test)
{
	cpu.write_memory(pc, RTI);
	cpu.push_address(address);
	cpu.push(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_P() == value);
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(rts_call_test)
{
	cpu.write_memory(pc, RTS);
	cpu.push_address(address - 1);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_PC() == address);
}

BOOST_AUTO_TEST_CASE(sec_call_test)
{
	cpu.write_memory(pc, SEC);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_C());
}

BOOST_AUTO_TEST_CASE(sed_call_test)
{
	cpu.write_memory(pc, SED);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_D());
}

BOOST_AUTO_TEST_CASE(sei_call_test)
{
	cpu.write_memory(pc, SEI);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.is_I());
}

BOOST_AUTO_TEST_CASE(sta_absolute_y_call_test)
{
	cpu.write_memory(pc, STA_ABSOLUTE_Y);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory(address) == value);
}

BOOST_AUTO_TEST_CASE(stx_zero_page_call_test)
{
	cpu.write_memory(pc, STX_ZERO_PAGE);
	cpu.set_X(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory(low) == value);
}

BOOST_AUTO_TEST_CASE(sty_absolute_call_test)
{
	cpu.write_memory(pc, STY_ABSOLUTE);
	cpu.set_Y(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.read_memory(address) == value);
}

BOOST_AUTO_TEST_CASE(tax_call_test)
{
	cpu.write_memory(pc, TAX);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == value);
}

BOOST_AUTO_TEST_CASE(tay_call_test)
{
	cpu.write_memory(pc, TAY);
	cpu.set_A(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_Y() == value);	
}

BOOST_AUTO_TEST_CASE(tsx_call_test)
{
	cpu.write_memory(pc, TSX);
	cpu.set_SP(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_X() == value);
}

BOOST_AUTO_TEST_CASE(txa_call_test)
{
	cpu.write_memory(pc, TXA);
	cpu.set_X(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == value);
}

BOOST_AUTO_TEST_CASE(txs_call_test)
{
	cpu.write_memory(pc, TXS);
	cpu.set_X(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_SP() == value);
}

BOOST_AUTO_TEST_CASE(tya_call_test)
{
	cpu.write_memory(pc, TYA);
	cpu.set_Y(value);
	cpu.execute_instruction();
	BOOST_CHECK(cpu.get_A() == value);
}

BOOST_AUTO_TEST_SUITE_END()

