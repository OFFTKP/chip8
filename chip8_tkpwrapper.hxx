#pragma once
#ifndef TKP_CHIP8_TKPWRAPPER_H
#define TKP_CHIP8_TKPWRAPPER_H
#include <include/emulator.h>
#include <include/TKPImage.h>

namespace TKPEmu::Chip8 {
    class Chip8 : public Emulator {

		std::array<float, 4 * 64 * 32> screen_color_data_{};
    };
}
#endif