#ifndef LEVIATHAN_INCLUDED_MENU_META_GUI_HPP
#define LEVIATHAN_INCLUDED_MENU_META_GUI_HPP

#ifdef LEVIATHAN_USES_META

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

#include "../types.hpp"

struct input_t;
struct video_t;
struct renderer_t;

struct meta_state_t : public not_copyable_t {
public:
	meta_state_t();
	meta_state_t(meta_state_t&&) = default;
	meta_state_t& operator=(meta_state_t&&) = default;
	~meta_state_t();
public:
	bool init(const video_t& video);
	void handle(const input_t& input);
	void flush() const;
public:
	typedef bool(*event_callback_t)(const SDL_Event*);
	static event_callback_t get_event_callback();
	static bool_t Hitboxes, Framerate;
private:
	static bool_t Ready;
	SDL_Window* window;
	SDL_GLContext context;
};

#else

#include <SDL2/SDL_events.h>

#include "../types.hpp"

struct meta_state_t : public not_copyable_t {
public:
	meta_state_t() = default;
	meta_state_t(meta_state_t&&) = default;
	meta_state_t& operator=(meta_state_t&&) = default;
	~meta_state_t() = default;
public:
	typedef bool(*event_callback_t)(const SDL_Event*);
	static event_callback_t get_event_callback() { return nullptr; }
};

#endif

#endif // LEVIATHAN_INCLUDED_MENU_META_GUI_HPP
