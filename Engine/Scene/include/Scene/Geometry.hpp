// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Stone::Scene {

struct Plane {
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
	float distance = 0.0f;

	Plane() = default;

	Plane(const glm::vec3 &n, float d) : normal(n), distance(d) {
	}
};

struct Sphere {
	glm::vec3 center = glm::vec3(0.0f);
	float radius = 1.0f;

	Sphere() = default;

	Sphere(const glm::vec3 &c, float r) : center(c), radius(r) {
	}
};

struct Box {
	glm::vec3 min = glm::vec3(-0.5f);
	glm::vec3 max = glm::vec3(0.5f);

	Box() = default;

	Box(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {
	}
};

struct Line {
	glm::vec3 origin = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

	Line() = default;

	Line(const glm::vec3 &o, const glm::vec3 &d) : origin(o), direction(d) {
	}
};

struct Cone {
	glm::vec3 origin = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	float angle = M_PI_4;
	float length = 1.0f;

	Cone() = default;

	Cone(const glm::vec3 &o, const glm::vec3 &d, float a, float l) : origin(o), direction(d), angle(a), length(l) {
	}
};

struct Frustum {
	Plane planes[6] = {Plane(), Plane(), Plane(), Plane(), Plane(), Plane()};

	Frustum() = default;

	Frustum(const Plane &p0, const Plane &p1, const Plane &p2, const Plane &p3, const Plane &p4, const Plane &p5) {
		planes[0] = p0;
		planes[1] = p1;
		planes[2] = p2;
		planes[3] = p3;
		planes[4] = p4;
		planes[5] = p5;
	}
};

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Plane &plane, float size = 1.0f);
std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Sphere &sphere, int rings = 16);
std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Box &box);
std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Line &line);
std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Cone &cone, int segments = 16);
std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Frustum &frustum);


} // namespace Stone::Scene
