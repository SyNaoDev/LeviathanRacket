#include "./ghost.hpp"
#include "./particles.hpp"

#include "../component/kontext.hpp"
#include "../component/location.hpp"
#include "../component/health.hpp"
#include "../component/sprite.hpp"
#include "../resource/id.hpp"
#include "../system/audio.hpp"

// Functions

void ai::ghost::ctor(entt::entity s, kontext_t& kontext) {
	auto& location = kontext.get<location_t>(s);
	location.bounding = rect_t(4.0f, 0.0f, 8.0f, 16.0f);

	auto& sprite = kontext.assign_if<sprite_t>(s, res::anim::Ghost);
	sprite.layer = layer_value::Automatic;
	sprite.pivot = glm::vec2(8.0f, 16.0f);
	sprite.position = location.position;

	if (location.direction & direction_t::Left) {
		sprite.mirroring = mirroring_t::Horizontal;
	}

	auto& health = kontext.assign_if<health_t>(s);
	health.reset(8, 8, 0, 1);
	health.flags[health_flags_t::Leviathan] = true;

	kontext.assign_if<routine_t>(s, ghost::tick);
}


void ai::ghost::tick(entt::entity s, routine_tuple_t& rtp) {
	auto& sprite = rtp.kontext.get<sprite_t>(s);
	if (sprite.state == 1 and sprite.finished()) {
		sprite.new_state(0);
	}
	auto& health = rtp.kontext.get<health_t>(s);
	if (health.current <= 0) {
		auto& location = rtp.kontext.get<location_t>(s);
		rtp.kontext.spawn(ai::blast_medium::type, location.center());
		rtp.kontext.smoke(location.center(), 3);
		rtp.kontext.shrapnel(location.center(), 1);
		rtp.audio.play(res::sfx::NpcDeath0, 6);
	} else if (health.flags[health_flags_t::Hurt]) {
		health.flags[health_flags_t::Hurt] = false;

		auto& location = rtp.kontext.get<location_t>(s);
		rtp.kontext.shrapnel(location.center(), 2);

		sprite.shake = 0.3f;
		sprite.new_state(1);
	}
}

// Tables

LEVIATHAN_CTOR_TABLE_CREATE(routine_ctor_generator_t, ghost) {
	LEVIATHAN_TABLE_PUSH(ai::ghost::type, ai::ghost::ctor);
}
