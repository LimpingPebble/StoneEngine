// Copyright 2024 Stone-Engine

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Stone::Scene {

/**
 * @brief Represents a vertex in a 3D model.
 */
struct Vertex {
	glm::vec3 position = glm::vec3(0);		  /**< The position of the vertex. */
	glm::vec3 normal = glm::vec3(0, 0, 1);	  /**< The normal vector of the vertex. */
	glm::vec3 tangent = glm::vec3(1, 0, 0);	  /**< The tangent vector of the vertex. */
	glm::vec3 bitangent = glm::vec3(0, 1, 0); /**< The bitangent vector of the vertex. */
	glm::vec2 uv = glm::vec2(0);			  /**< The texture coordinates of the vertex. */

	Vertex() = default;
	Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b, const glm::vec2 &uv);
	Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv);
};

/**
 * @brief Represents a vertex in a deformable 3D model.
 */
struct WeightVertex : Vertex {
	glm::vec4 weights = glm::vec4(0.0f); /**< The weights of the bones affecting the vertex. */
	glm::ivec4 ids = glm::ivec4(0);		 /**< The IDs of the bones affecting the vertex. */

	WeightVertex() = default;
	WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec3 &t, const glm::vec3 &b, const glm::vec2 &uv,
				 const glm::vec4 &w, const glm::ivec4 &i);
	WeightVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv);
};

/**
 * @brief Enum class representing the axis directions.
 */
enum class AxisDirection : uint8_t {
	PositiveX = 0, /**< Positive X-axis. */
	NegativeX = 1, /**< Negative X-axis. */
	PositiveY = 2, /**< Positive Y-axis. */
	NegativeY = 3, /**< Negative Y-axis. */
	PositiveZ = 4, /**< Positive Z-axis. */
	NegativeZ = 5, /**< Negative Z-axis. */
};

} // namespace Stone::Scene
