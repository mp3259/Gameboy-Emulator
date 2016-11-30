#include "emulator.h"

Emulator::Emulator()
{
	cpu.init(&memory);
	display.init(&memory);
}

// Start emulation of CPU
void Emulator::run(int total_iterations)
{
	// CPU cycles to emulate per frame draw
	int cycles_per_frame = cpu.CLOCK_SPEED / display.framerate;

	// Current cycle in frame
	int current_cycle = 0;

	for (int i = 0; i < total_iterations; i++)
	{
		while (current_cycle < cycles_per_frame)
		{
			Opcode code = memory.read(cpu.reg_PC);

			if (cpu.reg_PC == 0x282A) {
				bool breakpoint = true;
			}

			cpu.parse_opcode(code);
			current_cycle += cpu.num_cycles;
			update_timers(cpu.num_cycles);
			update_scanline(cpu.num_cycles);
			do_interrupts();
			cpu.num_cycles = 0;
		}
		// Render();
		// Reset counter for next frame
		current_cycle = 0;
	}
}

void Emulator::update_divider(int cycles)
{
	divider_counter += cycles;

	if (divider_counter >= 256) // 16384 Hz
	{
		divider_counter = 0;
		memory.DIV.set(memory.DIV.get() + 1);
	}
}

// Opcode cycle number may need adjusted, used Nintendo values
void Emulator::update_timers(int cycles)
{
	update_divider(cycles);

	// This can be optimized if needed
	Byte new_freq = get_timer_frequency();

	if (timer_frequency != new_freq)
	{
		set_timer_frequency();
		timer_frequency = new_freq;
	}

	if (timer_enabled())
	{
		timer_counter -= cycles;

		// enough cpu clock cycles have happened to update timer
		if (timer_counter <= 0)
		{
			Byte timer_value = memory.TIMA.get();
			set_timer_frequency();

			// Timer will overflow, generate interrupt
			if (timer_value == 255)
			{
				memory.TIMA.set(memory.TMA.get());
				request_interrupt(INTERRUPT_TIMER);
			}
			else
			{
				memory.TIMA.set(timer_value + 1);
			}
		}
	}
}

bool Emulator::timer_enabled()
{
	return memory.TAC.is_bit_set(BIT_2);
}

Byte Emulator::get_timer_frequency()
{
	return (memory.TAC.get() & 0x3);
}

void Emulator::set_timer_frequency()
{
	Byte frequency = get_timer_frequency();
	timer_frequency = frequency;

	switch (frequency)
	{
		// timer_counter calculated by (Clock Speed / selected frequency)
	case 0: timer_counter = 1024; break; // 4096 Hz
	case 1: timer_counter = 16; break; // 262144 Hz
	case 2: timer_counter = 64; break; // 65536 Hz
	case 3: timer_counter = 256; break; // 16384 Hz
	}
}

void Emulator::request_interrupt(Byte id)
{
	memory.IF.set_bit(id);
}

void Emulator::do_interrupts()
{
	// If master flag is enabled
	if (cpu.interrupt_master_enable)
	{
		// If there are any interrupts set
		if (memory.IF.get() > 0)
		{
			// Loop through each bit and call interrupt for lowest . highest priority bits set
			for (int i = 0; i < 5; i++)
			{
				if (memory.IF.is_bit_set(i))
				{
					if (memory.IE.is_bit_set(i))
					{
						service_interrupt(i);
					}
				}
			}
		}
	}
}

void Emulator::service_interrupt(Byte id)
{
	cpu.interrupt_master_enable = false;
	memory.IF.clear_bit(id);

	// Push current execution address to stack
	memory.write(--cpu.reg_SP, high_byte(cpu.reg_PC));
	memory.write(--cpu.reg_SP, low_byte(cpu.reg_PC));

	switch (id)
	{
	case INTERRUPT_VBLANK: cpu.reg_PC = 0x40; break;
	case INTERRUPT_LCDC:   cpu.reg_PC = 0x48; break;
	case INTERRUPT_TIMER:  cpu.reg_PC = 0x50; break;
	case INTERRUPT_JOYPAD: cpu.reg_PC = 0x60; break;
	}
}

void Emulator::set_lcd_status()
{
	Byte status = memory.STAT.get();

	if (display.is_lcd_enabled() == false)
	{
		// Set mode to 1 during LCD disabled and reset scanline
		scanline_counter = 456;
		memory.LY.clear();
		status &= 252;
		status = set_bit(status, 0);
		memory.STAT.set(status);
		return;
	}

	Byte current_line = memory.LY.get();
	// extract current LCD mode
	Byte current_mode = status & 0x03;

	Byte mode = 0;
	bool do_interrupt = false;

	// in VBLANK, set mode to 1
	if (current_line >= 144)
	{
		mode = 1; // In vertical blanking period
		// 1 binary
		status = set_bit(status, BIT_0);
		status = clear_bit(status, BIT_1);
		do_interrupt = is_bit_set(status, BIT_4);

	}
	else
	{
		int mode2_threshold = 456 - 80;
		int mode3_threshold = mode2_threshold - 172;

		if (scanline_counter >= mode2_threshold)
		{
			mode = 2; // Searching OAM RAM
			// 2 binary
			status = set_bit(status, BIT_1);
			status = clear_bit(status, BIT_0);
			do_interrupt = is_bit_set(status, BIT_5);
		}
		else if (scanline_counter >= mode3_threshold)
		{
			mode = 3; // Transferring data to LCD driver
			// 3 binary
			status = set_bit(status, BIT_1);
			status = set_bit(status, BIT_0);
		}
		else
		{
			mode = 0; // CPU has access to all display RAM
			// 0 binary
			status = clear_bit(status, BIT_1);
			status = clear_bit(status, BIT_0);
			do_interrupt = is_bit_set(status, BIT_3);
		}
	}

	// Entered new mode, request interrupt
	if (do_interrupt && (mode != current_mode))
		request_interrupt(INTERRUPT_LCDC);

	// check coincidence flag, set bit 2 if it matches
	if (memory.LY.get() == memory.LYC.get())
	{
		status = set_bit(status, BIT_2);

		if (is_bit_set(status, BIT_6))
			request_interrupt(INTERRUPT_LCDC);
	}
	// clear bit 2 if not
	else
		status = clear_bit(status, BIT_2);

	memory.STAT.set(status);
}

void Emulator::update_scanline(int cycles)
{
	set_lcd_status();

	if (display.is_lcd_enabled())
		scanline_counter -= cycles;
	else
		return;

	if (scanline_counter <= 0)
	{
		Byte current_scanline = memory.LY.get();
		// increment scanline and reset counter
		memory.LY.set(current_scanline + 1);
		scanline_counter = 456;

		// Entered VBLANK period
		if (current_scanline == 144)
			request_interrupt(0);
		// Reset counter if past maximum
		else if (current_scanline > 153)
			memory.LY.clear();
		// Draw the next scanline
		else if (current_scanline < 144)
			display.draw_scanline();
	}
}