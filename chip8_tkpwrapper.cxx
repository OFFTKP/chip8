#include <chip8/chip8_tkpwrapper.hxx>
#include <iostream>
#include <GL/glew.h>

namespace TKPEmu::Chip8 {
    Chip8::Chip8() {}
    Chip8::Chip8(std::any args) : Chip8() {
		key_mappings_ = std::any_cast<Chip8Keys>(args);
	}
	Chip8::~Chip8() {
		Stopped.store(true);
	}
    bool Chip8::load_file(std::string path) {
		inter_.load_file(path);
        Loaded = true;
        return true;
    }
    void Chip8::start() {
		Loaded = true;
		Loaded.notify_all();
		Paused = true;
		Stopped = false;
		Step = false;
		Reset();
		while (!Stopped.load()) {
			if (!Paused.load()) {
				update();
			} else {
				Step.wait(false);
				Step.store(false);
				update();
			}
		}
	}
    void Chip8::update() {
        inter_.Update();
    }
    void Chip8::reset() {
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
	bool& Chip8::IsReadyToDraw() {
		return inter_.should_draw_;
	}
	void* Chip8::GetScreenData() {
		return inter_.GetScreenData();
	}
}