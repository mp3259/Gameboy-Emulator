#include "cpu.h"

CPU::CPU()
{
	init();
}

// Initialize CPU internal data structures
void CPU::init()
{
	// Program Counter is initialized to $100
	// Stack Pointer is initialized to $FFFE

	// set meaningless register values for opcode debugging
	reg_A = 0;
	reg_B = 0;
	reg_C = 0;
	reg_D = 0;
	reg_E = 0;
	reg_F = 0;
	reg_H = 0;
	reg_L = 0;
	reg_SP = 0xFFFE;
	reg_PC = 0;
}

// Reproduces the effect of a reset signal sent to the CPU
void CPU::reset()
{
}

// Start emulation of CPU
void CPU::execute(int cycles)
{
}

void CPU::interrupt_signal()
{
}

void CPU::stop()
{
}

Address CPU::address(Byte high, Byte low)
{
	return (high << 8 | low);
}

Byte CPU::high_nibble(Byte target)
{
	return ((target >> 4) & 0xF);
}

Byte CPU::low_nibble(Byte target)
{
	return (target & 0xF);
}

Byte CPU::high_reg_pair(Byte_2 reg_pair)
{
	return (Byte) (reg_pair >> 8) & 0xFF;
}

Byte CPU::low_reg_pair(Byte_2 reg_pair)
{
	return (Byte) reg_pair;
}

void CPU::dec_reg_pair(Byte& high, Byte& low)
{
	Address addr = address(high, low) - 1;
	high = high_reg_pair(addr);
	low = low_reg_pair(addr);
}

void CPU::inc_reg_pair(Byte& high, Byte& low)
{
	Address addr = address(high, low) + 1;
	high = (Byte) (addr >> 8) & 0xFF;
	low = (Byte) addr;
}

void CPU::set_flag(int flag, bool value)
{
	if (value == true)
		reg_F |= flag;
	else
		reg_F &= ~(flag);
}

void CPU::debug()
{
	reg_A = 40;
	parse_opcode(0xE6);
}

