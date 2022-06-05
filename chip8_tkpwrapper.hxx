#pragma once
#ifndef TKP_CHIP8_TKPWRAPPER_H
#define TKP_CHIP8_TKPWRAPPER_H
#include <include/emulator.h>
#include <include/TKPImage.h>
#include <chip8/chip8_inter.hxx>

namespace TKPEmu::Chip8 {
	using Chip8Keys = std::array<SDL_Keycode, 16>;
    class Chip8 : public Emulator {
	public:
        Chip8();
        Chip8(std::any args);
        ~Chip8();
		void HandleKeyDown(SDL_Keycode key) override;
		void HandleKeyUp(SDL_Keycode key) override;
		void* GetScreenData() override {
            return inter_.GetScreenData();
        }
        bool& IsReadyToDraw() override {
            return inter_.should_draw_;
        }
		// std::string GetEmulatorName() override;
		// std::string GetScreenshotHash() override;
		// bool IsReadyToDraw() override;
    private:
        Interpreter inter_;
        // void v_log_state() override;
		// void save_state(std::ofstream& ofstream) override;
		// void load_state(std::ifstream& ifstream) override;
		// void start_normal() override;
		void start_debug() override;
		// void start_console() override;
		// void reset_normal() override;
		void reset_skip() override;
		bool load_file(std::string path) override;
		void update() override;
		Chip8Keys key_mappings_;
		// std::string print() const override;
    };
}
#endif