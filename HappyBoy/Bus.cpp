#include "Bus.h"
#include "CPU.h"

#include <sstream>
#include <iomanip>

Bus::Bus(std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu) : ppu(ppu), apu(apu)
{
	
}

uint8_t Bus::read(uint16_t addr)
{
	/*uint8_t logo[] = {
	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,

		0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C
	};
	memcpy(ram + 0x8000, logo, 56);*/
	if (0x0000 <= addr && addr < 0x8000) {
		if (0x0000 <= addr && addr < 0x0100) {
			if (bootRomControl) {
				return cart[addr];
			}
			else {
				return bootrom[addr];
			}
		}
		else {
			return cart[addr];
		}
	}
	else if (0xE000 <= addr && addr < 0xFE00) {
		return 0xFF;
	}
	else if (0xFEA0 <= addr && addr < 0xFF00) {
		return 0xFF;
	}
	else if (addr == 0xFF00) {
		return 0xFF; // Joypad input, disabled for now
	}
	else if (addr == 0xFF04) {
		return cpu->counter >> 8;
	}
	else if (addr == 0xFF05) {
		return cpu->TIMA;
	}
	else if (addr == 0xFF06) {
		return cpu->TMA;
	}
	else if (addr == 0xFF07) {
		return cpu->TAC.reg;
	}
	else if (addr == 0xFF0F) {
		return cpu->IF.reg;
	}
	else if (addr == 0xFF10) {
		return apu->NR10.reg;
	}
	else if (addr == 0xFF11) {
		return apu->NR11.reg;
	}
	else if (addr == 0xFF12) {
		return apu->NR12.reg;
	}
	else if (addr == 0xFF13) {
		return 0xFF;
	}
	else if (addr == 0xFF14) {
		return apu->NR14.reg;
	}
	else if (addr == 0xFF16) {
		return apu->NR21.reg;
	}
	else if (addr == 0xFF17) {
		return apu->NR22.reg;
	}
	else if (addr == 0xFF18) {
		return 0xFF;
	}
	else if (addr == 0xFF19) {
		return apu->NR24.reg;
	}
	else if (addr == 0xFF1A) {
		return apu->NR30.reg;
	}
	else if (addr == 0xFF1B) {
		return apu->NR31;
	}
	else if (addr == 0xFF1C) {
		return apu->NR32.reg;
	}
	else if (addr == 0xFF1D) {
		return 0xFF;
	}
	else if (addr == 0xFF1E) {
		return apu->NR34.reg;
	}
	else if (addr == 0xFF20) {
		return apu->NR41.reg;
	}
	else if (addr == 0xFF21) {
		return apu->NR42.reg;
	}
	else if (addr == 0xFF22) {
		return apu->NR43.reg;
	}
	else if (addr == 0xFF23) {
		return apu->NR44.reg;
	}
	else if (addr == 0xFF24) {
		return apu->NR50.reg;
	}
	else if (addr == 0xFF25) {
		return apu->NR51.reg;
	}
	else if (addr == 0xFF26) {
		return apu->NR52.reg;
	}	
	else if (addr == 0xFF40) {
		return ppu->LCDC.reg;
	}
	else if (addr == 0xFF41) {
		return ppu->STAT.reg;
	}
	else if (addr == 0xFF42) {
		return ppu->SCY;
	}
	else if (addr == 0xFF43) {
		return ppu->SCX;
	}
	else if (addr == 0xFF44) {
		return ppu->LY;
	}
	else if (addr == 0xFF45) {
		return ppu->LYC;
	}
	else if (addr == 0xFF47) {
		return ppu->BGP.reg;
	}
	else if (addr == 0xFFFF) {
		return cpu->IE.reg;
	}
	else {
		return ram[addr];
	}
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (0x0000 <= addr && addr < 0x8000) {
		return;
	}
	else if (0xE000 <= addr && addr < 0xFE00) {
		return;
	}
	else if (0xFEA0 <= addr && addr < 0xFF00) {
		return;
	}
	else if (addr == 0xFF00) {
		return;
	}
	else if (addr == 0xFF04) {
		cpu->counter &= 0xFF;
	}
	else if (addr == 0xFF05) {
		cpu->TIMA = data;
	}
	else if (addr == 0xFF06) {
		cpu->TMA = data;
	}
	else if (addr == 0xFF07) {
		cpu->TAC.reg = data;
	}
	else if (addr == 0xFF0F) {
		cpu->IF.reg = data;
	}
	else if (addr == 0xFF10) {
		apu->NR10.reg = data;
	}
	else if (addr == 0xFF11) {
		apu->NR11.reg = data;
	}
	else if (addr == 0xFF12) {
		apu->NR12.reg = data;
	}
	else if (addr == 0xFF13) {
		apu->NR13 = data;
	}
	else if (addr == 0xFF14) {
		apu->NR14.reg = data;
	}
	else if (addr == 0xFF16) {
		apu->NR21.reg = data;
	}
	else if (addr == 0xFF17) {
		apu->NR22.reg = data;
	}
	else if (addr == 0xFF18) {
		apu->NR23 = data;
	}
	else if (addr == 0xFF19) {
		apu->NR24.reg = data;
	}
	else if (addr == 0xFF1A) {
		apu->NR30.reg = data;
	}
	else if (addr == 0xFF1B) {
		apu->NR31 = data;
	}
	else if (addr == 0xFF1C) {
		apu->NR32.reg = data;
	}
	else if (addr == 0xFF1D) {
		apu->NR33 = data;
	}
	else if (addr == 0xFF1E) {
		apu->NR34.reg = data;
	}
	else if (addr == 0xFF20) {
		apu->NR41.reg = data;
	}
	else if (addr == 0xFF21) {
		apu->NR42.reg = data;
	}
	else if (addr == 0xFF22) {
		apu->NR43.reg = data;
	}
	else if (addr == 0xFF23) {
		apu->NR44.reg = data;
	}
	else if (addr == 0xFF24) {
		apu->NR50.reg = data;
	}
	else if (addr == 0xFF25) {
		apu->NR51.reg = data;
	}
	else if (addr == 0xFF26) {
		apu->NR52.reg |= data & 0x80;
	}
	else if (addr == 0xFF40) {
		ppu->LCDC.reg = data;
	}
	else if (addr == 0xFF41) {
		ppu->STAT.reg = data;
	}
	else if (addr == 0xFF42) {
		ppu->SCY = data;
	}
	else if (addr == 0xFF43) {
		ppu->SCX = data;
	}
	else if (addr == 0xFF44) {
		ppu->LY = data;
	}
	else if (addr == 0xFF45) {
		ppu->LYC = data;
	}
	else if (addr == 0xFF46) {
		memcpy(ram + (data << 8), ram + 0xFE00, 160);
	}
	else if (addr == 0xFF47) {
		ppu->BGP.reg = data;
	}
	else if (addr == 0xFF50) {
		bootRomControl = data;
	}
	else if (addr == 0xFFFF) {
		cpu->IE.reg = data;
	}
	else {
		ram[addr] = data;
	}
}

std::vector<std::string> Bus::getMemory(uint16_t addr)
{
	std::vector<std::string> mem;
	int lines = 16;
	for (int i = 0; i < lines; i++) {
		std::stringstream s;
		s << "$" << std::setfill('0') << std::setw(4) << std::hex << addr << ": ";
		for (int j = 0; j < 16; j++) {
			s << " " << std::setw(2) << (int)ram[addr];
			addr++;
		}
		mem.push_back(s.str());
	}
	return mem;
}
