// Copyright 2024 Stone-Engine

#include "Utils/Glm.hpp"

#include <glm/ext.hpp>

namespace glm {

float &value_at(mat3 &m, int r, int c) {
	return value_ptr(m)[3 * c + r];
}

float &value_at(mat4 &m, int r, int c) {
	return value_ptr(m)[4 * c + r];
}

} // namespace glm
