// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>

namespace Stone::Scene {

struct Plane {
	glm::vec3 normal;
	float distance;

	Plane(const glm::vec3& n, float d) : normal(n), distance(d) {}
};

struct Sphere {
	glm::vec3 center;
	float radius;

	Sphere(const glm::vec3& c, float r) : center(c), radius(r) {}
};

} // namespace Stone::Scene