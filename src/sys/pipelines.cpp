#include "./pipelines.hpp"

static constexpr byte_t kBlankVert420[] = R"(
#version 420 core
layout(binding = 0, std140) uniform transforms {
	mat4 viewport;
	vec2 dimensions;
	vec2 resolution;
};
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
out STAGE {
	layout(location = 0) vec4 color;
} vs;
void main() {
	gl_Position = viewport * vec4(position, 0.0f, 1.0f);
	vs.color = color;
})";

static constexpr byte_t kBlankVert330[] = R"(
#version 330 core
layout(std140) uniform transforms {
	mat4 viewport;
	vec2 dimensions;
	vec2 resolution;
};
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
out STAGE {
	vec4 color;
} vs;
void main() {
	gl_Position = viewport * vec4(position, 0.0f, 1.0f);
	vs.color = color;
})";

std::string pipelines::blank_vert(glm::ivec2 version) {
	if (version[0] == 4 and version[1] >= 2) {
		return kBlankVert420;
	}
	return kBlankVert330;
}

static constexpr byte_t kMajorVert420[] = R"(
#version 420 core
layout(binding = 0, std140) uniform transforms {
	mat4 viewport;
	vec2 dimensions;
	vec2 resolution;
};
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 uvcoords;
layout(location = 2) in float alpha;
out STAGE {
	layout(location = 0) vec3 uvcoords;
	layout(location = 1) float alpha;
} vs;
void main() {
	gl_Position = viewport * vec4(position, 0.0f, 1.0f);
	vs.uvcoords = uvcoords;
	vs.alpha = alpha;
})";

static constexpr byte_t kMajorVert330[] = R"(
#version 330 core
layout(std140) uniform transforms {
	mat4 viewport;
	vec2 dimensions;
	vec2 resolution;
};
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 uvcoords;
layout(location = 2) in float alpha;
out STAGE {
	vec3 uvcoords;
	float alpha;
} vs;
void main() {
	gl_Position = viewport * vec4(position, 0.0f, 1.0f);
	vs.uvcoords = uvcoords;
	vs.alpha = alpha;
})";

std::string pipelines::major_vert(glm::ivec2 version) {
	if (version[0] == 4 and version[1] >= 2) {
		return kMajorVert420;
	}
	return kMajorVert330;
}

static constexpr byte_t kColorsFrag420[] = R"(
#version 420 core
in STAGE {
	layout(location = 0) vec4 color;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	fragment = fs.color;
})";

static constexpr byte_t kColorsFrag330[] = R"(
#version 330 core
in STAGE {
	vec4 color;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	fragment = fs.color;
})";

std::string pipelines::colors_frag(glm::ivec2 version) {
	if (version[0] == 4 and version[1] >= 2) {
		return kColorsFrag420;
	}
	return kColorsFrag330;
}

static constexpr byte_t kSpritesFrag420[] = R"(
#version 420 core
layout(binding = 0) uniform sampler2D diffuse_map;
in STAGE {
	layout(location = 0) vec3 uvcoords;
	layout(location = 1) float alpha;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	vec4 color = texture(diffuse_map, fs.uvcoords.xy);
	fragment = vec4(color.rgb, color.a * fs.alpha);
})";

static constexpr byte_t kSpritesFrag330[] = R"(
#version 330 core
uniform sampler2D diffuse_map;
in STAGE {
	vec3 uvcoords;
	float alpha;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	vec4 color = texture(diffuse_map, fs.uvcoords.xy);
	fragment = vec4(color.rgb, color.a * fs.alpha);
})";

std::string pipelines::sprites_frag(glm::ivec2 version) {
	if (version[0] == 4 and version[1] >= 2) {
		return kSpritesFrag420;
	}
	return kSpritesFrag330;
}

static constexpr byte_t kIndexedFrag420[] = R"(
#version 420 core
layout(binding = 0) uniform sampler2D indexed_map;
layout(binding = 1) uniform sampler2D palette_map;
in STAGE {
	layout(location = 0) vec3 uvcoords;
	layout(location = 1) float alpha;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	vec4 index = texture(indexed_map, fs.uvcoords.xy);
	vec4 color = texture(palette_map, vec2(index[0], fs.uvcoords.z));
	fragment = vec4(color.rgb, color.a * fs.alpha);
})";

static constexpr byte_t kIndexedFrag330[] = R"(
#version 330 core
uniform sampler2D indexed_map;
uniform sampler2D palette_map;
in STAGE {
	vec3 uvcoords;
	float alpha;
} fs;
layout(location = 0) out vec4 fragment;
void main() {
	vec4 index = texture(indexed_map, fs.uvcoords.xy);
	vec4 color = texture(palette_map, vec2(index[0], fs.uvcoords.z));
	fragment = vec4(color.rgb, color.a * fs.alpha);
})";

std::string pipelines::indexed_frag(glm::ivec2 version) {
	if (version[0] == 4 and version[1] >= 2) {
		return kIndexedFrag420;
	}
	return kIndexedFrag330;
}