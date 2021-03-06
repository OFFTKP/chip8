#include <chip8/chip8_tkpwrapper.hxx>
#include <iostream>

namespace TKPEmu::Chip8 {
    Chip8::Chip8() {}
    // Chip8::Chip8(std::unique_ptr<OptionsBase> args) : Chip8() {
	// 	// key_mappings_ = std::any_cast<Chip8Keys>(args);
	// }
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
		Paused = false;
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
	void Chip8::HandleKeyDown(uint32_t key) {
		for (int i = 0; i < 16; i++) {
			if (key_mappings_[i] == key) {
				inter_.key_pressed_[i] = true;
				break;
			}
		}
	}
	void Chip8::HandleKeyUp(uint32_t key) {
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
	bool Chip8::poll_uncommon_request(const Request& request) {
		return false;
	}
}