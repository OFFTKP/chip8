#ifndef TKP_CHIP8_INTER
#define TKP_CHIP8_INTER
#include <array>
#include <cstdint>

namespace TKPEmu::Chip8 {
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
    private:
        std::array<uint8_t, 16> regs_;
        uint16_t reg_i_ = 0;
        std::array<uint16_t, 16> stack_;
        std::array<uint8_t, 0x1000> mem_;
        uint16_t pc_ = 0x200;
        uint8_t sp_ = 0;
        void clear_screen();
        void reset();
        void run_8inst(const Opcode& opcode);
    };
}
#endif