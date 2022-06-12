#pragma once
#ifndef TKP_CHIP8_TKPWRAPPER_H
#define TKP_CHIP8_TKPWRAPPER_H
#include <include/emulator.h>
#include <include/TKPImage.h>
#include <chip8/chip8_inter.hxx>

namespace TKPEmu::Chip8 {
	using Chip8Keys = std::array<SDL_Keycode, 16>;
    class Chip8 : public Emulator {
		TKP_EMULATOR(Chip8);
    private:
        Interpreter inter_;
		void update();
		Chip8Keys key_mappings_;
    };
}
#endif