int CPU::parse_opcode(Opcode code)
{
	int opbytes = 1;

	Byte value = 200;
	Byte value2 = 200;

	Byte addr1 = 0b00000000;
	Byte addr2 = 0b00000101;
	
	Byte test = 0b0000001100000000;

	// H = 2
	// L = 255

	reg_H = addr1;
	reg_L = addr2;

	switch (code)
	{
		// 65
		case 0x06: LD(reg_B, value); opbytes = 2; break;
		case 0x0E: LD(reg_C, value); opbytes = 2; break;
		case 0x16: LD(reg_D, value); opbytes = 2; break;
		case 0x1E: LD(reg_E, value); opbytes = 2; break;
		case 0x26: LD(reg_H, value); opbytes = 2; break;
		case 0x2E: LD(reg_L, value); opbytes = 2; break;
		// 66
		case 0x7F: LD(reg_A, reg_A); break;
		case 0x78: LD(reg_A, reg_B); break;
		case 0x79: LD(reg_A, reg_C); break;
		case 0x7A: LD(reg_A, reg_D); break;
		case 0x7B: LD(reg_A, reg_E); break;
		case 0x7C: LD(reg_A, reg_H); break;
		case 0x7D: LD(reg_A, reg_L); break;
		case 0x7E: LD(reg_A, address(reg_H, reg_L)); break;
		case 0x40: LD(reg_B, reg_B); break;
		case 0x41: LD(reg_B, reg_C); break;
		case 0x42: LD(reg_B, reg_D); break;
		case 0x43: LD(reg_B, reg_E); break;
		case 0x44: LD(reg_B, reg_H); break;
		case 0x45: LD(reg_B, reg_L); break;
		case 0x46: LD(reg_B, address(reg_H, reg_L)); break;
		case 0x48: LD(reg_C, reg_B); break;
		case 0x49: LD(reg_C, reg_C); break;
		case 0x4A: LD(reg_C, reg_D); break;
		case 0x4B: LD(reg_C, reg_E); break;
		case 0x4C: LD(reg_C, reg_H); break;
		case 0x4D: LD(reg_C, reg_L); break;
		case 0x4E: LD(reg_C, address(reg_H, reg_L)); break;
		case 0x50: LD(reg_D, reg_B); break;
		case 0x51: LD(reg_D, reg_C); break;
		// 67
		case 0x52: LD(reg_D, reg_D); break;
		case 0x53: LD(reg_D, reg_E); break;
		case 0x54: LD(reg_D, reg_H); break;
		case 0x55: LD(reg_D, reg_L); break;
		case 0x56: LD(reg_D, address(reg_H, reg_L)); break;
		case 0x58: LD(reg_E, reg_B); break;
		case 0x59: LD(reg_E, reg_C); break;
		case 0x5A: LD(reg_E, reg_D); break;
		case 0x5B: LD(reg_E, reg_E); break;
		case 0x5C: LD(reg_E, reg_H); break;
		case 0x5D: LD(reg_E, reg_L); break;
		case 0x5E: LD(reg_E, address(reg_H, reg_L)); break;
		case 0x60: LD(reg_H, reg_B); break;
		case 0x61: LD(reg_H, reg_C); break;
		case 0x62: LD(reg_H, reg_D); break;
		case 0x63: LD(reg_H, reg_E); break;
		case 0x64: LD(reg_H, reg_H); break;
		case 0x65: LD(reg_H, reg_L); break;
		case 0x66: LD(reg_H, address(reg_H, reg_L)); break;
		case 0x68: LD(reg_L, reg_B); break;
		case 0x69: LD(reg_L, reg_C); break;
		case 0x6A: LD(reg_L, reg_D); break;
		case 0x6B: LD(reg_L, reg_E); break;
		case 0x6C: LD(reg_L, reg_H); break;
		case 0x6D: LD(reg_L, reg_L); break;
		case 0x6E: LD(reg_L, address(reg_H, reg_L)); break;
		case 0x70: LD(address(reg_H, reg_L), reg_B); break;
		case 0x71: LD(address(reg_H, reg_L), reg_C); break;
		case 0x72: LD(address(reg_H, reg_L), reg_D); break;
		case 0x73: LD(address(reg_H, reg_L), reg_E); break;
		case 0x74: LD(address(reg_H, reg_L), reg_H); break;
		case 0x75: LD(address(reg_H, reg_L), reg_L); break;
		case 0x36: LD(address(reg_H, reg_L), value); opbytes = 2; break; // 8-bit immediate value
		// 68
		case 0x0A: LD(reg_A, address(reg_B, reg_C)); break;
		case 0x1A: LD(reg_A, address(reg_D, reg_E)); break;
		case 0xFA: LD(reg_A, address(value2, value)); opbytes = 3; break; // little-endian
		//case 0x3E: LD(reg_A, #); break; // I don't know what # represents...
		// 69
		case 0x47: LD(reg_B, reg_A); break;
		case 0x4F: LD(reg_C, reg_A); break;
		case 0x57: LD(reg_D, reg_A); break;
		case 0x5F: LD(reg_E, reg_A); break;
		case 0x67: LD(reg_H, reg_A); break;
		case 0x6F: LD(reg_L, reg_A); break;
		case 0x02: LD(address(reg_B, reg_C), reg_A); break;
		case 0x12: LD(address(reg_D, reg_E), reg_A); break;
		case 0x77: LD(address(reg_H, reg_L), reg_A); break;
		case 0xEA: LD(address(value2, value), reg_A); opbytes = 3; break; // little-endian
		// 70
		case 0xF2: LD(reg_A, memory.read(0xFF00 + reg_C)); break;
		case 0xE2: LD((0xFF00 + reg_C), reg_A); break;
		// 71
		case 0x3A: LD(reg_A, address(reg_H, reg_L)); dec_reg_pair(reg_H, reg_L); break; // LD A, (HL) .. dec HL
		// 72
		case 0x32: LD(address(reg_H, reg_L), reg_A); dec_reg_pair(reg_H, reg_L); break; // UNTESTED
		// 73
		case 0x2A: LD(reg_A, address(reg_H, reg_L)); inc_reg_pair(reg_H, reg_L); break; // UNTESTED
		// 74
		case 0x22: LD(address(reg_H, reg_L), reg_A); inc_reg_pair(reg_H, reg_L); break; // UNTESTED
		// 75
		case 0xE0: memory.write((0xFF00 + value), reg_A); break;// UNTESTED
		case 0xF0: LD(reg_A, memory.read(0xFF00 + value)); break; // UNTESTED
		// 76
		case 0x01: LD(reg_B, reg_C, value, value2); opbytes = 3; break;
		case 0x11: LD(reg_D, reg_E, value, value2); opbytes = 3; break;
		case 0x21: LD(reg_H, reg_L, value, value2); opbytes = 3; break;
		case 0x31: LD(reg_SP, value, value2); opbytes = 3; break;
		case 0xF9: LD(reg_SP, reg_L, reg_H); break;
		// 77
		case 0xF8: LD(reg_H, reg_L, (reg_SP + value)); opbytes = 2; break; // UNTESTED ... value is a SIGNED byte?
		// 78
		case 0x08: LD(address(value2, value), reg_SP); opbytes = 3; break; // UNTESTED
		case 0xF5: PUSH(reg_A, reg_F); break;
		case 0xC5: PUSH(reg_B, reg_C); break;
		case 0xD5: PUSH(reg_D, reg_E); break;
		case 0xE5: PUSH(reg_H, reg_L); break;
		// 79
		case 0xF1: POP(reg_A, reg_F); break;
		case 0xC1: POP(reg_B, reg_C); break;
		case 0xD1: POP(reg_D, reg_E); break;
		case 0xE1: POP(reg_H, reg_L); break;
		// 80
		case 0x87: ADD(reg_A, reg_A); break;
		case 0x80: ADD(reg_A, reg_B); break;
		case 0x81: ADD(reg_A, reg_C); break;
		case 0x82: ADD(reg_A, reg_D); break;
		case 0x83: ADD(reg_A, reg_E); break;
		case 0x84: ADD(reg_A, reg_H); break;
		case 0x85: ADD(reg_A, reg_L); break;
		case 0x86: ADD(reg_A, address(reg_H, reg_L)); break; // UNTESTED
		case 0xC6: ADD(reg_A, value); opbytes = 2;  break; // UNTESTED
		// 81
		case 0x8F: ADDC(reg_A, reg_A); break;
		case 0x88: ADDC(reg_A, reg_B); break;
		case 0x89: ADDC(reg_A, reg_C); break;
		case 0x8A: ADDC(reg_A, reg_D); break;
		case 0x8B: ADDC(reg_A, reg_E); break;
		case 0x8C: ADDC(reg_A, reg_H); break;
		case 0x8D: ADDC(reg_A, reg_L); break;
		case 0x8E: ADDC(reg_A, address(reg_H, reg_L)); break;
		case 0xCE: ADDC(reg_A, value); opbytes = 2; break; // UNTESTED
		// 82
		case 0x97: SUB(reg_A, reg_A); break;
		case 0x90: SUB(reg_A, reg_B); break;
		case 0x91: SUB(reg_A, reg_C); break;
		case 0x92: SUB(reg_A, reg_D); break;
		case 0x93: SUB(reg_A, reg_E); break;
		case 0x94: SUB(reg_A, reg_H); break;
		case 0x95: SUB(reg_A, reg_L); break;
		case 0x96: SUB(reg_A, address(reg_H, reg_L)); break; // UNTESTED
		case 0xD6: SUB(reg_A, value); opbytes = 2; break; // UNTESTED
		// 83
		case 0x9F: SUBC(reg_A, reg_A); break;
		case 0x98: SUBC(reg_A, reg_B); break;
		case 0x99: SUBC(reg_A, reg_C); break;
		case 0x9A: SUBC(reg_A, reg_D); break;
		case 0x9B: SUBC(reg_A, reg_E); break;
		case 0x9C: SUBC(reg_A, reg_H); break;
		case 0x9D: SUBC(reg_A, reg_L); break;
		case 0x9E: SUBC(reg_A, address(reg_H, reg_L)); break;
		// case 0x??: SUBC(reg_A, value); opbytes = 2;  break; // Opcode ?? ? in manual
		// 84
		case 0xA7: AND(reg_A, reg_A); break;
		case 0xA0: AND(reg_A, reg_B); break;
		case 0xA1: AND(reg_A, reg_C); break;
		case 0xA2: AND(reg_A, reg_D); break;
		case 0xA3: AND(reg_A, reg_E); break;
		case 0xA4: AND(reg_A, reg_H); break;
		case 0xA5: AND(reg_A, reg_L); break;
		case 0xA6: AND(reg_A, address(reg_H, reg_L)); break;
		case 0xE6: AND(reg_A, value); opbytes = 2; break;
		// 85
		case 0xB7: OR(reg_A, reg_A); break;
		case 0xB0: OR(reg_A, reg_B); break;
		case 0xB1: OR(reg_A, reg_C); break;
		case 0xB2: OR(reg_A, reg_D); break;
		case 0xB3: OR(reg_A, reg_E); break;
		case 0xB4: OR(reg_A, reg_H); break;
		case 0xB5: OR(reg_A, reg_L); break;
		case 0xB6: OR(reg_A, address(reg_H, reg_L)); break;
		case 0xF6: OR(reg_A, value); opbytes = 2; break;
		// 86
		case 0xAF: XOR(reg_A, reg_A); break;
		case 0xA8: XOR(reg_A, reg_B); break;
		case 0xA9: XOR(reg_A, reg_C); break;
		case 0xAA: XOR(reg_A, reg_D); break;
		case 0xAB: XOR(reg_A, reg_E); break;
		case 0xAC: XOR(reg_A, reg_H); break;
		case 0xAD: XOR(reg_A, reg_L); break;
		case 0xAE: XOR(reg_A, address(reg_H, reg_L)); break;
		case 0xEE: XOR(reg_A, value); opbytes = 2; break; // * parameter in manual, assumed immediate value
		// 87
		case 0xBF: CP(reg_A, reg_A); break;
		case 0xB8: CP(reg_A, reg_B); break;
		case 0xB9: CP(reg_A, reg_C); break;
		case 0xBA: CP(reg_A, reg_D); break;
		case 0xBB: CP(reg_A, reg_E); break;
		case 0xBC: CP(reg_A, reg_H); break;
		case 0xBD: CP(reg_A, reg_L); break;
		case 0xBE: CP(reg_A, address(reg_H, reg_L)); break;
		case 0xFE: CP(reg_A, value); opbytes = 2; break;
		// 88
		case 0x3C: INC(reg_A); break;
		case 0x04: INC(reg_B); break;
		case 0x0C: INC(reg_C); break;
		case 0x14: INC(reg_D); break;
		case 0x1C: INC(reg_E); break;
		case 0x24: INC(reg_H); break;
		case 0x2C: INC(reg_L); break;
		case 0x34: INC(address(reg_H, reg_L)); break;
		// 89
		case 0x3D: DEC(reg_A); break;
		case 0x05: DEC(reg_B); break;
		case 0x0D: DEC(reg_C); break;
		case 0x15: DEC(reg_D); break;
		case 0x1D: DEC(reg_E); break;
		case 0x25: DEC(reg_H); break;
		case 0x2D: DEC(reg_L); break;
		case 0x35: DEC(address(reg_H, reg_L)); break;
	}

	return opbytes;
}

