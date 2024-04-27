// Copyright 2024 Stone-Engine

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Stone::Scene {

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 uv;
};

struct WeightVertex : Vertex {
	glm::ivec4 boneIDs;
	glm::vec4 boneWeights;
};

enum class CubeAxis : uint8_t {
	Positive_X = 0,
	Negative_X = 1,
	Positive_Y = 2,
	Negative_Y = 3,
	Positive_Z = 4,
	Negative_Z = 5,
};

} // namespace Stone::Scene
