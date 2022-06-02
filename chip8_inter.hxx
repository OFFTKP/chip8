#ifndef TKP_CHIP8_INTER
#define TKP_CHIP8_INTER
#include <array>
#include <cstdint>

namespace TKPEmu::Chip8 {
    class Chip8;
    union Opcode {
        uint16_t full;
        struct {
            uint16_t _4 : 4;
            uint16_t _3 : 4;
            uint16_t _2 : 4;
            uint16_t _1 : 4;
        };
        struct {
            uint16_t _2b : 8;
            uint16_t _1b : 8;
        };
        struct {
            uint16_t _3last : 12;
            uint16_t _3last_other : 4;
        };
    };
    class Interpreter {
    public:
        Interpreter();
        void Run(Opcode opcode);
        float* GetScreenData() {
            return screen_color_data_.data();
        }
    private:
        std::array<uint8_t, 16> regs_;
        std::array<uint64_t, 32> screen_;
		std::array<float, 4 * 64 * 32> screen_color_data_{};
        uint16_t reg_i_ = 0;
        std::array<uint16_t, 16> stack_;
        std::array<uint8_t, 0x1000> mem_;
        uint16_t pc_ = 0x200;
        uint8_t sp_ = 0;
        bool should_draw_ = true;
        void clear_screen();
        void redraw();
        void reset();
        void run_8inst(const Opcode& opcode);
        uint8_t read(uint16_t addr);
        std::array<uint8_t, 0x50> font_ {
            0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0,
            0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80,
            0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
            0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80,
            0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80
        };
        friend class TKPEmu::Chip8::Chip8;
    };
}
#endif