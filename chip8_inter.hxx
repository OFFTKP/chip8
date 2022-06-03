#ifndef TKP_CHIP8_INTER
#define TKP_CHIP8_INTER
#include <array>
#include <cstdint>
#include <string>

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
    };
    class Interpreter {
    public:
        Interpreter();
        void Run(Opcode opcode);
        float* GetScreenData() {
            return &screen_color_data_[0];
        }
        Opcode GetNextOpcode() {
            Opcode ret;
            ret.full = (mem_[pc_] << 8) | (mem_[pc_ + 1]);
            pc_ += 2;
            return ret;
        }
    private:
        std::array<uint8_t, 16> regs_;
        std::array<uint64_t, 32> screen_;
		std::array<float, 4 * 64 * 32> screen_color_data_;
        uint16_t i_ = 0;
        uint8_t dt_ = 0;
        uint8_t st_ = 0;
        std::array<uint16_t, 16> stack_;
        std::array<uint8_t, 0x1000> mem_;
        uint16_t pc_ = 0x200;
        uint8_t sp_ = 0;
        bool should_draw_ = false;
        void clear_screen();
        void redraw(size_t line_start, size_t lines, size_t x_start);
        void reset();
        inline void run_8inst(const Opcode& opcode);
        inline void run_Finst(const Opcode& opcode);
        bool load_file(const std::string& path);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
        friend class TKPEmu::Chip8::Chip8;
    };
}
#endif