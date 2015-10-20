#include "headers/CPU.hpp"
#include <iostream>

/*
 * Calls the proper instruction depending on the opcpde. 
 * Even when an 8-bit value is expected the function should work properly
 * by using the 16-bit address value. Typecasting ensures that the lower 8 bits
 * are passed the the function.
 */
void CPU::execute(uint8_t opcode, uint16_t address)
{
	switch(opcode) {
		// ADC
		case ADC_IMMEDIATE: 
			adc(address, false);
			break;

		case ADC_ZERO_PAGE:case ADC_ZERO_PAGE_X: 
		case ADC_ABSOLUTE: case ADC_ABSOLUTE_X: case ADC_ABSOLUTE_Y:
		case ADC_INDIRECT_X: case ADC_INDIRECT_Y:
			adc(address, true);
			break;

		// AND
		case AND_IMMEDIATE: 
			_and(address, false);
			break;

		case AND_ZERO_PAGE: case AND_ZERO_PAGE_X:
		case AND_ABSOLUTE: case AND_ABSOLUTE_X: case AND_ABSOLUTE_Y:
		case AND_INDIRECT_X: case AND_INDIRECT_Y:
			_and(address, true);
			break;

		// ASL - A
		case ASL_ACCUMULATOR:
			asl(0, false);
			break;

		// ASL - Memory
		case ASL_ZERO_PAGE: case ASL_ZERO_PAGE_X: case ASL_ABSOLUTE:
		case ASL_ABSOLUTE_X:
			asl(address, true);
			break;

		//BCC
		case BCC:
			bcc(address);
			break;

		// BCS
		case BCS:
			bcs(address);
			break;

		// BEQ
		case BEQ:
			beq(address);
			break;

		// BIT
		case BIT_ZERO_PAGE: case BIT_ABSOLUTE:
			bit(address);
			break;

		// BMI
		case BMI:
			bmi(address);
			break;

		// BNE
		case BNE:
			bne(address);
			break;

		// BPL
		case BPL:
			bpl(address);
			break;

		// BRK
		case BRK:
			brk();
			break;

		// BVC
		case BVC:
			bvc(address);
			break;

		// BVS
		case BVS:
			bvs(address);
			break;

		// CLC
		case CLC:
			clc();
			break;

		// CLD
		case CLD:
			cld();
			break;

		// CLI
		case CLI:
			cli();
			break;

		// CLV
		case CLV:
			clv();
			break;

		// CMP
		case CMP_IMMEDIATE:
			cmp(address, false);
			break;

		case CMP_ZERO_PAGE: case CMP_ZERO_PAGE_X:
		case CMP_ABSOLUTE: case CMP_ABSOLUTE_X: case CMP_ABSOLUTE_Y:
		case CMP_INDIRECT_X: case CMP_INDIRECT_Y:
			cmp(address, true);
			break;

		// CPX
		case CPX_IMMEDIATE: 
			cpx(address, false);
			break;

		case CPX_ZERO_PAGE: case CPX_ABSOLUTE:
			cpx(address, true);
			break;

		// CPY
		case CPY_IMMEDIATE: 
			cpy(address, false);
			break;

		case CPY_ZERO_PAGE: case CPY_ABSOLUTE:
			cpy(address, true);
			break;

		// DEC
		case DEC_ZERO_PAGE: case DEC_ZERO_PAGE_X: case DEC_ABSOLUTE:
		case DEC_ABSOLUTE_X:
			dec(address);
			break;

		// DEX
		case DEX:
			dex();
			break;

		// DEY
		case DEY:
			dey();
			break;

		// EOR
		case EOR_IMMEDIATE: 
			eor(address, false);
			break;

		case EOR_ZERO_PAGE: case EOR_ZERO_PAGE_X:
		case EOR_ABSOLUTE: case EOR_ABSOLUTE_X: case EOR_ABSOLUTE_Y:
		case EOR_INDIRECT_X: case EOR_INDIRECT_Y:
			eor(address, true);
			break;

		// INC
		case INC_ZERO_PAGE: case INC_ZERO_PAGE_X: case INC_ABSOLUTE:
		case INC_ABSOLUTE_X:
			inc(address);
			break;

		// INX
		case INX:
			inx();
			break;

		// INY
		case INY:
			iny();
			break;

		// JMP
		case JMP_ABSOLUTE: case JMP_INDIRECT:
			jmp(address);
			break;

		// JSR
		case JSR:
			jsr(address);
			break;

		// LDA
		case LDA_IMMEDIATE: 
			lda(address, false);
			break;

		case LDA_ZERO_PAGE: case LDA_ZERO_PAGE_X:
		case LDA_ABSOLUTE: case LDA_ABSOLUTE_X: case LDA_ABSOLUTE_Y:
		case LDA_INDIRECT_X: case LDA_INDIRECT_Y:
			lda(address, true);
			break;

		// LDX
		case LDX_IMMEDIATE: 
			ldx(address, false);
			break;

		case LDX_ZERO_PAGE: case LDX_ZERO_PAGE_Y:
		case LDX_ABSOLUTE: case LDX_ABSOLUTE_X:
			ldx(address, true);
			break;

		// LDY
		case LDY_IMMEDIATE:
			ldy(address, false);
			break;

		case LDY_ZERO_PAGE: case LDY_ZERO_PAGE_X:
		case LDY_ABSOLUTE: case LDY_ABSOLUTE_X:
			ldy(address, true);
			break;

		// LSR - A
		case LSR_ACCUMULATOR:
			lsr(0, false);
			break;

		// LSR - Memory
		case LSR_ZERO_PAGE: case LSR_ZERO_PAGE_X: case LSR_ABSOLUTE:
		case LSR_ABSOLUTE_X:
			lsr(address, true);
			break;

		// NOP
		case NOP:
			nop();
			break;

		// ORA
		case ORA_IMMEDIATE: 
			ora(address, false);
			break;

		case ORA_ZERO_PAGE: case ORA_ZERO_PAGE_X:
		case ORA_ABSOLUTE: case ORA_ABSOLUTE_X: case ORA_ABSOLUTE_Y:
		case ORA_INDIRECT_X: case ORA_INDIRECT_Y:
			ora(address, true);
			break;

		// PHA
		case PHA:
			pha();
			break;

		// PHP
		case PHP:
			php();
			break;

		// PLA
		case PLA:
			pla();
			break;

		// PLP
		case PLP:
			plp();
			break;

		// ROL - A
		case ROL_ACCUMULATOR:
			rol(address, false);
			break;

		// ROL - Memory
		case ROL_ZERO_PAGE: case ROL_ZERO_PAGE_X: case ROL_ABSOLUTE:
		case ROL_ABSOLUTE_X:
			rol(address, true);
			break;

		// ROR - A
		case ROR_ACCUMULATOR:
			ror(address, false);
			break;

		// ROR - Memory
		case ROR_ZERO_PAGE: case ROR_ZERO_PAGE_X: case ROR_ABSOLUTE:
		case ROR_ABSOLUTE_X:
			ror(address, true);
			break;

		// RTI
		case RTI:
			rti();
			break;

		// RTS
		case RTS:
			rts();
			break;

		// SBC
		case SBC_IMMEDIATE: 
			sbc(address, false);
			break;

		case SBC_ZERO_PAGE: case SBC_ZERO_PAGE_X:
		case SBC_ABSOLUTE: case SBC_ABSOLUTE_X: case SBC_ABSOLUTE_Y:
		case SBC_INDIRECT_X: case SBC_INDIRECT_Y:
			sbc(address, true);
			break;

		// SEC
		case SEC:
			sec();
			break;

		// SED
		case SED:
			sed();
			break;

		// SEI
		case SEI:
			sei();
			break;

		// STA
		case STA_ZERO_PAGE: case STA_ZERO_PAGE_X: case STA_ABSOLUTE:
		case STA_ABSOLUTE_X: case STA_ABSOLUTE_Y: case STA_INDIRECT_X:
		case STA_INDIRECT_Y:
			sta(address);
			break;

		// STX
		case STX_ZERO_PAGE: case STX_ZERO_PAGE_Y: case STX_ABSOLUTE:
			stx(address);
			break;

		// STY
		case STY_ZERO_PAGE: case STY_ZERO_PAGE_X: case STY_ABSOLUTE:
			sty(address);
			break;

		// TAX
		case TAX:
			tax();
			break;

		// TAY
		case TAY:
			tay();
			break;

		// TSX
		case TSX:
			tsx();
			break;

		// TXA
		case TXA:
			txa();
			break;

		// TXS
		case TXS:
			txs();
			break;

		// TYA
		case TYA:
			tya();
			break;

		default:
		cerr << "Unsupported instruction called: " << hex << "0x" << opcode << endl;
		break;
	}
}