#include <chip8/chip8_tkpwrapper.hxx>

namespace TKPEmu::Chip8 {
    Chip8::Chip8() {
        EmulatorImage.width = 64;
        EmulatorImage.height = 32;
    }
    Chip8::Chip8(std::any args) : Chip8() {}
    bool Chip8::load_file(std::string path) {
        Loaded = true;
        return true;
    }
}