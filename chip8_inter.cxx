#include <chip8/chip8_inter.hxx>
#include <bit>
#include <fstream>
#include <cstring>

namespace {
    constexpr std::array<uint8_t, 0x50> font_ {
        0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0,
        0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80,
        0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
        0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80,
        0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80
    };
    std::ifstream::pos_type filesize(const char* filename)
    {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return in.tellg(); 
    }
    unsigned char reverse(unsigned char b) { // TODO: remove this
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        return b;
    }
}

namespace TKPEmu::Chip8 {
    Interpreter::Interpreter() {}
    void Interpreter::reset() {
        std::fill(screen_.begin(), screen_.end(), 0);
        std::fill(screen_color_data_.begin(), screen_color_data_.end(), 0);
        pc_ = 0x200;
        std::fill(regs_.begin(), regs_.end(), 0);
        i_ = 0;
        std::fill(stack_.begin(), stack_.end(), 0);
        clear_screen();
    }
    void Interpreter::Run(Opcode opcode) {
        switch (opcode._1) {
            case 0: {
                if (opcode.full == 0x00E0) {
                    clear_screen();
                } else if (opcode.full == 0x00EE) {
                    pc_ = stack_[sp_--];
                }
                break;
            }
            case 1: {
                pc_ = 0xFFF & opcode.full;
                break;
            }
            case 2: {
                ++sp_;
                sp_ &= 0xF;
                stack_[sp_] = pc_;
                pc_ = 0xFFF & opcode.full;
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
                i_ = opcode.full & 0xFFF;
                break;
            }
            case 0xB: {
                pc_ = opcode.full & 0xFFF + regs_[0];
                break;
            }
            case 0xC: {
                break;
            }
            case 0xD: {
                auto byte_cnt = opcode._4;
                auto x = regs_[opcode._2];
                auto y_init = regs_[opcode._3];
                for (uint8_t i = 0; i < byte_cnt; i++) {
                    auto y = y_init + i;
                    auto& cur_line = screen_[y];
                    uint8_t cur_draw = read((i_ + i) & 0xFFF);
                    cur_line = std::rotr(cur_line, x);
                    cur_line ^= reverse(cur_draw); // TODO: theres probably a better way to do this
                    cur_line = std::rotl(cur_line, x);
                }
                redraw();
                break;
            }
            case 0xE: {
                break;
            }
            case 0xF: {
                run_Finst(opcode);
                break;
            }
        }
    }
    void Interpreter::run_Finst(const Opcode& opcode) {
        switch (opcode._2b) {
            case 0x07: {
                regs_[opcode._2] = dt_;
                break;
            }
            case 0x0A: {
                break;
            }
            case 0x15: {
                dt_ = regs_[opcode._2];
                break;
            }
            case 0x18: {
                st_ = regs_[opcode._2];
                break;
            }
            case 0x1E: {
                i_ += regs_[opcode._2];
                break;
            }
            case 0x29: {
                i_ = regs_[opcode._2] * 5;
                break;
            }
            case 0x33: {
                uint8_t num = regs_[opcode._2];
                uint8_t ones = num % 10;
                num /= 10;
                uint8_t tens = num % 10;
                num /= 10;
                uint8_t hundreds = num;
                write(i_, hundreds);
                write(i_ + 1, tens);
                write(i_ + 2, ones);
                break;
            }
            case 0x55: {
                auto end = opcode._2;
                for (int i = 0; i <= end; i++) {
                    write(i_ + i, regs_[i]);
                }
                i_ += end + 1;
                break;
            }
            case 0x65: {
                auto end = opcode._2;
                for (int i = 0; i <= end; i++) {
                    regs_[i] = read(i_ + i);
                }
                i_ += end + 1;
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
        std::fill(screen_.begin(), screen_.end(), 0);
        for (std::size_t i = 0; i < screen_color_data_.size(); i++) {
            if ((i & 0b11) == 0b11)
                screen_color_data_[i] = 1.0f;
        }
        should_draw_ = true;
    }
    void Interpreter::redraw() {
        for (int j = 0; j < 32; j++) {
            uint64_t line = screen_[j];
            for (int i = 0; i < 64; i++) {
                auto index = i * 4 + j * 64 * 4;
                float on = !!(line & ((uint64_t)1 << i));
                screen_color_data_[index++] = on;
                screen_color_data_[index++] = on;
                screen_color_data_[index++] = on;
                screen_color_data_[index] = 1.0f;
            }
        }
        should_draw_ = true;
    }
    uint8_t Interpreter::read(uint16_t addr) {
        return mem_[addr];
    }
    void Interpreter::write(uint16_t addr, uint8_t data) {
        mem_[addr] = data;
    }
    bool Interpreter::load_file(const std::string& path) {
        mem_.fill(0);
        std::size_t size = filesize(path.c_str());
        std::ifstream is;
		is.open(path, std::ios::binary);
        if (is.is_open()) {
            is.read(reinterpret_cast<char*>(&mem_[0x200]), sizeof(uint8_t) * size);
            is.close();
            pc_ = 0x200;
            std::memcpy(&mem_[0], &font_[0], 0x50);
            return true;
        }
        return false;
    }
}