// 8-bit loads

void CPU::LD(Byte& destination, Byte value)
{
	destination = value;
}

void CPU::LD(Byte& destination, Address addr)
{
	destination = memory.read(addr);
}

void CPU::LD(Address addr, Byte value)
{
	memory.write(addr, value);
}


// 16-bit loads

void CPU::LD(Byte_2& reg, Byte lsb, Byte msb)
{
	reg = address(msb, lsb);
}

void CPU::LD(Byte& reg_high, Byte& reg_low, Byte lsb, Byte msb)
{
	reg_high = msb;
	reg_low = lsb;
}

void CPU::LD(Byte& reg_high, Byte& reg_low, Byte_2 value)
{
	reg_high = high_reg_pair(value);
	reg_low = low_reg_pair(value);

	set_flag(FLAG_ZERO, false);
	set_flag(FLAG_SUB, false);
	// H - Set or reset according to operation???
	// C - Set or reset according to operation???
}

void CPU::LD(Address destination, Byte_2 value)
{
	// These high-low may possibly need to be reversed
	memory.write(destination++, low_reg_pair(value));
	memory.write(destination, high_reg_pair(value));
}

// Stack Operations

void CPU::PUSH(Byte high, Byte low)
{
	memory.write(--reg_SP, high);
	memory.write(--reg_SP, low);
}

