#ifndef LEVIATHAN_INCLUDED_SYSTEM_INPUT_HPP
#define LEVIATHAN_INCLUDED_SYSTEM_INPUT_HPP

#include <map>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_events.h>

#include "./button.hpp"

struct setup_file_t;

namespace __enum_policy {
	enum type : arch_t {
		Run,
		Stop,
		Quit
	};
}

using policy_t = __enum_policy::type;

struct input_t : public not_copyable_t {
public:
	input_t();
	input_t(input_t&&) = default;
	input_t& operator=(input_t&&) = default;
	~input_t();
public:
	bool init(const setup_file_t& config);
	policy_t poll(policy_t policy, bool(*callback)(const SDL_Event*));
	policy_t poll(policy_t policy);
	void flush();
	bool has_joystick_connection() const;
	bool has_valid_recording() const;
	std::string get_scancode_name(arch_t index) const;
	std::string get_joystick_button(arch_t index) const;
	std::string get_config_name(arch_t index, bool_t is_joystick) const;
	sint_t receive_record();
	void set_nothings_recording();
	void set_keyboard_recording();
	btn_t set_keyboard_binding(sint_t code, arch_t btn);
	void set_joystick_recording();
	btn_t set_joystick_binding(sint_t code, arch_t btn);
private:
	void all_key_bindings(const setup_file_t& config);
	void all_joy_bindings(const setup_file_t& config);
public:
	std::bitset<btn_t::Total> pressed, holding;
	glm::vec2 position;
private:
	std::map<sint_t, btn_t> key_bind, joy_bind;
	sint_t recorder;
	SDL_Joystick* joystick;
};

#endif // LEVIATHAN_INCLUDED_SYSTEM_INPUT_HPP
