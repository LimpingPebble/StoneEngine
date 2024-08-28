// Copyright 2024 Stone-Engine

#include "Scene/Vertex.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Stone::Scene {

Vertex::Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b, const glm::vec2 &uv)
	: position(p), normal(n), tangent(t), bitangent(b), uv(uv) {
}

Vertex::Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv)
	: position(p), normal(n), tangent(), bitangent(), uv(uv) {
	if (normal == glm::vec3(0, 1, 0))
		tangent = glm::vec3(1, 0, 0);
	else if (normal == glm::vec3(0, -1, 0))
		tangent = glm::vec3(-1, 0, 0);
	else
		tangent = glm::cross(glm::vec3(0, 1, 0), normal);
	bitangent = glm::cross(tangent, normal);
}

Vertex::Vertex(const glm::vec3 &p, const glm::vec2 &uv)
	: Vertex(p, glm::vec3(0, 1, 0), uv) {
}

WeightVertex::WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b,
						   const glm::vec2 &uv, const glm::vec4 &w, const glm::ivec4 &i)
	: Vertex(p, n, t, b, uv), weights(w), ids(i) {
}

WeightVertex::WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv)
	: Vertex(p, n, uv), weights(), ids() {
}

WeightVertex::WeightVertex(const glm::vec3 &p, const glm::vec2 &uv)
	: WeightVertex(p, glm::vec3(0, 1, 0), uv) {
}

} // namespace Stone::Scene
