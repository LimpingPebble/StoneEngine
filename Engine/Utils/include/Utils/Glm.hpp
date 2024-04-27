// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

namespace std {
string to_string(const glm::vec2 &v);
string to_string(const glm::vec3 &v, bool color = false);
string to_string(const glm::vec4 &v, bool color = false);
string to_string(const glm::mat3 &m);
string to_string(const glm::mat4 &m);
string to_string(const glm::quat &q);
} // namespace std

namespace glm {
float &value_at(mat3 &m, int r, int c);
float &value_at(mat4 &m, int r, int c);
} // namespace glm
