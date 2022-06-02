#include <chip8/chip8_inter.hxx>
#include <bit>

namespace TKPEmu::Chip8 {
    Interpreter::Interpreter() {}
    void Interpreter::reset() {
        pc_ = 0x200;
        std::fill(regs_.begin(), regs_.end(), 0);
        reg_i_ = 0;
        std::fill(stack_.begin(), stack_.end(), 0);
        clear_screen();
    }

    void Interpreter::Run(Opcode opcode) {
        switch (opcode._1) {
            case 0: {
                if (opcode.full == 0x00E0) {
                    clear_screen();
                } else if (opcode.full == 0x00EE) {

                }
                break;
            }
            case 1: {
                pc_ = 0xFFF & opcode.full;
                break;
            }
            case 2: {
                pc_ = 0xFFF & opcode.full;
                ++sp_ &= 0xF;
                stack_[sp_] = pc_;
                break;
            }
            case 3: {
                pc_ += (regs_[opcode._2] == opcode._2b) * 2;
                break;
            }
            case 4: {
                pc_ += (regs_[opcode._2] != opcode._2b) * 2;
                break;
            }
            case 5: {
                pc_ += (regs_[opcode._2] == regs_[opcode._3]) * 2;
                break;
            }
            case 6: {
                regs_[opcode._2] = opcode._2b;
                break;
            }
            case 7: {
                regs_[opcode._2] += opcode._2b;
                break;
            }
            case 8: {
                run_8inst(opcode);
                break;
            }
            case 9: {
                pc_ += (regs_[opcode._2] != regs_[opcode._3]) * 2;
                break;
            }
            case 0xA: {
                reg_i_ = opcode._3last;
                break;
            }
            case 0xB: {
                pc_ = opcode._3last + regs_[0];
                break;
            }
            case 0xC: {
                break;
            }
            case 0xD: {
                auto byte_cnt = regs_[opcode._4];
                auto x = regs_[opcode._2];
                for (uint8_t i = 0; i < byte_cnt; i++) {
                    auto y = regs_[opcode._3] + i;
                    auto cur_line = screen_[y];
                    auto cur_draw = read(reg_i_ + i);
                    cur_line = std::rotr(cur_line, x);
                    cur_line ^= cur_draw;
                    cur_line = std::rotl(cur_line, x);
                }
                redraw();
                break;
            }
            case 0xE: {
                break;
            }
            case 0xF: {
                break;
            }
        }
    }
    void Interpreter::run_8inst(const Opcode& opcode) {
        switch (opcode._4) {
            case 0: {
                regs_[opcode._2] = regs_[opcode._3];
                break;
            }
            case 1: {
                regs_[opcode._2] |= regs_[opcode._3];
                break;
            }
            case 2: {
                regs_[opcode._2] &= regs_[opcode._3];
                break;
            }
            case 3: {
                regs_[opcode._2] ^= regs_[opcode._3];
                break;
            }
            case 4: {
                regs_[0xF] = __builtin_add_overflow(regs_[opcode._2], regs_[opcode._3], &regs_[opcode._2]);
                break;
            }
            case 5: {
                regs_[0xF] = regs_[opcode._2] > regs_[opcode._3];
                regs_[opcode._2] -= regs_[opcode._3];
                break;
            }
            case 6: {
                regs_[0xF] = regs_[opcode._2] & 0b1;
                regs_[opcode._2] >>= 1;
                break;
            }
            case 7: {
                regs_[0xF] = regs_[opcode._3] > regs_[opcode._2];
                regs_[opcode._2] = regs_[opcode._3] - regs_[opcode._2];
                break;
            }
            case 0xE: {
                regs_[0xF] = regs_[opcode._2] >> 7;
                regs_[opcode._2] <<= 1;
                break;
            }
        }
    }
    void Interpreter::clear_screen() {
        for (std::size_t i = 0; i < screen_color_data_.size(); i++) {
            if (i & 0b11) {
                screen_color_data_[i] = 1.0f;
            } else {
                screen_color_data_[i] = 0.0f;
            }
        }
    }
    void Interpreter::redraw() {
        for (int j = 0; j < 32; j++) {
            uint64_t line = screen_[j];
            for (int i = 0; i < 64; i++) {
                auto index = (i + j * 32) * 4;
                screen_color_data_[index++] = line & (1 << i);
                screen_color_data_[index++] = line & (1 << i);
                screen_color_data_[index++] = line & (1 << i);
                screen_color_data_[index] = 1.0f;
            }
        }
    }
    uint8_t Interpreter::read(uint16_t addr) {
        switch (addr & 0xFF00) {
            case 0x0000: {
                // Get font
                // Example addr: 0x00D2, 3rd byte of letter 'D'
                return font_[((addr >> 8) * 5) + addr & 0xF];
            }
        }
    }
}