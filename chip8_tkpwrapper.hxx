#pragma once
#ifndef TKP_CHIP8_TKPWRAPPER_H
#define TKP_CHIP8_TKPWRAPPER_H
#include <include/emulator.h>
#include <include/TKPImage.h>
#include <chip8/chip8_inter.hxx>

namespace TKPEmu::Chip8 {
    class Chip8 : public Emulator {

    private:
        Interpreter inter_;
    };
}
#endif