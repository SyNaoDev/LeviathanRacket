#include "./camera.hpp"

#include "../actor/naomi.hpp"
#include "../component/kontext.hpp"
#include "../component/location.hpp"
#include "../utility/constants.hpp"
#include "../utility/rng.hpp"
#include "../video/display-list.hpp"

#include <glm/gtc/matrix_transform.hpp>

static const glm::vec2 kDefaultSpeed  = glm::vec2(16.0f, 8.0f);
static const glm::vec2 kDefaultLowest = glm::vec2(8.0f, 6.0f);
static const glm::vec2 kDefaultCenter = (constants::NormalDimensions<real_t>() / 2.0f) + kDefaultLowest;

camera_t::camera_t() :
	identity(0),
	cycling(false),
	indefinite(false),
	timer(0.0),
	view_limits(kDefaultLowest, constants::NormalDimensions<real_t>()),
	position(kDefaultCenter),
	dimensions(constants::NormalDimensions<real_t>()),
	offsets(0.0f),
	quake_power(0.0f),
	view_angle(0.0f)
{

}

void camera_t::reset() {
	identity = 0;
	cycling = false;
	indefinite = false;
	timer = 0.0;
	view_limits = rect_t(kDefaultLowest, constants::NormalDimensions<real_t>());
	position = kDefaultCenter;
	dimensions = constants::NormalDimensions<real_t>();
	offsets = glm::zero<glm::vec2>();
	quake_power = 0.0f;
	view_angle = 0.0f;
}

void camera_t::handle(const kontext_t& kontext, const naomi_state_t& naomi_state) {
	glm::vec2 center = glm::zero<glm::vec2>();
	if (identity != 0) {
		entt::entity actor = kontext.search_id(identity);
		if (actor != entt::null) {
			auto& location = kontext.get<location_t>(actor);
			center = location.center();
		} else {
			identity = 0;
		}
	} else {
		center = naomi_state.camera_placement();
	}
	position += (center - position) / kDefaultSpeed;
	view_limits.push_fix(position, dimensions);
}

void camera_t::update(real64_t delta) {
	if (quake_power != 0.0) {
		if (cycling) {
			cycling = false;
			offsets = -offsets;
			view_angle = 0.0f;
		} else {
			cycling = true;
			offsets = glm::vec2(
				rng::next(-quake_power, quake_power),
				rng::next(-quake_power, quake_power)
			);
			view_angle = glm::radians(
				rng::next(-quake_power, quake_power)
			);
		}
		if (!indefinite) {
			timer -= delta;
			if (timer <= 0.0) {
				cycling = false;
				timer = 0.0;
				offsets = glm::zero<glm::vec2>();
				quake_power = 0.0f;
				view_angle = 0.0f;
			}
		}
	}
}

void camera_t::set_view_limits(rect_t view_limits) {
	this->view_limits.w = view_limits.w - constants::TileSize<real_t>();
	this->view_limits.h = view_limits.h - constants::TileSize<real_t>() - 4.0f;
	position = kDefaultCenter;
	dimensions = constants::NormalDimensions<real_t>();
}

void camera_t::set_focus(glm::vec2 position) {
	view_limits.push_fix(position, dimensions);
	this->position = position;
}

void camera_t::follow(sint_t identity) {
	this->identity = identity;
}

void camera_t::quake(real_t power, real64_t seconds) {
	if (!indefinite) {
		cycling = false;
		timer = seconds;
		quake_power = power;
		view_angle = 0.0f;
	}
}

void camera_t::quake(real_t power) {
	if (!indefinite) {
		cycling = false;
		indefinite = false;
		timer = 0.0;
		offsets = glm::zero<glm::vec2>();
		quake_power = power;
		view_angle = 0.0f;
	} else {
		cycling = false;
		indefinite = true;
		timer = 0.0;
		quake_power = 0.0f;
		view_angle = 0.0f;
	}
}

rect_t camera_t::get_viewport() const {
	return rect_t(
		position - (dimensions / 2.0f),
		dimensions
	);
}

arch_t camera_t::get_tile_range(glm::ivec2 first, glm::ivec2 last) const {
	glm::ivec2 result = last - first;
	return
		static_cast<arch_t>(result.x) *
		static_cast<arch_t>(result.y) *
		display_list_t::SingleQuad;
}

glm::mat4 camera_t::get_matrix() const {
	glm::mat4 result = glm::scale(
		glm::mat4(1.0f), glm::vec3(2.0f / dimensions.x, 2.0f / -dimensions.y, 0.0f)
	);
	if (view_angle != 0.0f) {
		result = glm::rotate(
			result,
			view_angle,
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
	}
	result = glm::translate(
		result, -glm::vec3(glm::round(position + offsets), 0.0f)
	);
	return result;
}
