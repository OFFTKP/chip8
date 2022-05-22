#include "chip8_inter.hxx"

namespace TKPEmu::Chip8 {
    Interpreter::Interpreter() {}

    void Interpreter::reset() {
        pc_ = 0x200;
        std::fill(regs_.begin(), regs_.end(), 0);
        reg_i_ = 0;
        std::fill(stack_.begin(), stack_.end(), 0);
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
}