#include "memory.h"

MemoryRegister::MemoryRegister() {}

MemoryRegister::MemoryRegister(Byte *_data)
{
	value = _data;
}

Byte MemoryRegister::get()
{
	return *value;
}

void MemoryRegister::set(Byte data)
{
	*value = data;
}

Memory::Memory()
{
	WRAM = vector<Byte>(0x2000); // $C000 - $DFFF, 8kB Working RAM
	ERAM = vector<Byte>(0x2000); // $A000 - $BFFF, 8kB switchable RAM bank, size liable to change in future
	ZRAM = vector<Byte>(0x0100); // $FF00 - $FFFF, 256 bytes of RAM
	VRAM = vector<Byte>(0x2000); // $8000 - $9FFF, 8kB Video RAM
	OAM  = vector<Byte>(0x00A0); // $FE00 - $FEA0, OAM Sprite RAM

	// Initialize Memory Register objects for easy reference
	TIMA = MemoryRegister(&ZRAM[0x05]);
	TMA  = MemoryRegister(&ZRAM[0x06]);
	TAC  = MemoryRegister(&ZRAM[0x07]);
	LCDC = MemoryRegister(&ZRAM[0x40]);
	SCY  = MemoryRegister(&ZRAM[0x42]);
	SCX  = MemoryRegister(&ZRAM[0x43]);
	LYC  = MemoryRegister(&ZRAM[0x45]);
	BGP  = MemoryRegister(&ZRAM[0x47]);
	ZBP0 = MemoryRegister(&ZRAM[0x48]);
	ZBP1 = MemoryRegister(&ZRAM[0x49]);
	WY 	 = MemoryRegister(&ZRAM[0x4A]);
	WX 	 = MemoryRegister(&ZRAM[0x4B]);
	IE 	 = MemoryRegister(&ZRAM[0xFF]);

	// The following memory locations are set to the following values after gameboy BIOS runs
	TIMA.set(0x00);
	TMA.set(0x00);
	TAC.set(0x00);
	LCDC.set(0x91);
	SCY.set(0x00);
	SCX.set(0x00);
	LYC.set(0x00);
	BGP.set(0xFC);
	ZBP0.set(0xFF);
	ZBP1.set(0xFF);
	WY.set(0x00);
	WX.set(0x00);
	IE.set(0x00);
}

void Memory::load_rom(std::string location)
{
	ifstream input(location, ios::binary);
	vector<Byte> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
	CART_ROM = buffer;
}

Byte Memory::read(Address location)
{
	switch (location & 0xF000)
	{
	// ROM0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return CART_ROM[location];

	// ROM1 (no bank switching)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return CART_ROM[location];

	// Graphics VRAM
	case 0x8000:
	case 0x9000:
		return VRAM[location & 0x1FFF];

	// External RAM
	case 0xA000:
	case 0xB000:
		return ERAM[location & 0x1FFF];

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
	case 0xE000:
		return WRAM[location & 0x1FFF];

	// Remaining Working RAM Shadow, I/O, Zero page RAM
	case 0xF000:
		switch (location & 0x0F00)
		{
			// Remaining Working RAM
			case 0x000: case 0x100: case 0x200: case 0x300:
			case 0x400: case 0x500: case 0x600: case 0x700:
			case 0x800: case 0x900: case 0xA00: case 0xB00:
			case 0xC00: case 0xD00:
				return WRAM[location & 0x1FFF];

			// Sprite OAM
			case 0xE00:
				if (location < 0xFEA0)
					return OAM[location & 0xFF];
				else
					return 0; // Empty but usable.. possibly return regular memory

			case 0xF00:
					return ZRAM[location & 0xFF];
		}
	}
}

void Memory::write(Address location, Byte data)
{
	switch (location & 0xF000)
	{
	// ROM0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		// CART_ROM[location] = data; - READ ONLY
		break;

	// ROM1 (no bank switching)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		// CART_ROM[location] = data; - READ ONLY
		break;

	// Graphics VRAM
	case 0x8000:
	case 0x9000:
		VRAM[location & 0x1FFF] = data;
		break;

	// External RAM
	case 0xA000:
	case 0xB000:
		ERAM[location & 0x1FFF] = data;
		break;

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
	case 0xE000:
		WRAM[location & 0x1FFF] = data;
		break;

	// Remaining Working RAM Shadow, I/O, Zero page RAM
	case 0xF000:
		switch (location & 0x0F00)
		{
		// Remaining Working RAM
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			WRAM[location & 0x1FFF] = data;
			break;

		// Sprite OAM
		case 0xE00:
			if (location < 0xFEA0)
			{
				OAM[location & 0xFF] = data;
				break;
			}
			else
				return; // Write here?

		case 0xF00:
			ZRAM[location & 0xFF] = data;
			break;
		}
	}
}