void CPU::POP(Byte& high, Byte& low)
{
	low = memory.read(reg_SP++);
	high = memory.read(reg_SP++);
}

// ALU Operations

void CPU::ADD(Byte& target, Byte value)
{
	int result = target + value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is 0
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) + (value & 0xF)) & 0x10)); // Set if carry from bit 3
	set_flag(FLAG_CARRY, (result > 0xFF)); // Set if carry from bit 7

	target += value;
}

void CPU::ADD(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADD(target, val);
}

void CPU::ADDC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	ADD(target, value);
	target += carry;
}

void CPU::ADDC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADDC(target, val);
}

void CPU::SUB(Byte& target, Byte value)
{
	int result = target - value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is zero
	set_flag(FLAG_SUB, true); // set
	// GB CPU manual says to set this flag if there is NO borrow from bit 4, Z80 manual says otherwise?
	// as of now it sets half carry if there was a borrow
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - (value & 0xF)) < 0));
	set_flag(FLAG_CARRY, (result < 0)); // set if borrow

	target -= value;
}

void CPU::SUB(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SUB(target, val);
}

void CPU::SUBC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	SUB(target, value);
	target -= carry;
}

void CPU::SUBC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SUBC(target, val);
}

void CPU::AND(Byte& target, Byte value)
{
	target &= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, true);
	set_flag(FLAG_CARRY, false);
}

void CPU::AND(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	AND(target, val);
}

void CPU::OR(Byte& target, Byte value)
{
	target |= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, false);
}

void CPU::OR(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	OR(target, val);
}

void CPU::XOR(Byte& target, Byte value)
{
	target ^= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, false);
}

void CPU::XOR(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	OR(target, val);
}

// Compare A with n. This is basically a A - n subtraction but the results are thrown away
void CPU::CP(Byte& target, Byte value)
{
	int result = target - value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is zero
	set_flag(FLAG_SUB, true); // set
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - (value & 0xF)) < 0));
	set_flag(FLAG_CARRY, (result < 0)); // set if borrow
}

void CPU::CP(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	CP(target, val);
}

void CPU::INC(Byte& target)
{
	int result = target + 1;

	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) + 1) & 0x10));

	target++;
}

void CPU::INC(Address addr)
{
	Byte value = memory.read(addr);
	INC(value);
	memory.write(addr, value);
}

void CPU::DEC(Byte& target)
{
	int result = target - 1;

	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, true);
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - 1) < 0));

	target--;
}

void CPU::DEC(Address addr)
{
	Byte value = memory.read(addr);
	DEC(value);
	memory.write(addr, value);
}