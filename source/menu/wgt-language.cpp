#include "./wgt-language.hpp"
#include "./stack-gui.hpp"
#include "./headsup-gui.hpp"

#include "../resource/id.hpp"
#include "../resource/vfs.hpp"
#include "../system/input.hpp"
#include "../system/audio.hpp"
#include "../system/kernel.hpp"
#include "../utility/setup-file.hpp"

static constexpr arch_t kTotalVisible = 9;
static const glm::vec2 kLangDefaultPosition = glm::vec2(4.0f, 2.0f);
static const glm::vec2 kLangBottomPosition = glm::vec2(120.0f, 2.0f);

wgt_language_t::wgt_language_t(arch_t flags) :
	widget_i(flags),
	languages(),
	cursor(0),
	first(0),
	last(kTotalVisible),
	text(),
	arrow()
{

}

void wgt_language_t::init(const input_t&, const video_t&, audio_t&, const music_t&, kernel_t&) {
	ready = true;
	const std::string i18n_path = vfs::resource_path(vfs_resource_path_t::I18N);
	languages = vfs::file_list(i18n_path);
	text.set_font(vfs::font(0));
	text.set_position(kLangDefaultPosition);
	arrow.set_file(vfs::animation(res::anim::Heads));
	arrow.set_state(1);
	arrow.set_position(
		text.get_font_size().x,
		4.0f + kLangDefaultPosition.y +
		(text.get_font_size().y * 2.0f)
	);
	this->setup_text();
}

void wgt_language_t::handle(setup_file_t& config, input_t& input, video_t&, audio_t& audio, music_t&, kernel_t& kernel, stack_gui_t& stack_gui, headsup_gui_t& headsup_gui) {
	bool selection = false;
	if (input.pressed[btn_t::Up]) {
		if (cursor > 0) {
			--cursor;
			if (cursor < first) {
				--first;
				--last;
				this->setup_text();
			} else {
				arrow.mut_position(
					0.0f, -text.get_font_size().y
				);
			}
			audio.play(res::sfx::Select, 0);
		}
	} else if (input.pressed[btn_t::Down]) {
		if (cursor < languages.size() - 1) {
			++cursor;
			if (cursor >= last) {
				++first;
				++last;
				this->setup_text();
			} else {
				arrow.mut_position(
					0.0f, text.get_font_size().y
				);
			}
			audio.play(res::sfx::Select, 0);
		}
	} else if (input.pressed[btn_t::Yes]) {
		active = false;
		selection = true;
	}
	if (input.pressed[btn_t::No] or input.pressed[btn_t::Options]) {
		active = false;
	}
	if (!active) {
		input.flush();
		if (!selection) {
			audio.play(res::sfx::Inven, 0);
		} else {
			audio.play(res::sfx::TitleBeg, 0);
			headsup_gui.fade_out();
			kernel.buffer_language(languages[cursor]);
			stack_gui.clear();
		}
	}
}

void wgt_language_t::update(real64_t delta) {
	if (ready and active) {
		arrow.update(delta);
	}
}

void wgt_language_t::render(renderer_t& renderer) const {
	if (ready and active) {
		text.render(renderer);
		arrow.render(renderer);
	}
}

void wgt_language_t::invalidate() const {
	if (ready and active) {
		text.invalidate();
		arrow.invalidate();
	}
}

void wgt_language_t::setup_text() {
	text.set_string(vfs::i18n_find("Language", 0));
	for (arch_t it = first; it < languages.size() and it != last; ++it) {
		text.append_string("\t " + languages[it] + '\n');
	}
}
