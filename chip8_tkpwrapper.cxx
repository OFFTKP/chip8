#include <chip8/chip8_tkpwrapper.hxx>
#include <iostream>
#include <GL/glew.h>

namespace TKPEmu::Chip8 {
    Chip8::Chip8() {
		// TODO_IMPORTANT: move this code to emulator
        EmulatorImage.width = 64;
        EmulatorImage.height = 32;
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, image_texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			EmulatorImage.width,
			EmulatorImage.height,
			0,
			GL_RGBA,
			GL_FLOAT,
			NULL
		);
		glBindTexture(GL_TEXTURE_2D, 0);
		EmulatorImage.texture = image_texture;
    }
    Chip8::Chip8(std::any args) : Chip8() {
		key_mappings_ = std::any_cast<Chip8Keys>(args);
	}
	Chip8::~Chip8() {
		Stopped.store(true);
		if (start_options != EmuStartOptions::Console)
			glDeleteTextures(1, &EmulatorImage.texture);
	}
    bool Chip8::load_file(std::string path) {
		inter_.load_file(path);
        Loaded = true;
        return true;
    }
    void Chip8::start_debug() {
		auto func = [this]() {
			std::lock_guard<std::mutex> lguard(ThreadStartedMutex);
			Loaded = true;
			Loaded.notify_all();
			Paused = true;
			Stopped = false;
			Step = false;
			Reset();
			// Emulation doesn't break on first instruction
			bool first_instr = true;
			while (!Stopped.load()) {
				if (!Paused.load()) {
					std::lock_guard<std::mutex> lg(DebugUpdateMutex);
					// bool broken = false;
					// if (!first_instr) {
					// 	for (const auto& bp : Breakpoints) {
					// 		bool brk = bp.Check();
					// 		if (brk) {
					// 			InstructionBreak.store(cpu_.PC);
					// 			Paused.store(true);
					// 			broken = true;
					// 		}
					// 	}
					// }
					// first_instr = false;
					// if (!broken)
                    update();
				} else {
					Step.wait(false);
					std::lock_guard<std::mutex> lg(DebugUpdateMutex);
					Step.store(false);
					update();
					// InstructionBreak.store(cpu_.PC);
				}
			}
		};
		UpdateThread = std::thread(func);
		UpdateThread.detach();
	}
    void Chip8::update() {
        inter_.Update();
    }
    void Chip8::reset_skip() {
        inter_.reset();
    }
	void Chip8::HandleKeyDown(SDL_Keycode key) {
		for (int i = 0; i < 16; i++) {
			if (key_mappings_[i] == key) {
				inter_.key_pressed_[i] = true;
				break;
			}
		}
	}
	void Chip8::HandleKeyUp(SDL_Keycode key) {
		for (int i = 0; i < 16; i++) {
			if (key_mappings_[i] == key) {
				inter_.key_pressed_[i] = false;
				break;
			}
		}
	}
}