// Copyright 2024 Stone-Engine

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Stone::Scene {

struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0, 0, 1);
	glm::vec3 tangent = glm::vec3(1, 0, 0);
	glm::vec3 bitangent = glm::vec3(0, 1, 0);
	glm::vec2 uv = glm::vec2(0);

	Vertex() = default;
	Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b, const glm::vec2 &uv);
	Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv);
};

struct WeightVertex : Vertex {
	glm::vec4 weights = glm::vec4(0.0f);
	glm::ivec4 ids = glm::ivec4(0);

	WeightVertex() = default;
	WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b, const glm::vec2 &uv,
				 const glm::vec4 &w, const glm::ivec4 &i);
	WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv);
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
